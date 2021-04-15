#include "gtest/gtest.h"
#include "../Game_Logic/Position.h"

#include "../Game_Logic/Game.h"
#include "../Game_Logic/Hero.h"
#include "../Game_Logic/Deck.h"

TEST(DeckTest, CorrectSize){
    Deck deck();
    EXPECT_TRUE(deck.isMax());
}
TEST(DeckTest, FullDraw){
    Deck deck();
    int maxDeckSize = 30;
    Card card;
    for(int i = 0; i < maxDeckSize; i++){
        card = deck.GetDeck().pop_back();
    }
    EXPECT_TRUE(deck.isEmpty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}