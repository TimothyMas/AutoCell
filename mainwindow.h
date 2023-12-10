#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include "grid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateGridDisplay();
    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void on_ResetButton_clicked();
    void initializeGrid();

    void on_setStartingCells_valueChanged(int arg1);
    void on_setGridHeight_textChanged(const QString &arg1);
    void on_UpdateSpeedChanged(int value);
    void onMainWindowResized();

private:
    Ui::MainWindow *ui;
    Grid *grid;
    QTimer *timer;
    bool simulationRunning;
    QGraphicsScene *scene;
    static constexpr int cellSize = 14;
    int updateSpeed = 300; // Initial update speed
};

#endif // MAINWINDOW_H
