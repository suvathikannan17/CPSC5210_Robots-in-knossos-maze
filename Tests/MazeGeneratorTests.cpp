#include <gtest/gtest.h>
#include "Maze.h"

#include <queue>
#include <utility>
#include <vector>
#include <sstream>
#include "GameIO.h"
#include "Consts.h"
#include <iostream>
#include <string>

using namespace std;

class MazeGeneratorTest : public testing::TestWithParam<int> {};

// Test 1: Check that the maze has the requested dimensions.
TEST_P(MazeGeneratorTest, CorrectDimensions) {
    int size = GetParam();
    Maze maze(size, size, 0);

    EXPECT_EQ(maze.getRows(), size);
    EXPECT_EQ(maze.getColumns(), size);
}

// Test 2: Every generated cell should be a wall or open space.
TEST_P(MazeGeneratorTest, OnlyValidSymbols) {
    int size = GetParam();
    Maze maze(size, size, 0);

    for (int r = 0; r < maze.getRows(); r++) {
        for (int c = 0; c < maze.getColumns(); c++) {
            char symbol = maze[r][c]->getSymbol();

            EXPECT_TRUE(symbol == '#' || symbol == ' ');
        }
    }
}

// 3: The starting position is open/empty.
TEST_P(MazeGeneratorTest, StartingPositionIsOpen) {
    int size = GetParam();
    Maze maze(size, size, 0);

    EXPECT_EQ(maze[1][1]->getSymbol(), ' ');
}

// 4: The outside boundaries should be walls.
TEST_P(MazeGeneratorTest, BoundariesAreWalls) {
    int size = GetParam();
    Maze maze(size, size, 0);

    for (int col = 0; col < maze.getColumns(); col++) {
        EXPECT_EQ(maze[0][col]->getSymbol(), '#');
        EXPECT_EQ(maze[maze.getRows() - 1][col]->getSymbol(), '#');
    }

    for (int row = 0; row < maze.getRows(); row++) {
        EXPECT_EQ(maze[row][0]->getSymbol(), '#');
        EXPECT_EQ(maze[row][maze.getColumns() - 1]->getSymbol(), '#');
    }
}

// 5: The maze should contain some open spaces.
TEST_P(MazeGeneratorTest, ContainsOpenPaths) {
    int size = GetParam();
    Maze maze(size, size, 0);
    int openSpaces = 0;

    for (int row = 0; row < maze.getRows(); row++) {
        for (int col = 0; col < maze.getColumns(); col++) {
            if (maze[row][col]->getSymbol() == ' ') {
                openSpaces++;
            }
        }
    }

    EXPECT_GT(openSpaces, 1); // more or equal to 1 empty space
}

// 6: All open cells should be reachable from the start.
TEST_P(MazeGeneratorTest, AllOpenCellsAreConnected) {
    int size = GetParam();
    Maze maze(size, size, 0);

    int rows = maze.getRows();
    int columns = maze.getColumns();

    // will track visited cells
    vector<vector<bool>> visited;

    for (int row = 0; row < rows; row++) {
        vector<bool> newRow;

        for (int column = 0; column < columns; column++) {
            newRow.push_back(false);
        }

        visited.push_back(newRow);
    }

    // Count the total number of open spaces in the maze
    int totalOpenSpaces = 0;

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            if (maze[row][column]->getSymbol() == ' ') {
                totalOpenSpaces++;
            }
        }
    }

    // Start searching from position (1, 1) (BFS)
    queue<pair<int, int>> positionsToVisit;

    positionsToVisit.push({1, 1});
    visited[1][1] = true;

    int reachableSpaces = 0;

    // Movement directions: up, down, left, right
    int rowChanges[] = {-1, 1, 0, 0};
    int columnChanges[] = {0, 0, -1, 1};

    // Keep searching until there are no more positions to visit
    while (!positionsToVisit.empty()) {

        pair<int, int> currentPosition = positionsToVisit.front();
        positionsToVisit.pop();

        reachableSpaces++;

        // Check all four neighboring cells
        for (int direction = 0; direction < 4; direction++) {

            int newRow = currentPosition.first + rowChanges[direction];
            int newColumn = currentPosition.second + columnChanges[direction];

            // Make sure the position is inside the maze
            bool insideMaze =
                newRow >= 0 && newRow < rows &&
                newColumn >= 0 && newColumn < columns;

            if (insideMaze) {

                bool alreadyVisited = visited[newRow][newColumn];
                bool isOpenSpace =
                    maze[newRow][newColumn]->getSymbol() == ' ';

                // Visit the cell if it is open and has not been visited
                if (!alreadyVisited && isOpenSpace) {
                    visited[newRow][newColumn] = true;
                    positionsToVisit.push({newRow, newColumn});
                }
            }
        }
    }

    // All open spaces should have been reachable
    EXPECT_EQ(reachableSpaces, totalOpenSpaces);
}

// Run each test with different maze sizes
INSTANTIATE_TEST_SUITE_P(
    MazeSizes,
    MazeGeneratorTest,
    testing::Values(15, 16, 21)
);
