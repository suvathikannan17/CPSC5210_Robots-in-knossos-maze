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
#include <fstream>
#include <filesystem>

using namespace std;

class GameIOTest : public testing::TestWithParam<int> {};

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

// Test 28: Valid Dimensions Input with Extra Spaces
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
}


// Test 29: Accept a valid number of items.
TEST(GameIOTest, ValidItemsNumber) {
    std::istringstream input("4\nq\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int items = 0;
    bool result = GameIO::getValidItemsNum(items, 20, 20);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(result);
    EXPECT_EQ(items, 4);
}

// Test 30: Reject negative item count.
TEST(GameIOTest, NegativeItemsNumber) {
    std::istringstream input("-5\n4\nq\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int items = 0;
    bool result = GameIO::getValidItemsNum(items, 20, 20);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(result);
    EXPECT_EQ(items, 4);
    EXPECT_NE(output.str().find("Error:"), std::string::npos);
}

// Test 31: Reject nonnumeric item input.
TEST(GameIOTest, NonNumericItemsNumber) {
    std::istringstream input("abc\n4\nq\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int items = 0;
    bool result = GameIO::getValidItemsNum(items, 20, 20);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(result);
    EXPECT_EQ(items, 4);
    EXPECT_NE(output.str().find("Error:"), std::string::npos);
}

// Test 32: Quit item count input.
TEST(GameIOTest, QuitItemsNumber) {
    std::istringstream input("q\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int items = 0;
    bool result = GameIO::getValidItemsNum(items, 20, 20);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_FALSE(result);
}

// Test 33: Accept uppercase W for movement.
TEST(GameIOTest, MoveUpUppercase) {
    std::istringstream input("W\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    char direction = '\0';
    bool quit = GameIO::getRobotMoveDirection(direction);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_FALSE(quit);
    EXPECT_EQ(direction, 'W');
}

// Test 34: Accept lowercase movement.
TEST(GameIOTest, MoveLeftLowercase) {
    std::istringstream input("a\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    char direction = '\0';
    bool quit = GameIO::getRobotMoveDirection(direction);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_FALSE(quit);
    EXPECT_EQ(direction, 'a');
}

// Test 35: Reject invalid movement and accept valid movement.
TEST(GameIOTest, InvalidMoveThenValid) {
    std::istringstream input("X\nD\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    char direction = '\0';
    bool quit = GameIO::getRobotMoveDirection(direction);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_FALSE(quit);
    EXPECT_EQ(direction, 'D');
    EXPECT_NE(output.str().find("Invalid input"), std::string::npos);
}

// Test 36: Quit movement using lowercase q.
TEST(GameIOTest, QuitMovementLowercase) {
    std::istringstream input("q\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    char direction = '\0';
    bool quit = GameIO::getRobotMoveDirection(direction);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(quit);
}

// Test 37: Quit movement using uppercase Q.
TEST(GameIOTest, QuitMovementUppercase) {
    std::istringstream input("Q\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    char direction = '\0';
    bool quit = GameIO::getRobotMoveDirection(direction);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(quit);
}

// Test 38: Verify maze output.
TEST(GameIOTest, PrintMaze) {
    Maze maze(15, 15, 0);

    testing::internal::CaptureStdout();
    GameIO::printMaze(maze);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.empty());
}

// Test 39: Verify maximum item count calculation.
TEST(GameIOTest, MaximumItemsCalculation) {
    EXPECT_EQ(get_max_items_num(20, 20), 16);
    EXPECT_EQ(get_max_items_num(30, 30), 39);
}

// Test 40: Accept maximum allowed item count.
TEST(GameIOTest, MaximumItemsAccepted) {
    std::istringstream input("16\nq\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int items = 0;
    bool result = GameIO::getValidItemsNum(items, 20, 20);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(result);
    EXPECT_EQ(items, 16);
}

// Test 41: Reject item count above maximum.
TEST(GameIOTest, AboveMaximumItemsRejected) {
    std::istringstream input("17\n4\nq\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int items = 0;
    bool result = GameIO::getValidItemsNum(items, 20, 20);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(result);
    EXPECT_EQ(items, 4);
    EXPECT_NE(output.str().find("Error:"), std::string::npos);
}

// Test 42: Verify exact minimum item count is rejected.
// Documents current boundary validation behavior.
TEST(GameIOTest, ExactMinimumItemsRejected) {
    std::istringstream input("3\n4\nq\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    int items = 0;
    bool result = GameIO::getValidItemsNum(items, 20, 20);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(result);
    EXPECT_EQ(items, 4);
    EXPECT_NE(output.str().find("Error:"), std::string::npos);
}

// Test 43: Verify all valid movement directions.
TEST(GameIOTest, AllValidMovementDirections) {
    std::string directions = "WwAaSsDd";

    for (char expected : directions) {
        std::istringstream input(
            std::string(1, expected) + "\nq\n");
        std::ostringstream output;

        auto* oldCin = std::cin.rdbuf(input.rdbuf());
        auto* oldCout = std::cout.rdbuf(output.rdbuf());

        char direction = '\0';
        bool quit = GameIO::getRobotMoveDirection(direction);

        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);

        EXPECT_FALSE(quit);
        EXPECT_EQ(direction, expected);
    }
}

// Test 44: Verify fogged maze output.
TEST(GameIOTest, PrintFoggedMaze) {
    Maze maze(15, 15, 0);
    Field* robot = maze[7][7];

    testing::internal::CaptureStdout();
    GameIO::printFoggedMaze(maze, robot);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.empty());

    std::istringstream lines(output);
    std::string line;
    int rowCount = 0;

    while (std::getline(lines, line)) {
        EXPECT_EQ(line.length(), 7u);
        rowCount++;
    }

    EXPECT_EQ(rowCount, 7);
}

// Test 45: Verify fogged maze at the boundary.
TEST(GameIOTest, PrintFoggedMazeAtBoundary) {
    Maze maze(15, 15, 0);
    Field* robot = maze[0][0];

    testing::internal::CaptureStdout();
    GameIO::printFoggedMaze(maze, robot);
    std::string output = testing::internal::GetCapturedStdout();

    std::istringstream lines(output);
    std::string line;
    int rowCount = 0;

    while (std::getline(lines, line)) {
        EXPECT_EQ(line.length(), 4u);
        rowCount++;
    }

    EXPECT_EQ(rowCount, 4);
}

// Test 46: Verify maze is written to a file.
TEST(GameIOTest, WriteMazeToFile) {
    Maze maze(15, 15, 0);

    std::string filename = "test_gameio_maze_output.txt";
    std::string message = "Test game completed";

    GameIO::writeMazeToFile(maze, filename, message);

    std::ifstream file(filename);

    EXPECT_TRUE(file.is_open());

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string content = buffer.str();

    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find(message), std::string::npos);
    EXPECT_NE(content.find("#"), std::string::npos);

    file.close();

    // Remove the generated test file.
    std::filesystem::remove(filename);
}

// Test 47: Verify invalid file path handling.
TEST(GameIOTest, WriteMazeToInvalidPath) {
    Maze maze(15, 15, 0);

    std::string filename = "/dev/null/test_maze.txt";
    std::string message = "Test result";

    testing::internal::CaptureStdout();

    GameIO::writeMazeToFile(maze, filename, message);

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(
        output.find("Error: Unable to open file"),
        std::string::npos
    );
}