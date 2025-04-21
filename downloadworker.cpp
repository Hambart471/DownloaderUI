// downloadworker.cpp
// ================================================
// Implements DownloadWorker: creates a QNetworkAccessManager,
// issues a GET request, tracks progress, saves to disk, and
// emits signals for the UI.

#include "downloadworker.h"  // Class declaration + Qt includes

// If DOWNLOAD_DIR wasn’t set by CMake, default to "downloads"
#ifndef DOWNLOAD_DIR
#define DOWNLOAD_DIR "downloads"
#endif

#include <QFile>  // For writing data to disk
#include <QDir>   // For creating/checking directories
#include <QUrl>   // For parsing URL into filename

// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
DownloadWorker::DownloadWorker(const QString& url, int id, QObject* parent)
    : QThread(parent)   // Initialize base QThread with given parent
    , m_url(url)        // Store URL to download
    , m_id(id)          // Store unique ID
    , m_manager(nullptr) // Manager created later in run()
    , m_reply(nullptr)   // Reply set when request issued
{
    // No other work here; everything happens in run().
}

// ----------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------
DownloadWorker::~DownloadWorker()
{
    // If thread still running, wait here for run() to finish.
    if (isRunning()) {
        wait();  // Blocks until exec() loop exits and run() returns
    }
    // No need to delete m_manager/m_reply here: they’re deleted in run().
}

// ----------------------------------------------------------------
// run()
// ----------------------------------------------------------------
void DownloadWorker::run()
{
    // 1) Create network manager in this thread context
    m_manager = new QNetworkAccessManager();

    // 2) Prepare the GET request for our URL
    QNetworkRequest request{ QUrl(m_url) };

    // 3) Issue the asynchronous GET; get a QNetworkReply*
    m_reply = m_manager->get(request);

    // 4) Connect progress notifications
    connect(m_reply, &QNetworkReply::downloadProgress,
            this, &DownloadWorker::onDownloadProgress);

    // 5) Connect finished notification
    connect(m_reply, &QNetworkReply::finished,
            this, &DownloadWorker::onFinished);

    // 6) Enter a local event loop to process reply signals in this thread
    exec();

    // 7) Cleanup after quit() is called in onFinished()
    delete m_reply;
    delete m_manager;
}

// ----------------------------------------------------------------
// onDownloadProgress()
// ----------------------------------------------------------------
void DownloadWorker::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        // Calculate percentage
        int percent = static_cast<int>((bytesReceived * 100) / bytesTotal);
        emit progress(m_id, percent);
    } else {
        // Unknown total length
        emit progress(m_id, -1);
    }
}

// ----------------------------------------------------------------
// onFinished()
// ----------------------------------------------------------------
void DownloadWorker::onFinished()
{
    // 1) Read all buffered data
    m_data = m_reply->readAll();

    // 2) Ensure download directory exists
    QDir dir;
    if (!dir.exists(DOWNLOAD_DIR)) {
        dir.mkpath(DOWNLOAD_DIR);
    }

    // 3) Determine local filename and full path
    QString filename = fileNameFromUrl(m_url);
    QString filepath = QString("%1/%2").arg(DOWNLOAD_DIR, filename);

    // 4) Save data to file
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(m_data);
        file.close();
    }
    // (Error handling could be added here)

    // 5) Notify UI thread that download is complete
    emit finished(m_id, filename);

    // 6) Quit the event loop started in run()
    quit();
}

// ----------------------------------------------------------------
// fileNameFromUrl()
// ----------------------------------------------------------------
QString DownloadWorker::fileNameFromUrl(const QString& url) const
{
    QUrl qurl(url);
    return qurl.fileName();  // Extracts text after final '/'
}
