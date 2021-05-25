#include "MySQL.h"

MySQL::MySQL() {

    ConnectionDetails details("localhost", "root", "123456", "PixelTactics");
    connection = make_unique<Connection>(details);
    connection->MySqlConnectionSetup();
}

Deck MySQL::GetCards() {

    MYSQL_ROW row;
    MYSQL_RES *res;
    res = connection->ExecuteQuery("select * from Cards");

    Deck cards;

    while ((row = mysql_fetch_row(res)) != nullptr) {
        Card card;
        card.texture = atoi(row[0]);
        card.name = row[1];
        card.HP = atoi(row[2]);
        card.strength = atoi(row[3]);
        card.frontLinePower = row[4];
        card.middleLinePower = row[5];
        card.backLinePower = row[6];
        cards.push_back(card);
    }

    return cards;
}

