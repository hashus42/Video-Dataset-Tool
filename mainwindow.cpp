#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Install event filter if you later want to catch more keys
    this->installEventFilter(this);

    // Where we keep our tiny "database"
    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appData);
    configPath_ = appData + QDir::separator() + "config.txt";

    loadConfig();
    recalcNextImageFromDir();
    updateInfoLabels();

    // Connect timer for playback
    connect(&timer_, &QTimer::timeout, this, &MainWindow::tick);

    // Keyboard shortcut: press 'S' to save current frame
    saveShortcut_ = new QShortcut(QKeySequence(Qt::Key_S), this);
    connect(saveShortcut_, &QShortcut::activated, this, &MainWindow::saveCurrentFrame);

    // Slider behavior
    connect(ui->timeSlider, &QSlider::sliderPressed,  this, &MainWindow::on_timeSlider_sliderPressed);
    connect(ui->timeSlider, &QSlider::sliderReleased, this, &MainWindow::on_timeSlider_sliderReleased);

    // Reflect paths in labels if available
    if (!lastVideoPath_.isEmpty())
        ui->videoPathLabel->setText(lastVideoPath_);
    if (!saveDirPath_.isEmpty())
        ui->saveDirLabel->setText(saveDirPath_);

    // If last video exists, open it (but don't auto-play)
    if (!lastVideoPath_.isEmpty() && QFile::exists(lastVideoPath_))
        openVideo(lastVideoPath_);
}

MainWindow::~MainWindow()
{
    saveConfig();
    delete ui;
}

// ================== UI Slots ==================

void MainWindow::on_selectVideoBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select Video",
                                                lastVideoPath_.isEmpty() ? QDir::homePath() : QFileInfo(lastVideoPath_).absolutePath(),
                                                "Videos (*.mp4 *.avi *.mkv *.mov *.m4v *.webm);;All Files (*)");
    if (path.isEmpty()) return;

    openVideo(path);
    lastVideoPath_ = path;
    ui->videoPathLabel->setText(path);
    saveConfig();
}

void MainWindow::on_selectDirBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Save Directory",
                                                    saveDirPath_.isEmpty() ? QDir::homePath() : saveDirPath_);
    if (dir.isEmpty()) return;

    saveDirPath_ = dir;
    ui->saveDirLabel->setText(dir);

    recalcNextImageFromDir();
    updateInfoLabels();
    saveConfig();
}

void MainWindow::on_playPauseBtn_clicked()
{
    if (!cap_.isOpened()) return;
    setPlaying(!playing_);
}

void MainWindow::on_reloadVideoBtn_clicked()
{
    if (!cap_.isOpened()) return;
    // Restart from the beginning and start playing
    setPlaying(false);
    seekTo(0);
    setPlaying(true);
}

void MainWindow::on_preVideoBtn_clicked()
{
    if (!cap_.isOpened()) return;
    setPlaying(false);
    stepRelative(-1);
}

void MainWindow::on_nextVideoBtn_clicked()
{
    if (!cap_.isOpened()) return;
    setPlaying(false);
    stepRelative(+1);
}

void MainWindow::on_timeSlider_sliderMoved(int value)
{
    if (!cap_.isOpened()) return;
    seekTo(value);
}

void MainWindow::on_timeSlider_sliderPressed()
{
    sliderHeld_ = true;
    if (playing_) setPlaying(false);
}

void MainWindow::on_timeSlider_sliderReleased()
{
    if (!cap_.isOpened()) { sliderHeld_ = false; return; }
    // Finalize position at the released value (cheap, but ensures sync)
    int target = ui->timeSlider->value();
    seekTo(target);
    sliderHeld_ = false;
}

// ================== Playback Loop ==================

void MainWindow::tick()
{
    if (!cap_.isOpened()) return;

    cv::Mat frame;
    if (!cap_.read(frame))
    {
        // End of video => stop
        setPlaying(false);
        return;
    }

    currentFrameIndex_ = static_cast<int>(cap_.get(cv::CAP_PROP_POS_FRAMES)) - 1;
    currentFrameBGR_ = frame.clone();

    displayMat(currentFrameBGR_);
    if (!sliderHeld_)
        ui->timeSlider->setValue(currentFrameIndex_);
    updateInfoLabels();
}

// ================== Helpers ==================

void MainWindow::openVideo(const QString &path)
{
    if (cap_.isOpened()) cap_.release();

    cap_.open(path.toStdString());
    if (!cap_.isOpened())
    {
        QMessageBox::warning(this, "Error", "Failed to open video.");
        return;
    }

    fps_ = cap_.get(cv::CAP_PROP_FPS);
    if (fps_ <= 0.0) fps_ = 30.0;

    frameCount_ = static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_COUNT));
    currentFrameIndex_ = 0;

    ensureSliderRange();
    updateTimerFromFPS();

    // Show first frame
    seekTo(0);
    setPlaying(false);
}

void MainWindow::updateTimerFromFPS()
{
    int intervalMs = static_cast<int>(1000.0 / std::max(1.0, fps_));
    timer_.setInterval(intervalMs);
}

void MainWindow::ensureSliderRange()
{
    ui->timeSlider->setMinimum(0);
    ui->timeSlider->setMaximum(std::max(0, frameCount_ - 1));
    ui->timeSlider->setSingleStep(1);
    ui->timeSlider->setPageStep(std::max(1, frameCount_ / 20));
}

