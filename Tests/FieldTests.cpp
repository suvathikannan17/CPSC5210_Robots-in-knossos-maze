
#include <gtest/gtest.h>
#include "Field.h"
#include <utility>

// Test 1: Verify the constructor initializes position and symbol.
TEST(FieldTest, ConstructorInitializesValues) {
    Field field({2, 3}, '#');

    EXPECT_EQ(field.getPosition(), std::make_pair(2, 3));
    EXPECT_EQ(field.getSymbol(), '#');
}

// Test 2: Verify getPosition returns the correct coordinates.
TEST(FieldTest, GetPosition) {
    Field field({5, 7}, ' ');

    EXPECT_EQ(field.getPosition(), std::make_pair(5, 7));
}

// Test 3: Verify setPosition updates the coordinates.
TEST(FieldTest, SetPosition) {
    Field field({1, 1}, '#');

    field.setPosition({4, 6});

    EXPECT_EQ(field.getPosition(), std::make_pair(4, 6));
}

// Test 4: Verify getSymbol returns the correct character.
TEST(FieldTest, GetSymbol) {
    Field field({0, 0}, '#');

    EXPECT_EQ(field.getSymbol(), '#');
}

// Test 5: Verify setSymbol updates the character.
TEST(FieldTest, SetSymbol) {
    Field field({0, 0}, '#');

    field.setSymbol(' ');

    EXPECT_EQ(field.getSymbol(), ' ');
}

// Test 6: Verify position can contain zero and negative coordinates.
TEST(FieldTest, NegativeAndZeroPosition) {
    Field field({0, -3}, '#');

    EXPECT_EQ(field.getPosition(), std::make_pair(0, -3));

    field.setPosition({-2, 0});

    EXPECT_EQ(field.getPosition(), std::make_pair(-2, 0));
}

// Test 7: Verify the symbol can be changed multiple times.
TEST(FieldTest, MultipleSymbolUpdates) {
    Field field({1, 1}, '#');

    field.setSymbol(' ');
    EXPECT_EQ(field.getSymbol(), ' ');

    field.setSymbol('#');
    EXPECT_EQ(field.getSymbol(), '#');
}
