#include "MySQL.h"

MySQL::MySQL() {

    ConnectionDetails details("localhost", "root", "123456", "PixelTactics");
    connection = new Connection(details);
    connection->MySqlConnectionSetup();

    mapper = new CardsMapper();
}

vector<Card> MySQL::GetCards() {

    MYSQL_ROW row;
    MYSQL_RES *res;
    res = connection->ExecuteQuery("select * from Cards");

    vector<Card> cards;

    while ((row = mysql_fetch_row(res)) != nullptr) {
        Card card;
        card.ID = atoi(row[0]);
        card.name = row[1];
        card.HP = atoi(row[2]);
        card.strength = atoi(row[3]);
        mapper->MapCard(card);

        /* string check = "";
        (card.frontLinePower)(check);
        (card.middleLinePower)(check);
        (card.backLinePower)(check);
        (card.leaderPower)(check); */

        cards.push_back(card);
    }

    return cards;
}

