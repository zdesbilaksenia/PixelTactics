#include "pole.h"
#include "../Cards/Card.h"
#include <iostream>
#include "../DataBase/MySQL.h"
#include "../Cards/Powers.h"
#include "../Mapper/PowerMapper.h"
#include <vector>

int main(){
    Pole pole;

    Position FirstPlayerLeader(1,1,0);
    Position SecondPlayerLeader(1,1,1);

    MySQL sql;
    vector<Card> cards = sql.GetCards();
}