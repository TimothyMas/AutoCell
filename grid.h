#ifndef GRID_H
#define GRID_H

#include <vector>
#include <QColor>

class Grid
{
public:
    Grid(int width, int height);
    void randomizeGrid(int startingCells);
    void nextGeneration();
    bool isCellAlive(int x, int y) const;
    bool isCellDying(int x, int y) const;
    int getCellAge(int x, int y) const;
    QColor getCellColor(int x, int y) const;
    void setCellStatus(int x, int y, bool alive);
    void reset();
    void toggleCell(int x, int y);
    int getWidth() const { return gridWidth; }
    int getHeight() const { return gridHeight; }

private:
    int gridWidth;
    int gridHeight;
    std::vector<std::vector<bool>> cells;
    std::vector<std::vector<bool>> buffer;
    std::vector<std::vector<int>> cellAge;
    std::vector<std::vector<QColor>> cellColor;

    // Declarations for tracking cell positions
    std::vector<std::vector<int>> startingX;
    std::vector<std::vector<int>> startingY;

    int countAliveNeighbors(int x, int y) const;
    bool isCellDoingCircles(int x, int y) const;
};

#endif // GRID_H
