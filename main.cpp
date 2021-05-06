#include <iostream>
#include "DataBase/MySQL.h"
#include "Card.h"
#include <vector>

using std::vector;

int main() {

    MySQL sql;
    vector<Card> cards = sql.GetCards();
    return 0;
}
