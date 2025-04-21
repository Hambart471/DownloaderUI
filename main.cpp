// main.cpp
// ================================================
// Entry point for the DownloaderUI application, now with
// a dark charcoal background + neon‑green accent theme.

#include <QApplication>    // Core application class for GUI apps
#include "mainwindow.h"    // Declaration of our MainWindow

int main(int argc, char* argv[])
{
    // 1) Instantiate QApplication: must come before any QWidget is created.
    QApplication app(argc, argv);

    // ----------------------------------------------------------------------------
    // 2) Install a Qt Style‑Sheet for the dark + neon‑green theme.
    //    We use a raw string literal (R"(... )") for multiline clarity.
    //    Adjust hex values (#121212, #00FF00, #004400) to tweak appearance.
    // ----------------------------------------------------------------------------
    app.setStyleSheet(R"(
        /* Global defaults for all widgets */
        QWidget {
            background-color: #121212;             /* dark charcoal */
            color:            #00FF00;             /* neon green text */
            selection-background-color: #004400;    /* darker green when selected */
            selection-color:            #00FF00;
        }

        /* QPushButton: neon border, transparent fill */
        QPushButton {
            background-color: transparent;
            border:           1px solid #00FF00;
            border-radius:    4px;
            padding:          6px 12px;
            color:            #00FF00;
        }
        QPushButton:hover {
            background-color: #004400; /* subtle glow on hover */
        }
        QPushButton:pressed {
            background-color: #002200; /* darker when pressed */
        }

        /* QLineEdit: dark inset with neon border */
        QLineEdit {
            background-color: #1E1E1E;
            border:           1px solid #00FF00;
            border-radius:    3px;
            padding:          4px;
            color:            #00FF00;
        }

        /* QTableWidget: dark grid, neon headers */
        QTableWidget {
            background-color: #1E1E1E;
            gridline-color:   #2E2E2E;
        }
        QHeaderView::section {
            background-color: #2E2E2E;
            color:            #00FF00;
            border:           1px solid #00FF00;
            padding:          4px;
        }
        QTableWidget::item {
            color: #00FF00;
        }
        QTableWidget::item:selected {
            background-color: #004400;
            color:            #00FF00;
        }

        /* QProgressBar: neon frame & chunk */
        QProgressBar {
            background-color: #1E1E1E;
            border:           1px solid #00FF00;
            border-radius:    4px;
            text-align:       center;
            color:            #00FF00;
            min-height:       20px;
        }
        QProgressBar::chunk {
            background-color: #00FF00;
            border-radius:    4px;
        }

        /* QScrollBar styling */
        QScrollBar:vertical, QScrollBar:horizontal {
            background: #121212;
            width:      12px;
            margin:     0;
        }
        QScrollBar::handle {
            background: #00FF00;
            min-height: 20px;
            border-radius: 6px;
        }
        QScrollBar::add-line, QScrollBar::sub-line,
        QScrollBar::add-page, QScrollBar::sub-page {
            background: none;
            height:     0;
        }
    )");

    // ----------------------------------------------------------------------------
    // 3) Create and show the main window
    // ----------------------------------------------------------------------------
    MainWindow window;
    window.show();

    // ----------------------------------------------------------------------------
    // 4) Enter the Qt event loop
    // ----------------------------------------------------------------------------
    return app.exec();
}
