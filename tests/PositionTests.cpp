#include "gtest/gtest.h"

#include "../Game Logic/Position.h"
#include "../Game_Logic/Player.h"

TEST(PositionTest,CorrectPosition){
    Position test(1,2,1);
    int cell = test.GetCell();
    int line = test.GetLine();
    int side = test.GetSide();
    bool condition = test.isEmpty();
    EXPECT_EQ(cell,1);
    EXPECT_EQ(line,2);
    EXPECT_EQ(side,1);
    EXPECT_TRUE(condition);
}

TEST(PositionTest,SetHero){
    Position test(1,1,1);
    test.SetHero();
    EXPECT_FALSE(test.isEmpty());
}

TEST(PositionTest,GetHero){
    Position test(1,1,1);
    Hero hero();
    test.SetHero(hero);
    Hero checkHero();
    checkHero = test.GetHero();
    EXPECT_EQ(checkHero.GetHP(),hero.GetHP());
    EXPECT_EQ(checkHero.GetStrength(),hero.GetStrength());
    EXPECT_EQ(checkHero.GetName(),hero.GetName());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}