#include "gtest/gtest.h"

#include "Card.h"
#include "Powers.h"

TEST(CardTest, GetProperties) {
    Card card;
    card.SetHP(10);
    card.SetName("Steve");
    card.SetStrength(10);
    EXPECT_EQ(card.GetHP(), 10);
    EXPECT_EQ(card.GetName(), "Steve");
    EXPECT_EQ(card.GetStrength(), 10);
}

TEST(CardTest, MapPowers){
    Card card;
    Powers powers;
    powers.FillVectors();
    powers.ReturnFrontPower(card);
    powers.ReturnMiddlePower(card);
    powers.ReturnBackPower(card);
    powers.ReturnLeaderPower(card);

    EXPECT_TRUE(card.GetFrontLinePower() != nullptr);
    EXPECT_TRUE(card.GetMiddleLinePower() != nullptr);
    EXPECT_TRUE(card.GetBackLinePower() != nullptr);
    EXPECT_TRUE(card.GetLeaderPower() != nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