void MainWindow::seekTo(int frameIndex)
{
    if (!cap_.isOpened()) return;

    frameIndex = std::clamp(frameIndex, 0, std::max(0, frameCount_ - 1));
    cap_.set(cv::CAP_PROP_POS_FRAMES, frameIndex);

    cv::Mat frame;
    if (cap_.read(frame))
    {
        currentFrameIndex_ = static_cast<int>(cap_.get(cv::CAP_PROP_POS_FRAMES)) - 1;
        currentFrameBGR_ = frame.clone();
        displayMat(currentFrameBGR_);

        // IMPORTANT: don't fight the user while scrubbing
        if (!sliderHeld_)
            ui->timeSlider->setValue(currentFrameIndex_);
    }
    updateInfoLabels();
}

void MainWindow::stepRelative(int deltaFrames)
{
    int target = currentFrameIndex_ + deltaFrames;
    seekTo(target);
}

void MainWindow::setPlaying(bool on)
{
    playing_ = on;
    if (playing_) timer_.start();
    else          timer_.stop();

    // Update play/pause icon tooltip for clarity
    ui->playPauseBtn->setToolTip(playing_ ? "Pause" : "Play");
}

void MainWindow::displayMat(const cv::Mat &bgr)
{
    if (bgr.empty()) return;
    QImage img = matToQImage(bgr);
    // Keep aspect fit inside the QLabel
    QPixmap pix = QPixmap::fromImage(img).scaled(ui->videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->videoLabel->setPixmap(pix);
}

QImage MainWindow::matToQImage(const cv::Mat &bgr)
{
    cv::Mat rgb;
    if (bgr.channels() == 3)
        cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
    else if (bgr.channels() == 4)
        cv::cvtColor(bgr, rgb, cv::COLOR_BGRA2RGBA);
    else
        cv::cvtColor(bgr, rgb, cv::COLOR_GRAY2RGB);

    return QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
}

void MainWindow::updateInfoLabels()
{
    ui->frameInfoLabel->setText(QString("Frame: %1 / %2").arg(currentFrameIndex_).arg(frameCount_));
    ui->nextImageLabel->setText(QString("Next image: %1").arg(nextImageIndex_));
}

void MainWindow::saveCurrentFrame()
{
    if (currentFrameBGR_.empty())
        return;

    if (saveDirPath_.isEmpty())
    {
        QMessageBox::information(this, "Save directory required", "Please select a save directory first.");
        return;
    }

    QDir dir(saveDirPath_);
    if (!dir.exists())
        dir.mkpath(".");

    // Ensure numbering continues from largest numeric filename
    recalcNextImageFromDir();

    // Save as plain numeric names: 1.jpg, 2.jpg, ...
    const QString filename = QString::number(nextImageIndex_) + ".jpg";
    const QString fullPath = dir.filePath(filename);

    // Encode and save
    std::vector<int> params = { cv::IMWRITE_JPEG_QUALITY, 95 };
    bool ok = cv::imwrite(fullPath.toStdString(), currentFrameBGR_, params);
    if (!ok)
    {
        QMessageBox::warning(this, "Save failed", "Could not save image.");
        return;
    }

    ++nextImageIndex_;
    updateInfoLabels();
    saveConfig(); // persist latest paths + next index
}

void MainWindow::recalcNextImageFromDir()
{
    if (saveDirPath_.isEmpty())
    {
        nextImageIndex_ = 1;
        return;
    }
    nextImageIndex_ = extractLargestNumberInDir(saveDirPath_) + 1;
}

int MainWindow::extractLargestNumberInDir(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) return 0;

    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";
    QFileInfoList list = dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks | QDir::Readable);

    int maxNum = 0;
    QRegularExpression re("(\\d+)");
    for (const QFileInfo &fi : list)
    {
        const QString base = fi.completeBaseName(); // without extension
        QRegularExpressionMatchIterator it = re.globalMatch(base);
        while (it.hasNext())
        {
            QRegularExpressionMatch m = it.next();
            bool ok = false;
            int num = m.captured(1).toInt(&ok);
            if (ok) maxNum = std::max(maxNum, num);
        }
    }
    return maxNum;
}

// ================== Config TXT ==================

void MainWindow::loadConfig()
{
    QFile f(configPath_);
    if (!f.exists()) return;
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    while (!in.atEnd())
    {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;
        const int eq = line.indexOf('=');
        if (eq <= 0) continue;

        const QString key = line.left(eq).trimmed();
        const QString val = line.mid(eq + 1).trimmed();

        if (key == "last_video") lastVideoPath_ = val;
        else if (key == "save_dir") saveDirPath_ = val;
        else if (key == "next_image") nextImageIndex_ = val.toInt();
    }
    f.close();
}

void MainWindow::saveConfig() const
{
    QFile f(configPath_);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&f);
    out << "# Simple config for Video Dataset Preparation Tool\n";
    out << "last_video=" << lastVideoPath_ << "\n";
    out << "save_dir="   << saveDirPath_   << "\n";
    out << "next_image=" << nextImageIndex_ << "\n";
    f.close();
}

// ================== Events ==================

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    // (Reserved if you want additional key handling later)
    if (event->type() == QEvent::KeyPress)
    {
        auto *ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_S)
        {
            saveCurrentFrame();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
