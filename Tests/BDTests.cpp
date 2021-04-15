#include "gtest/gtest.h"

#include "Connection.h"
#include "DataBaseBuilder.h"
#include "DataBaseDirector.h"
#include "MySQL.h"


TEST(DataBaseDirector, BuilderTest) {
    DataBaseBuilder *builder = new DataBaseBuilder();
    DataBaseDirector director;
    director.SetBuilder(builder);
    EXPECT_TRUE(director.IsBuilderSet());

    //delete builder;
}

TEST(DataBaseBuilder, DataBaseBuildTest) {
    //DataBaseBuilder *builder = new DataBaseBuilder();
    //builder->BuildConnection();
    //builder->BuildMySQL();

    //EXPECT_TRUE(builder->GetResult() != nullptr);

    //delete builder;
}

TEST(Connection, ConnectionSetUp) {
    Connection connection({0, 0, 0, 0});
    EXPECT_TRUE(connection.MySqlConnectionSetup());
}

TEST(Connection, ExecuteQuery) {
    Connection connection({0, 0, 0, 0});
    EXPECT_TRUE(connection.ExecuteQuery("select * from smth"));
}

TEST(MySQL, CardsMapping) {
    Connection connection({0, 0, 0, 0});
    MySQL sql(connection);
    EXPECT_TRUE(!sql.GetCards().empty());
}

TEST(Mapper, CardsMapping) {
    Connection connection({0, 0, 0, 0});
    CardsMapper cardsMapper(connection);
    EXPECT_TRUE(!cardsMapper.MapCards().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}