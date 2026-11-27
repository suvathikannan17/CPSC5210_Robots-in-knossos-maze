
#include <gtest/gtest.h>
#include "Game.h"
#include "Item.h"
#include "GameIO.h"
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

    void activateHammer() {
    game.hammer_state_ = {true, 3};
    }

    void activateSword() {
        game.sword_state_ = {true, 3};
    }

    void activateShield() {
        game.shield_state_ = {true, 3};
    }

    void placeRobot(int row, int column) {
        game.robot_->setSymbol(' ');
        game.robot_ = game.maze_[row][column];
        game.robot_->setSymbol('R');
    }

    void placeMinotaur(int row, int column) {
        game.minotaur_->setSymbol(' ');
        game.minotaur_ = game.maze_[row][column];
        game.minotaur_->setSymbol('M');
    }

    void activateWarFog() {
    game.war_fog_state_ = {true, 3};
    }

    void updateItems() {
        game.updateItems();
    }

    std::pair<bool, int> getSwordState() {
        return game.sword_state_;
    }

    std::pair<bool, int> getShieldState() {
        return game.shield_state_;
    }

    std::pair<bool, int> getHammerState() {
        return game.hammer_state_;
    }

    std::pair<bool, int> getWarFogState() {
        return game.war_fog_state_;
    }

    void placeItem(Item* item) {
    auto position = item->getPosition();

    int row = position.first;
    int column = position.second;

    delete game.maze_[row][column];
    game.maze_[row][column] = item;
    }

    bool moveMinotaur() {
    return game.moveMinotaur();
    }

    std::pair<int, int> getMinotaurPosition() {
        return game.minotaur_->getPosition();
    }

    void surroundMinotaurWithWalls() {
        auto position = getMinotaurPosition();

        int row = position.first;
        int col = position.second;

        setSymbol(row - 1, col, '#');
        setSymbol(row + 1, col, '#');
        setSymbol(row, col - 1, '#');
        setSymbol(row, col + 1, '#');
    }

    std::string runGame(const std::string& commands) {
    std::istringstream input(commands + "\nq\n");
    std::ostringstream output;

    auto* oldCin = std::cin.rdbuf(input.rdbuf());
    auto* oldCout = std::cout.rdbuf(output.rdbuf());

    try {
        game.run();
    } catch (...) {
        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);
        throw;
    }

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    return output.str();
    }

    void blockMinotaur() {
        surroundMinotaurWithWalls();
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

// Test 10: Robot can break an interior wall with hammer.
TEST_F(GameTestFixture, HammerBreaksWall) {
    activateHammer();
    setSymbol(6, 7, '#');

    EXPECT_FALSE(moveRobot('W'));

    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_EQ(getSymbol(6, 7), 'R');
    EXPECT_EQ(getSymbol(7, 7), ' ');
}

// Test 11: Hammer cannot break a boundary wall.
TEST_F(GameTestFixture, HammerCannotBreakBoundaryWall) {
    activateHammer();
    placeRobot(1, 7);

    EXPECT_FALSE(moveRobot('W'));

    EXPECT_EQ(getRobotPosition(), std::make_pair(1, 7));
    EXPECT_EQ(getSymbol(0, 7), '#');
    EXPECT_EQ(getSymbol(1, 7), 'R');
}

// Test 12: Robot defeats minotaur using sword.
TEST_F(GameTestFixture, RobotDefeatsMinotaurWithSword) {
    activateSword();
    placeMinotaur(6, 7);

    EXPECT_TRUE(moveRobot('W'));

    EXPECT_EQ(getSymbol(7, 7), ' ');
    EXPECT_EQ(getSymbol(6, 7), 'R');
}

// Test 13: Shield protects robot during collision.
TEST_F(GameTestFixture, ShieldProtectsRobot) {
    activateShield();
    placeMinotaur(6, 7);

    EXPECT_FALSE(moveRobot('W'));

    EXPECT_EQ(getRobotPosition(), std::make_pair(7, 7));
    EXPECT_EQ(getSymbol(7, 7), 'R');
    EXPECT_EQ(getSymbol(6, 7), 'M');
}

// Test 14: Robot loses combat without sword or shield.
TEST_F(GameTestFixture, RobotLosesWithoutProtection) {
    placeMinotaur(6, 7);

    EXPECT_TRUE(moveRobot('W'));

    EXPECT_EQ(getSymbol(7, 7), ' ');
    EXPECT_EQ(getSymbol(6, 7), 'M');
}

// Test 15: Sword effect expires.
TEST_F(GameTestFixture, SwordExpires) {
    activateSword();

    for (int i = 0; i < 4; i++) {
        updateItems();
    }

    EXPECT_FALSE(getSwordState().first);
}

// Test 16: Shield effect expires.
TEST_F(GameTestFixture, ShieldExpires) {
    activateShield();

    for (int i = 0; i < 4; i++) {
        updateItems();
    }

    EXPECT_FALSE(getShieldState().first);
}

// Test 17: Hammer effect expires.
TEST_F(GameTestFixture, HammerExpires) {
    activateHammer();

    for (int i = 0; i < 4; i++) {
        updateItems();
    }

    EXPECT_FALSE(getHammerState().first);
}

// Test 18: War Fog effect expires.
TEST_F(GameTestFixture, WarFogExpires) {
    activateWarFog();

    for (int i = 0; i < 4; i++) {
        updateItems();
    }

    EXPECT_FALSE(getWarFogState().first);
}

// Test 19: Active items remain active before expiration.
TEST_F(GameTestFixture, ItemsRemainActiveBeforeExpiration) {
    activateSword();
    activateShield();
    activateHammer();
    activateWarFog();

    updateItems();

    EXPECT_EQ(getSwordState(), std::make_pair(true, 2));
    EXPECT_EQ(getShieldState(), std::make_pair(true, 2));
    EXPECT_EQ(getHammerState(), std::make_pair(true, 2));
    EXPECT_EQ(getWarFogState(), std::make_pair(true, 2));
}

// Test 20: Verify item expiration boundary.
TEST_F(GameTestFixture, ItemExpirationBoundary) {
    activateSword();
    activateShield();
    activateHammer();
    activateWarFog();

    for (int i = 0; i < 3; i++) {
        updateItems();
    }

    EXPECT_EQ(getSwordState(), std::make_pair(true, 0));
    EXPECT_EQ(getShieldState(), std::make_pair(true, 0));
    EXPECT_EQ(getHammerState(), std::make_pair(true, 0));
    EXPECT_EQ(getWarFogState(), std::make_pair(true, 0));

    updateItems();

    EXPECT_EQ(getSwordState(), std::make_pair(false, -1));
    EXPECT_EQ(getShieldState(), std::make_pair(false, -1));
    EXPECT_EQ(getHammerState(), std::make_pair(false, -1));
    EXPECT_EQ(getWarFogState(), std::make_pair(false, -1));
}

// Test 21: Robot collects hammer and activates its effect.
TEST_F(GameTestFixture, CollectHammer) {
    placeItem(new Hammer({6, 7}));

    EXPECT_EQ(getSymbol(6, 7), 'P');
    EXPECT_FALSE(getHammerState().first);

    EXPECT_FALSE(moveRobot('W'));

    EXPECT_EQ(getHammerState(), std::make_pair(true, 3));
    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_EQ(getSymbol(6, 7), 'R');
    EXPECT_EQ(getSymbol(7, 7), ' ');
}

// Test 22: Robot collects sword and activates its effect.
TEST_F(GameTestFixture, CollectSword) {
    placeItem(new Sword({6, 7}));

    EXPECT_EQ(getSymbol(6, 7), 'P');
    EXPECT_FALSE(getSwordState().first);

    EXPECT_FALSE(moveRobot('W'));

    EXPECT_EQ(getSwordState(), std::make_pair(true, 3));
    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_EQ(getSymbol(6, 7), 'R');
    EXPECT_EQ(getSymbol(7, 7), ' ');
}

// Test 23: Robot collects shield and activates its effect.
TEST_F(GameTestFixture, CollectShield) {
    placeItem(new Shield({6, 7}));

    EXPECT_EQ(getSymbol(6, 7), 'P');
    EXPECT_FALSE(getShieldState().first);

    EXPECT_FALSE(moveRobot('W'));

    EXPECT_EQ(getShieldState(), std::make_pair(true, 3));
    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_EQ(getSymbol(6, 7), 'R');
    EXPECT_EQ(getSymbol(7, 7), ' ');
}

// Test 24: Robot collects war fog and activates its effect.
TEST_F(GameTestFixture, CollectWarFog) {
    placeItem(new WarFog({6, 7}));

    EXPECT_EQ(getSymbol(6, 7), 'P');
    EXPECT_FALSE(getWarFogState().first);

    EXPECT_FALSE(moveRobot('W'));

    EXPECT_EQ(getWarFogState(), std::make_pair(true, 3));
    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_EQ(getSymbol(6, 7), 'R');
    EXPECT_EQ(getSymbol(7, 7), ' ');
}

// Test 25: Minotaur moves to an available empty field.
TEST_F(GameTestFixture, MinotaurMovesToEmptyField) {
    surroundMinotaurWithWalls();
    setSymbol(2, 3, ' ');

    EXPECT_FALSE(moveMinotaur());

    EXPECT_EQ(getMinotaurPosition(), std::make_pair(2, 3));
    EXPECT_EQ(getSymbol(2, 2), ' ');
    EXPECT_EQ(getSymbol(2, 3), 'M');
}

// Test 26: Minotaur cannot move when surrounded by walls.
TEST_F(GameTestFixture, MinotaurBlockedByWalls) {
    surroundMinotaurWithWalls();

    EXPECT_FALSE(moveMinotaur());

    EXPECT_EQ(getMinotaurPosition(), std::make_pair(2, 2));
    EXPECT_EQ(getSymbol(2, 2), 'M');
}

// Test 27: Minotaur can move onto an item field.
TEST_F(GameTestFixture, MinotaurMovesOntoItem) {
    surroundMinotaurWithWalls();
    placeItem(new Sword({2, 3}));

    EXPECT_EQ(getSymbol(2, 3), 'P');

    EXPECT_FALSE(moveMinotaur());

    EXPECT_EQ(getMinotaurPosition(), std::make_pair(2, 3));
    EXPECT_EQ(getSymbol(2, 2), ' ');
    EXPECT_EQ(getSymbol(2, 3), 'M');
}

// Test 28: Minotaur catches robot without protection.
TEST_F(GameTestFixture, MinotaurCatchesRobot) {
    placeRobot(2, 3);

    EXPECT_TRUE(moveMinotaur());

    EXPECT_EQ(getMinotaurPosition(), std::make_pair(2, 3));
    EXPECT_EQ(getSymbol(2, 2), ' ');
    EXPECT_EQ(getSymbol(2, 3), 'M');
}

// Test 29: Sword defeats an attacking minotaur.
TEST_F(GameTestFixture, SwordDefeatsAttackingMinotaur) {
    activateSword();
    placeRobot(2, 3);

    EXPECT_TRUE(moveMinotaur());

    EXPECT_EQ(getSymbol(2, 2), ' ');
    EXPECT_EQ(getSymbol(2, 3), 'R');
    EXPECT_EQ(getRobotPosition(), std::make_pair(2, 3));
}

// Test 30: Shield blocks minotaur when no other move exists.
TEST_F(GameTestFixture, ShieldBlocksMinotaur) {
    activateShield();
    surroundMinotaurWithWalls();
    placeRobot(2, 3);

    EXPECT_FALSE(moveMinotaur());

    EXPECT_EQ(getMinotaurPosition(), std::make_pair(2, 2));
    EXPECT_EQ(getRobotPosition(), std::make_pair(2, 3));
    EXPECT_EQ(getSymbol(2, 2), 'M');
    EXPECT_EQ(getSymbol(2, 3), 'R');
}

// Test 31: Minotaur moves away from robot with shield.
TEST_F(GameTestFixture, MinotaurMovesAwayFromShield) {
    activateShield();
    surroundMinotaurWithWalls();
    placeRobot(2, 3);

    setSymbol(3, 2, ' ');

    EXPECT_FALSE(moveMinotaur());

    EXPECT_EQ(getMinotaurPosition(), std::make_pair(3, 2));
    EXPECT_EQ(getRobotPosition(), std::make_pair(2, 3));
    EXPECT_EQ(getSymbol(2, 2), ' ');
    EXPECT_EQ(getSymbol(3, 2), 'M');
    EXPECT_EQ(getSymbol(2, 3), 'R');
}

// Test 32: Robot moves normally before quitting.
TEST_F(GameTestFixture, RunGameMoveThenQuit) {
    blockMinotaur();

    std::string output = runGame("W");

    EXPECT_EQ(getRobotPosition(), std::make_pair(6, 7));
    EXPECT_NE(
        output.find(GameIO::getQuitMessage()),
        std::string::npos
    );
}

// Test 33: Robot wins by reaching the top row.
TEST_F(GameTestFixture, RunGameWinByReachingEntrance) {
    blockMinotaur();
    placeRobot(1, 7);
    setSymbol(0, 7, 'I');

    std::string output = runGame("W");

    EXPECT_EQ(getRobotPosition(), std::make_pair(0, 7));
    EXPECT_NE(
        output.find(GameIO::getWonMessage()),
        std::string::npos
    );
}

// Test 34: Robot wins by attacking minotaur with sword.
TEST_F(GameTestFixture, RunGameWinWithSword) {
    activateSword();
    placeMinotaur(6, 7);

    std::string output = runGame("W");

    EXPECT_NE(
        output.find(GameIO::getWonMessage()),
        std::string::npos
    );

    EXPECT_EQ(getSymbol(6, 7), 'R');
}

// Test 35: Robot loses when moving into minotaur.
TEST_F(GameTestFixture, RunGameLoseByRobotCollision) {
    placeMinotaur(6, 7);

    std::string output = runGame("W");

    EXPECT_NE(
        output.find(GameIO::getLostMessage()),
        std::string::npos
    );

    EXPECT_EQ(getSymbol(6, 7), 'M');
}

// Test 36: Robot wins when minotaur attacks active sword.
TEST_F(GameTestFixture, RunGameWinByMinotaurAttack) {
    activateSword();
    placeMinotaur(5, 7);

    std::string output = runGame("W");

    EXPECT_NE(
        output.find(GameIO::getWonMessage()),
        std::string::npos
    );

    EXPECT_EQ(getSymbol(5, 7), ' ');
    EXPECT_EQ(getSymbol(6, 7), 'R');
}

// Test 37: Robot loses when minotaur catches it.
TEST_F(GameTestFixture, RunGameLoseByMinotaurAttack) {
    placeMinotaur(5, 7);

    std::string output = runGame("W");

    EXPECT_NE(
        output.find(GameIO::getLostMessage()),
        std::string::npos
    );

    EXPECT_EQ(getSymbol(6, 7), 'M');
}

// Test 38: Game displays fogged maze when war fog is active.
TEST_F(GameTestFixture, RunGameWithWarFog) {
    activateWarFog();

    std::string output = runGame("q");

    EXPECT_NE(
        output.find(GameIO::getQuitMessage()),
        std::string::npos
    );

    EXPECT_NE(
        output.find("   R   "),
        std::string::npos
    );

    EXPECT_EQ(
        output.find(std::string(15, '#')),
        std::string::npos
    );
}