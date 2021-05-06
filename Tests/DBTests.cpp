#include "gtest/gtest.h"

#include "../DataBase/Connection.h"
#include "../DataBase/MySQL.h"

TEST(Connection, ConnectionSetUp) {
    ConnectionDetails details = {0,0,0,0};
    Connection connection(details,
                          ConnectionDetails(__cxx11::basic_string(), __cxx11::basic_string(), __cxx11::basic_string(),
                                            __cxx11::basic_string()));
    EXPECT_TRUE(connection.MySqlConnectionSetup());
}

TEST(Connection, ExecuteQuery) {
    ConnectionDetails details = {0,0,0,0};
    Connection connection(details,
                          ConnectionDetails(__cxx11::basic_string(), __cxx11::basic_string(), __cxx11::basic_string(),
                                            __cxx11::basic_string()));
    EXPECT_TRUE(connection.ExecuteQuery("select * from smth"));
}

TEST(MySQL, CardsMapping) {
    ConnectionDetails details = {0,0,0,0};
    Connection connection(details,
                          ConnectionDetails(__cxx11::basic_string(), __cxx11::basic_string(), __cxx11::basic_string(),
                                            __cxx11::basic_string()));
    MySQL sql(connection);
    EXPECT_TRUE(!sql.GetCards().empty());
}

TEST(Mapper, CardsMapping) {
    ConnectionDetails details = {0,0,0,0};
    Connection connection(details,
                          ConnectionDetails(__cxx11::basic_string(), __cxx11::basic_string(), __cxx11::basic_string(),
                                            __cxx11::basic_string()));
    CardsMapper cardsMapper(connection);
    EXPECT_TRUE(!cardsMapper.MapCards().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}