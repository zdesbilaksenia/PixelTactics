#include "gtest/gtest.h"
#include "Hero.h"

TEST(HeroTest, CreateHeroFromCard) {
    Card card;
    card.HP = 10;
    card.name = "Steve";
    card.strength = 10;
    Hero *hero = new Hero(card);

    EXPECT_EQ(hero->GetHealth(), 10);
    EXPECT_EQ(hero->GetName(), "Steve");
    EXPECT_EQ(hero->GetStrength(), 10);
}

TEST(HeroTest, Attack) {
    Card card;
    card.HP = 10;
    card.name = "Steve";
    card.strength = 10;

    Hero *hero1 = new Hero(card);
    Hero *hero2 = new Hero(card);
    hero1->Attack(*hero2, hero1->GetCurStrength());

    EXPECT_EQ(hero2->GetCurHealth(), 0);
}

TEST(HeroTest, MakeLeader) {
    Card card;
    card.HP = 10;
    card.name = "Steve";
    card.strength = 10;

    Hero *hero1 = new Hero(card);
    hero1->MakeLeader();
    EXPECT_TRUE(hero1->IsLeader());
    EXPECT_EQ(hero1->GetCurHealth(), 20);
}

TEST(HeroTest, Properties) {
    Card card;
    card.HP = 10;
    card.name = "Steve";
    card.strength = 10;

    Hero *hero1 = new Hero(card);

    EXPECT_EQ(hero1->GetCurHealth(), 10);
    EXPECT_EQ(hero1->GetCurStrength(), 10);
    EXPECT_EQ(hero1->GetName(),"Steve");
    EXPECT_FALSE(hero1->IsDead());
    EXPECT_FALSE(hero1->IsLeader());
    EXPECT_FALSE(hero1->IsDamaged());
    hero1->ReduceHealth(2);
    EXPECT_EQ(hero1->GetCurHealth(), 8);
    EXPECT_TRUE(hero1->IsDamaged());
    hero1->SetCurStrength(15);
    EXPECT_EQ(hero1->GetCurStrength(), 15);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
