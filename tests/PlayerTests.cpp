#include "gtest/gtest.h"

#include "../Game_Logic/Game.h"
#include "../Game_Logic/Deck.h"
#include "../Game_Logic/Player.h"
#include <vector>
#include "../Game_Logic/Card.h"

TEST(PlayerTest,DrawCard){
    Player player();
    for(int i = 0; i < 3; i++){
        player.DrawCard();
    }
    std::vector<Card> testHand;
    testHand = player.GetHand();
    EXPECT_TRUE(testHand.size(),3);
}

TEST(PlayerTest,FullHand){
    Player player();
    for(int i = 0; i < 100; i++){
        player.DrawCard();
    }
    std::vector<Card> testHand;
    testHand = player.GetHand();
    //Рука не больше 10
    EXPECT_TRUE(testHand.size(),10);
}

TEST(PlayerTest,Test)

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}