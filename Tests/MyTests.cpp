#include "gtest/gtest.h"
#include "Game.h"

TEST(CanBeAttackedMeleeRequest,Test_1){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;
    Position* test_pos = pole.GetPosition(1,1,1);
    
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {false,false,false,false,true,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        ASSERT_TRUE(ShouldBe[i] == result[i]);
    }

}

TEST(CanBeAttackedMeleeRequest,Test_2){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;

    Position* test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    mapper.MapPowers(test_pos->GetHero());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    pole.Show();

    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {false,true,false,false,false,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        EXPECT_EQ(ShouldBe[i],result[i]);
    }

}


TEST(CanBeAttackedMeleeRequest,Test_3){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;

    Position* test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    pole.Show();

    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {false,true,true,false,false,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_EQ(ShouldBe[i],result[i]);
    }

}

TEST(CanBeAttackedMeleeRequest,Test_4){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;

    Position* test_pos = pole.GetPosition(0,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    pole.Show();

    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {true,false,false,false,true,false,false,false,true};
    for(int i = 0; i < ShouldBe.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_EQ(ShouldBe[i],result[i]);
    }

}

TEST(CanBeAttackedMeleeRequest,Test_5){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;

    Position* test_pos = pole.GetPosition(0,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    
    test_pos = pole.GetPosition(0,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(0,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    pole.Show();

    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {true,true,true,false,false,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_EQ(ShouldBe[i],result[i]);
    }

}

TEST(CanBeAttackedMeleeRequest,Test_6){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;

    Position* test_pos = pole.GetPosition(0,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    
    test_pos = pole.GetPosition(0,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(0,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(2,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    pole.Show();
    Hero* hero_to_die = &(pole.GetPosition(1,0,1)->GetHero());
    hero_to_die->Die();

    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {true,false,true,false,true,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_EQ(ShouldBe[i],result[i]);
    }

}


TEST(CanBeAttackedMeleeRequest,Test_7){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;

    Position* test_pos = pole.GetPosition(0,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    
    test_pos = pole.GetPosition(0,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos->GetHero().Die();

    test_pos = pole.GetPosition(0,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos->GetHero().Die();
    EXPECT_TRUE(pole.GetPosition(1,0,1)->GetHero().IsDead());
    

    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {true,false,false,false,false,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_EQ(ShouldBe[i],result[i]);
    }

}

TEST(CanBeAttackedMeleeRequest,Test_8){
    Pole pole;
    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    PowerMapper mapper;

    Position* test_pos = pole.GetPosition(0,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    test_pos->GetHero().Die();
    
    test_pos = pole.GetPosition(0,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);


    test_pos = pole.GetPosition(0,2,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);

    test_pos = pole.GetPosition(1,0,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    test_pos->GetHero().Die();
    EXPECT_TRUE(pole.GetPosition(1,0,1)->GetHero().IsDead());

    test_pos = pole.GetPosition(1,1,1);
    test_pos->SetHero(FirstPlayer.GetDeck().GetVector().back());
    pole.SetPosition(test_pos);
    

    std::vector<bool> result;
    result = pole.CanBeMeleeAttackedRequest(0);
    EXPECT_EQ(result.size(),9);
    std::vector<bool> ShouldBe = {false,false,false,true,true,false,false,false,false};
    for(int i = 0; i < ShouldBe.size(); i++){
        std::cout << i << " " <<std::endl;
        EXPECT_EQ(ShouldBe[i],result[i]);
    }

}
