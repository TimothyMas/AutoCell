#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <vector>
#include <QColor>

class Grid : public QObject
{
    Q_OBJECT

public:

    Grid(int width, int height);
    ~Grid();

    QColor getCellColor(int x, int y) const;

    void randomizeGrid(int startingCells);
    void nextGeneration();
    void setCellStatus(int x, int y, bool alive);
    void reset();
    void toggleCell(int x, int y);
    void placeStructure(const std::vector<std::vector<bool>>& structure, int x, int y);
    void setCellSize(int size);

    bool isCellAlive(int x, int y) const;
    bool isCellDying(int x, int y) const;

    int getCellAge(int x, int y) const;
    int getWidth() const { return gridWidth; }
    int getHeight() const { return gridHeight; }
    int cellSize;

signals:

    void gridUpdated();

private:

    int gridWidth;
    int gridHeight;
    int countAliveNeighbors(int x, int y) const;

    std::vector<std::vector<bool>> cells;
    std::vector<std::vector<bool>> buffer;
    std::vector<std::vector<int>> cellAge;
    std::vector<std::vector<QColor>> cellColor;
};

#endif // GRID_H
