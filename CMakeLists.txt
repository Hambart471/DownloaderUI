# CMakeLists.txt
# ================================================
# Top‑level CMake configuration for DownloaderUI.
# It specifies minimum CMake version, project name,
# C++ standard, finds Qt6, declares our executable and
# links against Qt Widgets and Qt Network.

cmake_minimum_required(VERSION 3.16)
project(DownloaderUI LANGUAGES CXX)

# ----------------------------------------------------------------------------
# 1) Set C++ standard and enable Qt’s automoc/uic/rcc features
#    - C++17 for modern language features.
#    - AUTOMOC: automatically run moc on Q_OBJECT classes.
#    - AUTOUIC: automatically compile .ui files.
#    - AUTORCC: automatically compile .qrc resource files (none here).
# ----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ----------------------------------------------------------------------------
# 2) Locate required Qt6 modules:
#    - Widgets: for GUI components (windows, buttons, layouts).
#    - Network: for QNetworkAccessManager & QNetworkReply.
# ----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

# ----------------------------------------------------------------------------
# 3) Declare the DownloaderUI executable and list all source/header/ui files.
#    - Headers listed here will also show up in IDE project views.
#    - List .ui so CMake knows to pass it through uic.
# ----------------------------------------------------------------------------
add_executable(DownloaderUI
    main.cpp             # Entry point: QApplication + show MainWindow
    mainwindow.h         # Declaration of our MainWindow class
    mainwindow.cpp       # Definition of MainWindow behavior
    mainwindow.ui        # Qt Designer form: layout of widgets
    downloadworker.h     # Declaration of DownloadWorker thread class
    downloadworker.cpp
    .gitignore
)

# ----------------------------------------------------------------------------
# 4) Link our executable against Qt6 Widgets and Network libraries.
#    - PRIVATE: only this target needs these libraries.
# ----------------------------------------------------------------------------
target_link_libraries(DownloaderUI
    PRIVATE Qt6::Widgets Qt6::Network
)
