
#include <gtest/gtest.h>
#include "Game.h"

#include <iostream>
#include <sstream>
#include <string>

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