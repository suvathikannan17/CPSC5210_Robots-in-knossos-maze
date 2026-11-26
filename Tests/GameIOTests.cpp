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

// // Run each test with different maze sizes
// INSTANTIATE_TEST_SUITE_P(
//     MazeSizes,
//     GameIOTest,
//     testing::Values(15, 16, 21)
// );