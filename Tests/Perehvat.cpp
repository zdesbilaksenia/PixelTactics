#include "gtest/gtest.h"
#include "Game.h"

TEST(Perehvat,Test_1){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    PowerMapper mapper;
    Position* test_pos = pole.GetPosition(1,1,1);
    Card OracleCard = allCards.GetVector()[9];
    std::cout << OracleCard.frontLinePower << std::endl;
    test_pos->SetHero(OracleCard);
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(OracleCard);
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,0,0);
    test_pos->SetHero(OracleCard);

    Card PaladinCard = allCards.GetVector()[10];
    std::cout << PaladinCard.frontLinePower << std::endl;
    pole.SetPosition(test_pos);

    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesCanBeAttackedDistantly;
    HeroesCanBeAttackedDistantly.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty()) &&
            pole_.GetVector()[1]->GetHero().CanBeAttackedDistantly()) {
            HeroesCanBeAttackedDistantly.push_back(true);
        } else {
            HeroesCanBeAttackedDistantly.push_back(false);
        }
    }


    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {false,true,false,false,false,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        ASSERT_TRUE(ShouldBe[i] == result[i]);
    }

}