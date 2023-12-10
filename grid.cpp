#include "grid.h"
#include <algorithm>
#include <ctime>
#include <QDebug>

Grid::Grid(int width, int height) : gridWidth(width), gridHeight(height)
{
    cells.resize(height, std::vector<bool>(width, false));
    buffer.resize(height, std::vector<bool>(width, false));
    cellAge.resize(height, std::vector<int>(width, 0));
    cellColor.resize(height, std::vector<QColor>(width, Qt::white)); // Initialize cell colors to white
    srand(static_cast<unsigned>(time(nullptr)));  // Seed the random number generator
}

void Grid::randomizeGrid(int startingCells)
{
    reset();  // Clear the grid first
    while (startingCells > 0) {
        int x = rand() % gridWidth;
        int y = rand() % gridHeight;
        if (!isCellAlive(x, y)) {
            setCellStatus(x, y, true);
            --startingCells;
        }
    }
}

void Grid::nextGeneration()
{
    buffer = cells;

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            int aliveNeighbors = countAliveNeighbors(x, y);
            if (isCellAlive(x, y)) {
                // Any live cell with two or three live neighbors survives.
                buffer[y][x] = aliveNeighbors == 2 || aliveNeighbors == 3;
                // Update cell age if it survives
                if (buffer[y][x]) {
                    cellAge[y][x]++;
                    if (cellAge[y][x] >= 200) {
                        // Cell has survived for 2 seconds or more, set its color to green
                        cellColor[y][x] = Qt::green;
                    }
                }
            } else {
                // Any dead cell with three live neighbors becomes a live cell.
                buffer[y][x] = aliveNeighbors == 3;
                // Reset cell age if it becomes alive
                if (buffer[y][x]) {
                    cellAge[y][x] = 0;
                }
                // Set color to black for live cells
                cellColor[y][x] = Qt::black;
            }
        }
    }

    cells.swap(buffer);
}

bool Grid::isCellAlive(int x, int y) const
{
    return cells[y][x];
}

bool Grid::isCellDying(int x, int y) const
{
    int aliveNeighbors = countAliveNeighbors(x, y);
    if (isCellAlive(x, y)) {
        // Any live cell with two or three live neighbors survives.
        return !(aliveNeighbors == 2 || aliveNeighbors == 3);
    } else {
        // Any dead cell with three live neighbors becomes a live cell.
        return !(aliveNeighbors == 3);
    }
}

int Grid::getCellAge(int x, int y) const
{
    return cellAge[y][x];
}

QColor Grid::getCellColor(int x, int y) const
{
    return cellColor[y][x];
}

void Grid::setCellStatus(int x, int y, bool alive)
{
    cells[y][x] = alive;
}

void Grid::reset()
{
    for (auto &row : cells) {
        std::fill(row.begin(), row.end(), false);
    }
    for (auto &row : cellColor) {
        std::fill(row.begin(), row.end(), Qt::white);
    }
}

void Grid::toggleCell(int x, int y)
{
    // Check for valid coordinates
    if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
        cells[y][x] = !cells[y][x];
    }
}

int Grid::countAliveNeighbors(int x, int y) const
{
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // Skip the cell itself
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                count += cells[ny][nx] ? 1 : 0;
            }
        }
    }
    return count;
}
