// downloadworker.h
// ================================================
// Declares DownloadWorker, a QThread subclass that downloads
// exactly one file from a URL in its own thread. Progress and
// completion are reported via Qt signals.

#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include <QThread>               // Base class for threads
#include <QString>               // Qt string class
#include <QNetworkAccessManager> // Manages network requests
#include <QNetworkReply>         // Handles asynchronous replies

/**
 * @class DownloadWorker
 * @brief Downloads a single file in a separate thread.
 *
 * Usage:
 *   DownloadWorker* w = new DownloadWorker("http://...", 1, this);
 *   connect(w, &DownloadWorker::progress, this, &SomeClass::onProgress);
 *   connect(w, &DownloadWorker::finished, this, &SomeClass::onFinished);
 *   w->start();
 *
 * Signals:
 *   - progress(id,percent): emitted repeatedly as download proceeds.
 *   - finished(id,filename): emitted once when file is saved to disk.
 */
class DownloadWorker : public QThread
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the worker.
     * @param url    The URL of the file to download.
     * @param id     A unique ID for this worker (for UI mapping).
     * @param parent Optional QObject parent for memory management.
     */
    explicit DownloadWorker(const QString& url, int id, QObject* parent = nullptr);

    /**
     * @brief Destructor: waits for thread exit if still running.
     */
    ~DownloadWorker();

signals:
    /**
     * @brief Reports download progress.
     * @param id      Worker ID.
     * @param percent Percentage complete (0–100) or -1 if unknown length.
     */
    void progress(int id, int percent);

    /**
     * @brief Reports that download has finished successfully.
     * @param id       Worker ID.
     * @param filename Name of the saved file (derived from URL).
     */
    void finished(int id, const QString& filename);

protected:
    /**
     * @brief QThread entry point. Sets up network request and event loop.
     */
    void run() override;

private slots:
    /**
     * @brief Handles bytesReceived/bytesTotal updates from QNetworkReply.
     */
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    /**
     * @brief Handles the finished() signal from QNetworkReply.
     */
    void onFinished();

private:
    QString                 m_url;     ///< URL string to download
    int                     m_id;      ///< Unique worker identifier
    QNetworkAccessManager*  m_manager; ///< Network manager (lives in this thread)
    QNetworkReply*          m_reply;   ///< Holds the async HTTP reply
    QByteArray              m_data;    ///< Buffer for downloaded bytes

    /**
     * @brief Extract the filename portion from a URL.
     * @param url The full URL.
     * @return Filename (text after last '/').
     */
    QString fileNameFromUrl(const QString& url) const;
};

#endif // DOWNLOADWORKER_H
