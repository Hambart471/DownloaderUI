// mainwindow.cpp
// ================================================
// Implements MainWindow: wires up UI actions, manages queue of
// DownloadWorker threads, updates QTableWidget with progress and completion.

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidgetItem>  // For inserting/updating table cells
#include <QMessageBox>       // For warning dialog when URL is empty

// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nextId(1)           // Initialize ID counter at 1
{
    // Build widgets/layout from mainwindow.ui
    ui->setupUi(this);

    // Note: Qt’s auto‑connection matches on_buttonAdd_clicked()
    // and on_buttonStart_clicked() based on objectName + signal.
}

// ----------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------
MainWindow::~MainWindow()
{
    // Wait for each worker thread to finish and delete it
    for (DownloadWorker* worker : m_workers) {
        if (worker->isRunning()) {
            worker->wait();
        }
        delete worker;
    }
    // Delete UI form and its child widgets
    delete ui;
}

// ----------------------------------------------------------------
// Slot: Add button clicked
// ----------------------------------------------------------------
void MainWindow::on_buttonAdd_clicked()
{
    // 1) Read and trim the URL the user entered
    QString url = ui->lineEditUrl->text().trimmed();

    // 2) If empty, warn and abort
    if (url.isEmpty()) {
        QMessageBox::warning(this, "DownloaderUI",
                             "Please enter a URL to download.");
        return;
    }

    // 3) Assign a new unique ID and new row index
    int id = m_nextId++;
    int row = ui->tableDownloads->rowCount();
    ui->tableDownloads->insertRow(row);

    // 4) Populate the table: column 0=URL, column 1="0%"
    ui->tableDownloads->setItem(row, 0, new QTableWidgetItem(url));
    ui->tableDownloads->setItem(row, 1, new QTableWidgetItem("0%"));

    // 5) Create and configure the DownloadWorker
    DownloadWorker* worker = new DownloadWorker(url, id, this);
    connect(worker, &DownloadWorker::progress,
            this, &MainWindow::handleProgress);
    connect(worker, &DownloadWorker::finished,
            this, &MainWindow::handleFinished);

    // 6) Store worker pointer and map ID → row
    m_workers.append(worker);
    m_rowMap[id] = row;

    // 7) Clear the input for convenience
    ui->lineEditUrl->clear();
}

// ----------------------------------------------------------------
// Slot: Start All button clicked
// ----------------------------------------------------------------
void MainWindow::on_buttonStart_clicked()
{
    // Iterate over all queued workers and start their threads
    for (DownloadWorker* worker : m_workers) {
        worker->start();  // triggers DownloadWorker::run()
    }
}

// ----------------------------------------------------------------
// Slot: progress update from worker
// ----------------------------------------------------------------
void MainWindow::handleProgress(int id, int percent)
{
    // 1) Lookup the table row for this worker ID
    if (!m_rowMap.contains(id))
        return;
    int row = m_rowMap[id];

    // 2) Retrieve the QTableWidgetItem for the Progress column
    QTableWidgetItem* item = ui->tableDownloads->item(row, 1);
    if (!item)
        return;

    // 3) Display either numeric percent or "Downloading…" if unknown
    if (percent < 0) {
        item->setText("Downloading…");
    } else {
        item->setText(QString::number(percent) + "%");
    }
}

// ----------------------------------------------------------------
// Slot: worker finished downloading
// ----------------------------------------------------------------
void MainWindow::handleFinished(int id, const QString& filename)
{
    // 1) Lookup row
    if (!m_rowMap.contains(id))
        return;
    int row = m_rowMap[id];

    // 2) Update File column to show the saved filename
    QTableWidgetItem* itemFile = ui->tableDownloads->item(row, 0);
    if (itemFile) {
        itemFile->setText(filename);
    }

    // 3) Mark Progress column as Completed
    QTableWidgetItem* itemProg = ui->tableDownloads->item(row, 1);
    if (itemProg) {
        itemProg->setText("Completed");
    }
}
