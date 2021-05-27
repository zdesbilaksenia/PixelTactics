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
    mapper.MapPowers(test_pos->GetHero());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(OracleCard);
    mapper.MapPowers(test_pos->GetHero());
    pole.SetPosition(test_pos);
    Position* OraclePos = pole.GetPosition(1,0,1);


    Card PaladinCard = allCards.GetVector()[10];
    test_pos = pole.GetPosition(1,0,0);
    test_pos->SetHero(PaladinCard);
    mapper.MapPowers(test_pos->GetHero());
    Position* PaladinPos = pole.GetPosition(1,0,0);

    std::cout << PaladinCard.frontLinePower << std::endl;
    pole.SetPosition(test_pos);

    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(OraclePos)->GetHero());
    test_pos = pole.GetPosition(1,1,1);
    heroes.push_back(&(test_pos)->GetHero());

    std::vector<bool> HeroesCanBeAttackedDistantly;
    HeroesCanBeAttackedDistantly.resize(0);
    int count = 0;
    int friendlyside = 0;
    pole.Show();
    EXPECT_FALSE(OraclePos->GetHero().frontLinePower == NULL);
    OraclePos->GetHero().frontLinePower(heroes);
    Interception(OraclePos,pole);
    
    heroes.clear();
    heroes.resize(0);
    std::cout << "Атака героя " << PaladinPos->GetHero().GetCurStrength() << std::endl;
    std::cout << "Здоровье противника " << OraclePos->GetHero().GetCurHealth() << std::endl;
    heroes.push_back(&(PaladinPos)->GetHero());
    heroes.push_back(&(OraclePos->GetHero()));

    HeroesCanBeAttackedDistantly = MakeDistantAttack(pole,friendlyside);


    EXPECT_EQ(HeroesCanBeAttackedDistantly.size(),18);
    std::vector<bool> ShouldBe = {false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false};
    EXPECT_EQ(ShouldBe.size(),18);
    for(int i = 0; i < ShouldBe.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_TRUE(ShouldBe[i] == HeroesCanBeAttackedDistantly[i]);
    }
    EXPECT_FALSE(PaladinPos->GetHero().frontLinePower == NULL);
    PaladinPos->GetHero().frontLinePower(heroes);
    std::cout << "Здоровье противника " << OraclePos->GetHero().GetCurHealth() << std::endl;


}