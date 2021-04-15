#include "gtest/gtest.h"
#include "../DrawableEntities/DrawableBox.h"
#include "../DrawableEntities/Unit.h"
#include "../DrawableEntities/Tile.h"
#include "../DrawableEntities/Card.h"
#include "../DrawableEntities/ScreenParts/BattleField.h"
#include "../DrawableEntities/ScreenParts/BottomPanel.h"

TEST(testDrawableBox, test_1) {
    DrawableBox box;
    box.setPosition(1, 1);
    EXPECT_EQ(box.getPosX(), 1);
    EXPECT_EQ(box.getPosY(), 1);
}

TEST(testUnit, test_1) {
    Unit unit(12);
    unit.setAttack(15);
    unit.setHealth(3);
    unit.setPosX(1);
    unit.setPosY(3);
    EXPECT_EQ(unit.getCharacterId(), 12);
    EXPECT_EQ(unit.getAttack(), 15);
    EXPECT_EQ(unit.getHealth(), 3);
    EXPECT_EQ(unit.getPosX(), 1);
    EXPECT_EQ(unit.getPosY(), 3);
}

TEST(testTile, test_1) {
    Tile tile;
    tile.setStatus(3);
    tile.setPosX(1);
    tile.setPosY(3);
    EXPECT_EQ(tile.getStatus(), 3);
    EXPECT_EQ(tile.getPosX(), 1);
    EXPECT_EQ(tile.getPosY(), 3);
}

TEST(testCardTile, test_1) {
    Card card(5);
    card.setPosX(1);
    card.setPosY(3);
    EXPECT_EQ(card.getCharacterId(), 5);
    EXPECT_EQ(card.getPosX(), 1);
    EXPECT_EQ(card.getPosY(), 3);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}