
#include <gtest/gtest.h>
#include "Item.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>

// WarFog tests

TEST(ItemTest, WarFogType) {
    WarFog item({2, 3});
    EXPECT_EQ(item.getType(), ItemType::WAR_FOG);
}

TEST(ItemTest, WarFogDescription) {
    WarFog item({2, 3});
    EXPECT_EQ(item.getDescription(),
              "War Mist: A mystical fog that temporarily limits vision.");
}

TEST(ItemTest, WarFogPositionAndSymbol) {
    WarFog item({2, 3});
    EXPECT_EQ(item.getPosition(), std::make_pair(2, 3));
    EXPECT_EQ(item.getSymbol(), 'P');
}

// Sword tests

TEST(ItemTest, SwordType) {
    Sword item({4, 5});
    EXPECT_EQ(item.getType(), ItemType::SWORD);
}

TEST(ItemTest, SwordDescription) {
    Sword item({4, 5});
    EXPECT_EQ(item.getDescription(),
              "Sword: A sharp blade that can slay the minotaur.");
}

TEST(ItemTest, SwordPositionAndSymbol) {
    Sword item({4, 5});
    EXPECT_EQ(item.getPosition(), std::make_pair(4, 5));
    EXPECT_EQ(item.getSymbol(), 'P');
}

// Shield tests

TEST(ItemTest, ShieldType) {
    Shield item({6, 7});
    EXPECT_EQ(item.getType(), ItemType::SHIELD);
}

TEST(ItemTest, ShieldDescription) {
    Shield item({6, 7});
    EXPECT_EQ(item.getDescription(),
              "Shield: A sturdy shield that provides protection from minotaur attacks.");
}

TEST(ItemTest, ShieldPositionAndSymbol) {
    Shield item({6, 7});
    EXPECT_EQ(item.getPosition(), std::make_pair(6, 7));
    EXPECT_EQ(item.getSymbol(), 'P');
}

// Hammer tests

TEST(ItemTest, HammerType) {
    Hammer item({8, 9});
    EXPECT_EQ(item.getType(), ItemType::HAMMER);
}

TEST(ItemTest, HammerDescription) {
    Hammer item({8, 9});
    EXPECT_EQ(item.getDescription(),
              "Hammer: A heavy hammer that can break through walls.");
}

TEST(ItemTest, HammerPositionAndSymbol) {
    Hammer item({8, 9});
    EXPECT_EQ(item.getPosition(), std::make_pair(8, 9));
    EXPECT_EQ(item.getSymbol(), 'P');
}

// Random item factory tests

TEST(ItemTest, RandomItemIsCreated) {
    std::unique_ptr<Item> item(
        Item::createRandomItem({3, 4})
    );

    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->getPosition(), std::make_pair(3, 4));
    EXPECT_EQ(item->getSymbol(), 'P');
}

TEST(ItemTest, RandomItemHasValidType) {
    for (int i = 0; i < 100; i++) {
        std::unique_ptr<Item> item(
            Item::createRandomItem({1, 2})
        );

        ASSERT_NE(item, nullptr);

        ItemType type = item->getType();

        EXPECT_TRUE(
            type == ItemType::WAR_FOG ||
            type == ItemType::SWORD ||
            type == ItemType::SHIELD ||
            type == ItemType::HAMMER
        );
    }
}