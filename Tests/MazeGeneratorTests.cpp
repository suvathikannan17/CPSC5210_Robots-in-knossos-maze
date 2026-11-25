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

// 7: Verify that the Maze class returns the correct row and column counts.
TEST(MazeTest, CorrectRowsAndColumns) {
    Maze maze(15, 20, 0);

    EXPECT_EQ(maze.getRows(), 15);
    EXPECT_EQ(maze.getColumns(), 20);
}

// 8: Verify that the const operator[] can access maze cells.
TEST(MazeTest, ConstIndexOperator) {
    Maze maze(15, 15, 0);

    const Maze& constMaze = maze;

    EXPECT_EQ(constMaze[0].size(), 15);
    EXPECT_NE(constMaze[0][0], nullptr);
}

// 9: Verify that printing the maze produces the correct
// number of rows and columns.
TEST(MazeTest, PrintMaze) {
    Maze maze(15, 20, 0);

    std::ostringstream output;
    output << maze;

    std::string result = output.str();

    std::istringstream input(result);
    std::string line;
    int rowCount = 0;

    while (std::getline(input, line)) {
        EXPECT_EQ(line.length(), 20);
        rowCount++;
    }

    EXPECT_EQ(rowCount, 15);
}

// 10: Verify that valid maze dimensions are accepted.
TEST(GameIOTest, ValidDimensions) {
    std::istringstream input("20 25\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int rows = 0;
    int columns = 0;

    bool result = GameIO::getValidDimensions(rows, columns);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(result);
    EXPECT_EQ(rows, 20);
    EXPECT_EQ(columns, 25);
}


//
// GameIO Unit Tests
//

// Helper function to simulate user input.
// Restores cin and cout after the method finishes.
void testDimensionsInput(
    const std::string& simulatedInput,
    bool expectedResult,
    int expectedRows,
    int expectedColumns)
{
    std::istringstream input(simulatedInput);
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int rows = 0;
    int columns = 0;

    bool result = GameIO::getValidDimensions(rows, columns);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_EQ(result, expectedResult);
    EXPECT_EQ(rows, expectedRows);
    EXPECT_EQ(columns, expectedColumns);
}

// Test 11: Verify the smallest accepted dimensions.
TEST(GameIOTest, MinimumAcceptedDimensions) {
    int minSize = MIN_MAZE_SIZE + 1;

    testDimensionsInput(
        std::to_string(minSize) + " " +
        std::to_string(minSize) + "\n",
        true, minSize, minSize);
}

// Test 12: Verify maximum allowed dimensions.
TEST(GameIOTest, MaximumDimensions) {
    int maxSize = MAX_MAZE_SIZE;

    testDimensionsInput(
        std::to_string(maxSize) + " " +
        std::to_string(maxSize) + "\n",
        true, maxSize, maxSize);
}

// Test 13: Reject dimensions below the minimum,
// then accept valid dimensions.
TEST(GameIOTest, BelowMinimumDimensions) {
    int invalid = MIN_MAZE_SIZE - 1;

    testDimensionsInput(
        std::to_string(invalid) + " " +
        std::to_string(invalid) + "\n20 20\n",
        true, 20, 20);
}

// Test 14: Reject dimensions above the maximum.
TEST(GameIOTest, AboveMaximumDimensions) {
    int invalid = MAX_MAZE_SIZE + 1;

    testDimensionsInput(
        std::to_string(invalid) + " " +
        std::to_string(invalid) + "\n20 20\n",
        true, 20, 20);
}

// Test 15: Reject negative dimensions.
TEST(GameIOTest, NegativeDimensions) {
    testDimensionsInput(
        "-5 -10\n20 20\n",
        true, 20, 20);
}

// Test 16: Reject nonnumeric input.
TEST(GameIOTest, NonNumericDimensions) {
    testDimensionsInput(
        "abc xyz\n20 20\n",
        true, 20, 20);
}

// Test 17: Reject zero dimensions.
TEST(GameIOTest, ZeroDimensions) {
    testDimensionsInput(
        "0 0\n20 20\n",
        true, 20, 20);
}

// Test 18: Verify exact minimum dimensions are rejected.
// This documents a possible boundary defect.
TEST(GameIOTest, ExactMinimumRejected) {
    int minSize = MIN_MAZE_SIZE;

    testDimensionsInput(
        std::to_string(minSize) + " " +
        std::to_string(minSize) + "\n20 20\n",
        true, 20, 20);
}

// Test 19: Verify uppercase Q quits input.
TEST(GameIOTest, QuitWithUppercaseQ) {
    testDimensionsInput("Q\n", false, 0, 0);
}

// Test 20: Verify lowercase q quits input.
TEST(GameIOTest, QuitWithLowercaseQ) {
    testDimensionsInput("q\n", false, 0, 0);
}


// Test 21: Verify quit message.
TEST(GameIOTest, QuitMessage) {
    std::string message = GameIO::getQuitMessage();
    EXPECT_NE(message.find("quit the game"), std::string::npos);
}

// Test 22: Verify losing message.
TEST(GameIOTest, LostMessage) {
    std::string message = GameIO::getLostMessage();
    EXPECT_NE(message.find("Game Over"), std::string::npos);
}

// Test 23: Verify winning message.
TEST(GameIOTest, WonMessage) {
    std::string message = GameIO::getWonMessage();
    EXPECT_NE(message.find("Congratulations"), std::string::npos);
}


// Test 24: Verify welcome message.
TEST(GameIOTest, WelcomeMessage) {
    testing::internal::CaptureStdout();
    GameIO::printWelcomeMessage();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Welcome to Robot in Knossos Maze"),
              std::string::npos);
}

// Test 25: Verify game instructions.
TEST(GameIOTest, Instructions) {
    testing::internal::CaptureStdout();
    GameIO::printInstructions();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Use 'W' to move up"),
              std::string::npos);
}

// Test 26: Verify maze generation time.
TEST(GameIOTest, MazeGenerationTime) {
    testing::internal::CaptureStdout();
    GameIO::printMazeGenerationTime(1.234);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("1.234"), std::string::npos);
}

// Test 27: Verify item description.
TEST(GameIOTest, ItemDescription) {
    testing::internal::CaptureStdout();
    GameIO::printItemDescription("Magic Shield");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Magic Shield"), std::string::npos);
}



// Run each test with different maze sizes
INSTANTIATE_TEST_SUITE_P(
    MazeSizes,
    MazeGeneratorTest,
    testing::Values(15, 16, 21)
);
