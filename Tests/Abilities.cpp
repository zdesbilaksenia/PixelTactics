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
   // Interception(OraclePos,pole);
    
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

TEST(Witch_and_Priest,Test_2){
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
//    Interception(OraclePos,pole);
    
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


    EXPECT_TRUE(OraclePos->GetHero().IsDead());

    Card WitchCard = allCards.GetVector()[1];
    test_pos = pole.GetPosition(0,0,1);
    test_pos->SetHero(WitchCard);
    mapper.MapPowers(test_pos->GetHero());
    Position* WitchPos = pole.GetPosition(0,0,1);

    std::cout << "Здоровье ведьмы " << WitchPos->GetHero().GetCurHealth() << std::endl;
    int BeforeHP = WitchPos->GetHero().GetCurHealth();
    heroes.clear();
    heroes.resize(0);
    heroes.push_back(&(WitchPos->GetHero()));
    heroes.push_back(&(OraclePos->GetHero()));
    WitchPos->GetHero().frontLinePower(heroes);
    std::cout << "Здоровье ведьмы " << WitchPos->GetHero().GetCurHealth() << std::endl;
    EXPECT_EQ(BeforeHP+1, WitchPos->GetHero().GetCurHealth());

    Card PriestCard = allCards.GetVector()[6];
    test_pos = pole.GetPosition(2,0,1);
    test_pos->SetHero(PriestCard);
    mapper.MapPowers(test_pos->GetHero());
    Position* PriestPos = pole.GetPosition(2,0,1);

    pole.Show();

    std::vector<bool> HeroesDead;
    HeroesDead.resize(0);
    for (int i = 0; i < pole.GetVector().size(); i++) {
        if ((pole.GetVector()[i]->GetHero().IsDead()) && !(pole.GetVector()[i]->isEmpty())) {
            HeroesDead.push_back(true);
        } else {
            HeroesDead.push_back(false);
        }
    }

    std::vector<bool> ShouldBeDead = {false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false};

    for(int i = 0; i < ShouldBeDead.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_TRUE(ShouldBeDead[i] == HeroesDead[i]);
    }

    heroes.clear();
    heroes.resize(0);
    heroes.push_back(&(PriestPos->GetHero()));
    heroes.push_back(&(OraclePos->GetHero()));
    std::cout << OraclePos->GetHero().GetCurHealth() << " " << OraclePos->GetHero().GetCurStrength() << std::endl;
    PriestPos->GetHero().frontLinePower(heroes);
    EXPECT_FALSE(OraclePos->GetHero().IsDead());
    EXPECT_FALSE(OraclePos->isEmpty());
    std::cout << OraclePos->GetHero().GetCurHealth() << " " << OraclePos->GetHero().GetCurStrength() << std::endl;

    HeroesDead.resize(0);
    for (int i = 0; i < pole.GetVector().size(); i++) {
        if ((pole.GetVector()[i]->GetHero().IsDead()) && !(pole.GetVector()[i]->isEmpty())) {
            HeroesDead.push_back(true);
        } else {
            HeroesDead.push_back(false);
        }
    }

    EXPECT_FALSE(ShouldBeDead[10] == HeroesDead[10]);
}

