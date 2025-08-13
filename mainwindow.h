#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QShortcut>
#include <QFile>
#include <QDir>
#include <QRegularExpression>

#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_selectVideoBtn_clicked();
    void on_selectDirBtn_clicked();
    void on_playPauseBtn_clicked();

    // CHANGED: stop -> reload
    void on_reloadVideoBtn_clicked();

    void on_preVideoBtn_clicked();
    void on_nextVideoBtn_clicked();

    // Scrub + live preview
    void on_timeSlider_sliderMoved(int value);
    void on_timeSlider_sliderPressed();
    void on_timeSlider_sliderReleased();
    void tick();

private:
    Ui::MainWindow *ui;

    // Playback
    QTimer timer_;
    cv::VideoCapture cap_;
    double fps_ = 30.0;
    int frameCount_ = 0;
    int currentFrameIndex_ = 0;
    bool playing_ = false;
    bool sliderHeld_ = false;

    // Saving / state
    QString lastVideoPath_;
    QString saveDirPath_;
    int nextImageIndex_ = 1;

    // Shortcuts
    QShortcut *saveShortcut_ = nullptr;

    // Frame cache
    cv::Mat currentFrameBGR_;

    // Config (simple txt)
    QString configPath_;
    void loadConfig();
    void saveConfig() const;

    // Helpers
    void openVideo(const QString &path);
    void updateTimerFromFPS();
    void updateInfoLabels();
    void displayMat(const cv::Mat &bgr);
    void ensureSliderRange();
    void seekTo(int frameIndex);
    void stepRelative(int deltaFrames);
    void setPlaying(bool on);
    void recalcNextImageFromDir();
    static int extractLargestNumberInDir(const QString &dirPath);
    void saveCurrentFrame();
    static QImage matToQImage(const cv::Mat &bgr);
};

#endif // MAINWINDOW_H
