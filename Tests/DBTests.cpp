#include "gtest/gtest.h"
#include "MySQL.h"

TEST(Connection, ConnectionCreating) {
    ConnectionDetails details("localhost", "root", "123456", "PixelTactics");
    Connection connection(details);
    EXPECT_TRUE(connection.MySqlConnectionSetup());
}

TEST(Connection, ExecuteQueryCheck) {
    ConnectionDetails details("localhost", "root", "123456", "PixelTactics");
    Connection connection(details);
    MYSQL_ROW row;
    MYSQL_RES *res;
    res = connection.ExecuteQuery("select * from Cards where Name='Ведьма'");
    row = mysql_fetch_row(res);
    EXPECT_EQ(atoi(row[0]), 2);
    EXPECT_EQ(row[1], "Ведьма");
    EXPECT_EQ(atoi(row[2]), 5);
    EXPECT_EQ(atoi(row[3]), 1);
}

TEST(MySQL, GettingCards) {
    MySQL sql;
    vector<Card> cards = sql.GetCards();
    EXPECT_EQ(cards.size(), 1);
    EXPECT_EQ(cards[0].ID, 2);
    EXPECT_EQ(cards[0].name, "Ведьма");
    EXPECT_EQ(cards[0].HP, 5);
    EXPECT_EQ(cards[0].strength, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}