#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

// Modern Light Blue Theme for Qt Application
void setModernLightBlueTheme(QApplication& app) {
    QPalette lightBluePalette;

    // Main background colors - light and airy
    lightBluePalette.setColor(QPalette::Window, QColor(245, 248, 255));          // Very light blue-white background
    lightBluePalette.setColor(QPalette::WindowText, QColor(30, 45, 70));         // Dark blue-gray text
    lightBluePalette.setColor(QPalette::Base, QColor(255, 255, 255));            // Pure white input fields
    lightBluePalette.setColor(QPalette::AlternateBase, QColor(235, 245, 255));   // Light blue alternate rows

    // Text colors
    lightBluePalette.setColor(QPalette::Text, QColor(25, 40, 65));              // Main text - dark blue
    lightBluePalette.setColor(QPalette::BrightText, QColor(0, 100, 200));       // Bright blue accent text
    lightBluePalette.setColor(QPalette::PlaceholderText, QColor(120, 140, 180)); // Muted placeholder text

    // Button styling
    lightBluePalette.setColor(QPalette::Button, QColor(220, 235, 255));         // Light blue button background
    lightBluePalette.setColor(QPalette::ButtonText, QColor(30, 70, 120));       // Dark blue button text

    // Interactive elements
    lightBluePalette.setColor(QPalette::Highlight, QColor(100, 160, 240));      // Selection/highlight
    lightBluePalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255)); // Selected text
    lightBluePalette.setColor(QPalette::Link, QColor(50, 120, 200));            // Links
    lightBluePalette.setColor(QPalette::LinkVisited, QColor(100, 80, 180));     // Visited links

    // Tooltips
    lightBluePalette.setColor(QPalette::ToolTipBase, QColor(250, 252, 255));    // Light tooltip background
    lightBluePalette.setColor(QPalette::ToolTipText, QColor(40, 60, 90));       // Dark tooltip text

    // Disabled states
    lightBluePalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(150, 160, 180));
    lightBluePalette.setColor(QPalette::Disabled, QPalette::Text, QColor(150, 160, 180));
    lightBluePalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(140, 150, 170));

    app.setPalette(lightBluePalette);

    // Add modern light stylesheet for enhanced styling
    app.setStyleSheet(R"(
        /* Main Window Styling */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f8faff, stop:1 #f0f5ff);
        }

        /* Button Styling */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #e8f2ff, stop:1 #d0e5ff);
            border: 2px solid #a0c8f0;
            border-radius: 8px;
            padding: 8px 16px;
            font-weight: bold;
            color: #1e4678;
            min-height: 20px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #d8eaff, stop:1 #b8d8ff);
            border-color: #70b0e0;
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #c0d8ff, stop:1 #a8c8f0);
            border-color: #5090d0;
        }

        QPushButton:disabled {
            background: #e8e8e8;
            border-color: #c0c0c0;
            color: #a0a0a0;
        }

        /* Input Field Styling */
        QLineEdit, QTextEdit, QPlainTextEdit {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f8fcff);
            border: 2px solid #b8d0e8;
            border-radius: 6px;
            padding: 6px 10px;
            color: #1a3050;
            selection-background-color: #64a0f0;
        }

        QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
            border-color: #5090d0;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f0f8ff);
        }

        /* ComboBox Styling */
        QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f0f8ff, stop:1 #e0f0ff);
            border: 2px solid #b8d0e8;
            border-radius: 6px;
            padding: 6px 10px;
            color: #1e4678;
            min-width: 100px;
        }

        QComboBox:hover {
            border-color: #70b0e0;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #e8f2ff, stop:1 #d8eaff);
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: none;
            border: 6px solid transparent;
            border-top: 6px solid #4080c0;
            margin-right: 5px;
        }

        QComboBox QAbstractItemView {
            background: #ffffff;
            border: 2px solid #b8d0e8;
            border-radius: 6px;
            color: #1e4678;
            selection-background-color: #64a0f0;
        }

        /* Slider Styling */
        QSlider::groove:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #e0f0ff, stop:1 #d0e8ff);
            height: 8px;
            border-radius: 4px;
            border: 1px solid #c0d8f0;
        }

        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #ffffff, stop:1 #e8f2ff);
            border: 3px solid #5090d0;
            width: 18px;
            margin: -7px 0;
            border-radius: 11px;
        }

        QSlider::handle:horizontal:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #f0f8ff, stop:1 #d8eaff);
            border-color: #3070b0;
        }

        /* Progress Bar */
        QProgressBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f0f8ff, stop:1 #e8f2ff);
            border: 2px solid #c0d8f0;
            border-radius: 8px;
            text-align: center;
            color: #1e4678;
            font-weight: bold;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #64a0f0, stop:1 #4080c0);
            border-radius: 6px;
        }

        /* Tab Widget */
        QTabWidget::pane {
            border: 2px solid #c0d8f0;
            border-radius: 8px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f8fcff);
        }

        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f0f8ff, stop:1 #e0f0ff);
            border: 2px solid #c0d8f0;
            border-bottom: none;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            padding: 8px 16px;
            margin-right: 2px;
            color: #2e5080;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f0f8ff);
            border-color: #5090d0;
            color: #1e4678;
            font-weight: bold;
        }

        QTabBar::tab:hover:!selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #e8f2ff, stop:1 #d8eaff);
            border-color: #90c0e0;
        }

        /* ScrollBar Styling */
        QScrollBar:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #f0f8ff, stop:1 #e8f2ff);
            width: 16px;
            border-radius: 8px;
            border: 1px solid #d0e0f0;
        }

        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #b8d0e8, stop:1 #a0c8f0);
            border-radius: 6px;
            min-height: 30px;
            margin: 2px;
            border: 1px solid #90b8e0;
        }

        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #a8c8e8, stop:1 #90b8e0);
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        /* Menu Bar */
        QMenuBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f0f8ff);
            border-bottom: 2px solid #c0d8f0;
            color: #1e4678;
        }

        QMenuBar::item {
            background: transparent;
            padding: 8px 12px;
            border-radius: 4px;
        }

        QMenuBar::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #e8f2ff, stop:1 #d0e8ff);
            color: #1e4678;
        }

        /* Menu */
        QMenu {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f8fcff);
            border: 2px solid #c0d8f0;
            border-radius: 8px;
            color: #1e4678;
            padding: 4px;
        }

        QMenu::item {
            background: transparent;
            padding: 8px 20px;
            border-radius: 4px;
        }

        QMenu::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #e8f2ff, stop:1 #d0e8ff);
            color: #1e4678;
        }

        /* Status Bar */
        QStatusBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f8fcff, stop:1 #f0f8ff);
            border-top: 2px solid #c0d8f0;
            color: #2e5080;
        }

        /* Group Box */
        QGroupBox {
            font-weight: bold;
            color: #1e4678;
            border: 2px solid #b8d0e8;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f8fcff);
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 8px 0 8px;
            color: #3080c0;
        }

        /* List Widget */
        QListWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #ffffff, stop:1 #f8fcff);
            border: 2px solid #b8d0e8;
            border-radius: 6px;
            color: #1a3050;
        }

        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #e0f0ff;
        }

        QListWidget::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #d8eaff, stop:1 #c0d8ff);
            color: #1e4678;
        }

        QListWidget::item:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #f0f8ff, stop:1 #e8f2ff);
        }

        /* Table Widget */
        QTableWidget {
            background: #ffffff;
            alternate-background-color: #f8fcff;
            gridline-color: #d0e8ff;
            border: 2px solid #b8d0e8;
            border-radius: 6px;
            color: #1a3050;
        }

        QTableWidget::item:selected {
            background: #64a0f0;
            color: #ffffff;
        }

        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #e8f2ff, stop:1 #d0e8ff);
            border: 1px solid #b8d0e8;
            padding: 8px;
            color: #1e4678;
            font-weight: bold;
        }
    )");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Video Dataset Preparation Tool");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Dataset Tools");

    // Set dark theme
    app.setStyle(QStyleFactory::create("Fusion"));

    setModernLightBlueTheme(app);

    qApp->setStyleSheet(
        "QToolTip {"
        "  background-color: #222; "
        "  color: white; "
        "  border: 1px solid white; "
        "  padding: 2px;"          // smaller padding
        "  font: 9pt 'Segoe UI';"  // smaller font
        "}"
        );

    MainWindow window;
    window.show();

    return app.exec();
}
