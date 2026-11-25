
#include "MazeGenerator.h"

#include "Consts.h"
#include "Field.h"

#include <algorithm>
#include <cstdlib>
#include <utility>
#include <vector>
#include <random>

using std::vector;
using std::pair;

void MazeGenerator::carveMazeDFS(int x, int y, int rows, int cols, Maze& maze, vector<pair<int, int>>& directions) {
    std::random_device rd;
    std::mt19937 generator(rd());

    // Use a local copy so recursive calls do not change this order
    auto localDirections = directions;
    std::shuffle(localDirections.begin(), localDirections.end(), generator);

    for (size_t i = 0; i < localDirections.size(); ++i) {
        int dx = localDirections[i].first;
        int dy = localDirections[i].second;
        int nextX = x + dx;
        int nextY = y + dy;
        int betweenX = x + dx / 2;
        int betweenY = y + dy / 2;

        // Check if next position is within bounds and is a wall.
        // Carve a path by removing the wall between positions.
        if (nextX > 0 && nextX < rows - 1 && nextY > 0 && nextY < cols - 1
            && maze[nextX][nextY]->getSymbol() == '#') {
            maze[betweenX][betweenY]->setSymbol(' ');
            maze[nextX][nextY]->setSymbol(' ');
            carveMazeDFS(nextX, nextY, rows, cols, maze, directions);
        }
    }
}

void MazeGenerator::generate(Maze& maze) {
    int rows = maze.getRows();
    int columns = maze.getColumns();

    // Initialize the maze with walls
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            maze[i][j] = new Field(pair<int, int>(i, j), '#');
        }
    }

    // Ensure odd dimensions for maze generation
    int oddRows = rows;
    int oddColumns = columns;

    if (rows % 2 == 0) --oddRows;
    if (columns % 2 == 0) --oddColumns;

    maze[1][1]->setSymbol(' ');

    vector<pair<int, int>> directions = {
        {-2, 0},
        {2, 0},
        {0, -2},
        {0, 2}
    };

    carveMazeDFS(1, 1, oddRows, oddColumns, maze, directions);

    // Handle extra row and column for even dimensions
    // Handle extra row for even dimensions
if (rows != oddRows) {
    for (int j = 1; j < columns - 1; ++j) {
        // Only open a cell if it connects to an existing path
        if (maze[rows - 3][j]->getSymbol() == ' ' &&
            rand() % 2 == 0) {
            maze[rows - 2][j]->setSymbol(' ');
        }
    }
}

// Handle extra column for even dimensions
if (columns != oddColumns) {
    for (int i = 1; i < rows - 1; ++i) {
        // Only open a cell if it connects to an existing path
        if (maze[i][columns - 3]->getSymbol() == ' ' &&
            rand() % 2 == 0) {
            maze[i][columns - 2]->setSymbol(' ');
        }
    }
}

    // Add extra paths to top third to create loops
    // and alternatives to avoid minotaur capture
    int extraPaths = static_cast<int>(
        (rows * columns) * EXTRA_PATHS_FRACTION
    );

    for (int n = 0; n < extraPaths; ++n) {
        int x = 1 + rand() % (int((rows - 2) * MINOTAUR_PLACEMENT_FRACTION));
        int y = 1 + rand() % (columns - 2);

        if (maze[x][y]->getSymbol() == '#') {
            if (maze[x-1][y]->getSymbol() == ' ' &&
                maze[x+1][y]->getSymbol() == ' ') {
                maze[x][y]->setSymbol(' ');
            }
            else if (maze[x][y-1]->getSymbol() == ' ' &&
                     maze[x][y+1]->getSymbol() == ' ') {
                maze[x][y]->setSymbol(' ');
            }
        }
    }
}