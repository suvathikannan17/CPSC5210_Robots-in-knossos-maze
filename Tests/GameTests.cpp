
#include <gtest/gtest.h>
#include "Game.h"

#include <iostream>
#include <sstream>
#include <string>

class GameTestFixture : public testing::Test {
protected:
    Game game{15, 15, 0};

    void SetUp() override {
        // Create a controlled maze for repeatable testing.
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (i == 0 || i == 14 || j == 0 || j == 14) {
                    game.maze_[i][j]->setSymbol('#');
                } else {
                    game.maze_[i][j]->setSymbol(' ');
                }
            }
        }

        // Place robot at a known position.
        game.robot_ = game.maze_[7][7];
        game.robot_->setSymbol('R');

        // Place minotaur away from the robot.
        game.minotaur_ = game.maze_[2][2];
        game.minotaur_->setSymbol('M');
    }

    bool moveRobot(char direction) {
        return game.moveRobot(direction);
    }

    std::pair<int, int> getRobotPosition() {
        return game.robot_->getPosition();
    }

    void setSymbol(int row, int column, char symbol) {
        game.maze_[row][column]->setSymbol(symbol);
    }

    char getSymbol(int row, int column) {
        return game.maze_[row][column]->getSymbol();
    }
};

// Test 1: Construct game with valid dimensions
TEST(GameTest, ConstructorWithValidDimensions) {
    EXPECT_NO_THROW({
        Game game(15, 15, 3);
    });
}

// Test 2: Construct game without items
TEST(GameTest, ConstructorWithZeroItems) {
    EXPECT_NO_THROW({
        Game game(15, 15, 0);
    });
}

// Test 3: Verify that entering q quits the game
TEST(GameTest, QuitGame) {
    Game game(15, 15, 0);

    std::istringstream input("q\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    try {
        game.run();
    } catch (...) {
        std::cin.rdbuf(oldInput);
        std::cout.rdbuf(oldOutput);
        throw;
    }

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    EXPECT_NE(
        output.str().find("You have chosen to quit the game!"),
        std::string::npos
    );
}

// Test 4: Move robot upward.
TEST_F(GameTestFixture, MoveRobotUp) {
    EXPECT_FALSE(moveRobot('W'));
    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_EQ(getSymbol(7, 7), ' ');
    EXPECT_EQ(getSymbol(6, 7), 'R');
}

// Test 5: Move robot downward.
TEST_F(GameTestFixture, MoveRobotDown) {
    EXPECT_FALSE(moveRobot('S'));
    EXPECT_EQ(getRobotPosition(), std::make_pair(8, 7));
}

// Test 6: Move robot left.
TEST_F(GameTestFixture, MoveRobotLeft) {
    EXPECT_FALSE(moveRobot('A'));
    EXPECT_EQ(getRobotPosition(), std::make_pair(7, 6));
}

// Test 7: Move robot right.
TEST_F(GameTestFixture, MoveRobotRight) {
    EXPECT_FALSE(moveRobot('D'));
    EXPECT_EQ(getRobotPosition(), std::make_pair(7, 8));
}

// Test 8: Robot cannot move through a wall.
TEST_F(GameTestFixture, RobotBlockedByWall) {
    setSymbol(6, 7, '#');

    EXPECT_FALSE(moveRobot('W'));
    EXPECT_EQ(getRobotPosition(), std::make_pair(7, 7));
    EXPECT_EQ(getSymbol(6, 7), '#');
    EXPECT_EQ(getSymbol(7, 7), 'R');
}

// Test 9: Robot can move into an entrance field.
TEST_F(GameTestFixture, RobotMovesIntoEntrance) {
    setSymbol(6, 7, 'I');

    EXPECT_FALSE(moveRobot('W'));
    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_EQ(getSymbol(6, 7), 'R');
}