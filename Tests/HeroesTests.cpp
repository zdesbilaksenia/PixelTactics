#include "gtest/gtest.h"

#include "Hero.h"
#include "Card.h"

TEST(HeroTest, CreateHeroFromCard) {
    Card card;
    card.SetHP(10);
    card.SetName("Steve");
    card.SetStrength(10);

    Hero *hero = new Hero(card);
    EXPECT_EQ(hero->GetHealth(), 10);
    //EXPECT_EQ(hero->GetName(), "Steve");
    //EXPECT_EQ(hero->GetStrength(), 10);
}

TEST(HeroTest, Attack) {
    Card card;
    card.SetHP(10);
    card.SetStrength(10);

    Hero *hero1 = new Hero(card);
    Hero *hero2 = new Hero(card);
    hero1->Attack();

    EXPECT_EQ(hero2->GetHealth(), 0);
}

TEST(HeroTest, CanAttack) {
    Card card;
    card.SetHP(10);
    card.SetStrength(10);

    Hero *hero1 = new Hero(card);

    EXPECT_TRUE(hero1->CanAttack());
}

TEST(HeroTest, ReduceHP) {
    Card card;
    card.SetHP(10);
    card.SetStrength(10);

    Hero *hero1 = new Hero(card);
    hero1->ReduceHealth(10);
    EXPECT_EQ(hero1->GetHealth(), 0);
}

TEST(HeroTest, Death) {
    Card card;
    card.SetHP(10);
    card.SetStrength(10);

    Hero *hero1 = new Hero(card);
    hero1->Die();
    EXPECT_TRUE(hero1->IsDead());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
