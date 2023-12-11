#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <vector>
#include "grid.h"

namespace Ui

{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:

    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void on_ResetButton_clicked();
    void on_setStartingCells_valueChanged(int arg1);
    void on_setGridHeight_textChanged(const QString &arg1);
    void on_UpdateSpeedChanged(int value);
    void on_Structure_comboBox_currentIndexChanged(int index);

private:

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Grid *grid;
    QTimer *timer;

    bool simulationRunning;
    bool structureSelected;

    int updateSpeed;
    int cellSize;

    std::vector<std::vector<bool>> selectedStructure;

    void initializeGrid();
    void updateGridDisplay();
    void onMainWindowResized();
};

#endif // MAINWINDOW_H
