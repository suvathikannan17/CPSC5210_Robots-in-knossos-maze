
#include <gtest/gtest.h>
#include "Maze.h"

#include <sstream>
#include <string>

// Test 7: Verify that the Maze class returns the correct row and column counts.
TEST(MazeTest, CorrectRowsAndColumns) {
    Maze maze(15, 20, 0);

    EXPECT_EQ(maze.getRows(), 15);
    EXPECT_EQ(maze.getColumns(), 20);
}

// Test 8: Verify that the const operator[] can access maze cells.
TEST(MazeTest, ConstIndexOperator) {
    Maze maze(15, 15, 0);

    const Maze& constMaze = maze;

    EXPECT_EQ(constMaze[0].size(), 15);
    EXPECT_NE(constMaze[0][0], nullptr);
}

// Test 9: Verify that printing the maze produces the correct
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