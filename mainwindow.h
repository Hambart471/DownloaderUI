// mainwindow.h
// ================================================
// Declaration of MainWindow: the central GUI class.
// It hosts a URL input, an “Add” button, a downloads table,
// and a “Start All” button, and manages DownloadWorker threads.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>     // Dynamic array to hold worker pointers
#include <QMap>        // Map from worker ID → table row index
#include "downloadworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window and its UI.
     * @param parent Optional parent widget.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructor: waits for any running threads and cleans up.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Queues a new download when “Add” clicked.
     */
    void on_buttonAdd_clicked();

    /**
     * @brief Starts all queued downloads when “Start All” clicked.
     */
    void on_buttonStart_clicked();

    /**
     * @brief Updates the progress cell for worker ID.
     * @param id      Worker ID (maps to a table row).
     * @param percent Percentage or -1 if unknown.
     */
    void handleProgress(int id, int percent);

    /**
     * @brief Marks a download row as completed.
     * @param id       Worker ID for row lookup.
     * @param filename Local filename for display.
     */
    void handleFinished(int id, const QString& filename);

private:
    Ui::MainWindow*        ui;        ///< Auto‑generated UI form
    QVector<DownloadWorker*> m_workers; ///< Keeps worker objects alive
    QMap<int,int>           m_rowMap;  ///< Maps worker ID → row in table
    int                     m_nextId;  ///< Next unique ID (starts at 1)
};

#endif // MAINWINDOW_H
