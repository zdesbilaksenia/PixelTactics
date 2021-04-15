#include "gtest/gtest.h"
#include "Position.h"

#include "../Game Logic/Game.h"
#include "Hero.h"

TEST(GameTest, CheckPole){
    Game game();
    int amountOfPositions = 18
    for(int i = 0; i < amountOfPositions; i++){
        int cell = test.GetCell();
        int line = test.GetLine();
        int side = test.GetSide();
        bool condition = test.isEmpty();
        EXPECT_EQ(cell,test.GetCell());
        EXPECT_EQ(line,test.GetLine());
        EXPECT_EQ(side,test.GetSide());
        EXPECT_TRUE(condition);
    }
}

TEST(GameTest, DiffrentSides){
    Game game();
    EXPECT_NE(game.GetFirstPlayer().GetSide(), game.GetSecondPlayer());
}

TEST(GameTest, IsLeaderOnPosition){
    Game game();
    int firstLeaderCell = 4;
    int secondLeaderCell = 13;
    std::vector <Position> testPole = game.GetPole();
    //Центральное поле первого игрока
    EXPECT_FALSE(testPole[firstLeaderCell].isEmpty());
    //Центральное поле второго игрока
    EXPECT_FALSE(testPole[secondLeaderCell].isEmpty());
    Hero firstLeader();
    Hero secondLeader();
    firstLeader = testPole[firstLeaderCell].GetHero();
    secondLeader = testPole[secondLeaderCell].GetHero();
    EXPECT_TRUE(firstLeader.IsLeader());
    EXPECT_TRUE(secondLeader.IsLeader());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}