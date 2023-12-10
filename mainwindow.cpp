#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <ctime>
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    grid(nullptr),
    timer(new QTimer(this)),
    simulationRunning(false)
{
    ui->setupUi(this);

    connect(ui->StartButton, &QPushButton::clicked, this, &MainWindow::on_StartButton_clicked);
    connect(ui->StopButton, &QPushButton::clicked, this, &MainWindow::on_StopButton_clicked);
    connect(ui->ResetButton, &QPushButton::clicked, this, &MainWindow::on_ResetButton_clicked);
    connect(ui->setStartingCells, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::initializeGrid);
    connect(ui->setUpdateSpeed, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_UpdateSpeedChanged);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateGridDisplay);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Change the window title to "AutoCell" or your desired title
    QMainWindow::setWindowTitle("AutoCell");

    // Create a QGraphicsScene and set it to the view
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Set up the view to scale with the UI
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setOptimizationFlag(QGraphicsView::IndirectPainting, true);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->graphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->graphicsView->setFrameStyle(QFrame::NoFrame);

    // Set the initial size and center the view
    ui->graphicsView->setMinimumSize(10, 10);
    ui->graphicsView->setAlignment(Qt::AlignCenter);
    ui->graphicsView->setInteractive(true);
}

MainWindow::~MainWindow()
{
    delete timer;
    delete grid;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (ui->graphicsView->scene() && !ui->graphicsView->scene()->items().isEmpty()) {
        // Adjust the view's scene rectangle when the main window is resized
        ui->graphicsView->fitInView(ui->graphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}


void MainWindow::paintEvent(QPaintEvent * /* event */) {
    // Rendering grid in QGraphicsView
    if (grid != nullptr) {
        scene->clear();
        for (int x = 0; x < grid->getWidth(); ++x) {
            for (int y = 0; y < grid->getHeight(); ++y) {
                bool isCellAlive = grid->isCellAlive(x, y);
                bool isDying = grid->isCellDying(x, y);
                int cellAge = grid->getCellAge(x, y);
                QColor cellColor = grid->getCellColor(x, y); // Get the cell color

                QRectF cellRect(x * cellSize, y * cellSize, cellSize, cellSize);
                QGraphicsRectItem *cellItem = new QGraphicsRectItem(cellRect);

                if (isCellAlive && isDying) {
                    // Cell is dying, set its color to red
                    cellItem->setBrush(Qt::red);
                } else if (isCellAlive) {
                    // Cell is alive, set its color from the grid
                    cellItem->setBrush(cellColor);
                } else {
                    // Cell is dead, set its color to white (or any other color)
                    cellItem->setBrush(Qt::white);
                }

                scene->addItem(cellItem);
            }
        }
    }
}

void MainWindow::updateGridDisplay()
{
    if (simulationRunning && grid != nullptr) {
        grid->nextGeneration();
        this->update(); // This will trigger a call to paintEvent
    }
}

void MainWindow::on_StartButton_clicked()
{
    if (!simulationRunning && grid != nullptr) {
        simulationRunning = true;
        ui->StartButton->setEnabled(false);
        ui->StopButton->setEnabled(true);
        ui->ResetButton->setEnabled(false);
        timer->start(updateSpeed); // Start the timer for continuous updates
    }
}

void MainWindow::on_StopButton_clicked()
{
    if (simulationRunning) {
        timer->stop();
        simulationRunning = false;
        ui->StartButton->setEnabled(true);
        ui->StopButton->setEnabled(false);
        ui->ResetButton->setEnabled(true);
    }
}

void MainWindow::on_ResetButton_clicked()
{
    if (grid != nullptr) {
        grid->reset();
        this->update();
        ui->StartButton->setEnabled(true);
        ui->StopButton->setEnabled(false);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !simulationRunning && grid != nullptr) {
        int x = event->pos().x() / cellSize;
        int y = event->pos().y() / cellSize;
        if (x >= 0 && x < grid->getWidth() && y >= 0 && y < grid->getHeight()) {
            grid->toggleCell(x, y);
            this->update();
        }
    }
}

void MainWindow::initializeGrid()
{
    int width = ui->setGridWidth->value();
    int height = ui->setGridHeight->value();
    int startingCells = ui->setStartingCells->value();

    if (grid != nullptr) {
        delete grid;
    }

    grid = new Grid(width, height);
    grid->randomizeGrid(startingCells);

    ui->StartButton->setEnabled(true);
    ui->StopButton->setEnabled(false);
    ui->ResetButton->setEnabled(true);

    simulationRunning = false;

    this->update();
}

void MainWindow::on_setStartingCells_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    initializeGrid();
}

void MainWindow::on_setGridHeight_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    initializeGrid();
}

void MainWindow::on_UpdateSpeedChanged(int value)
{
    updateSpeed = value; // Set the new update speed
    if (timer != nullptr && simulationRunning) {
        timer->start(updateSpeed); // Update the timer interval if the simulation is running
    }
}

void MainWindow::onMainWindowResized()
{
    if (scene != nullptr) {
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}
