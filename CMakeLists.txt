cmake_minimum_required(VERSION 3.16)
project(DB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lmysqlclient")

file(GLOB SOURCES main.cpp)
include_directories(DataBase/ Mapper/ /usr/include/mysql /usr/mysql)
add_library(DataBase_lib ${SOURCES} DataBase/Connection.cpp DataBase/MySQL.cpp Mapper/PowerMapper.cpp)

include_directories(Cards/)
add_library(Cards_lib ${SOURCES})

include_directories(Heroes/)
add_library(Heroes_lib ${SOURCES})

add_executable(${PROJECT_NAME} main.cpp DataBase/Connection.cpp Cards/Powers.cpp Heroes/Hero.cpp Mapper/PowerMapper.cpp)
target_link_libraries(${PROJECT_NAME} DataBase_lib Cards_lib Heroes_lib mysqlclient)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})

add_executable(DBTests Tests/DBTests.cpp)
target_link_libraries(DBTests ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(DBTests DataBase_lib)

add_executable(HeroesTests Tests/HeroesTests.cpp)
target_link_libraries(HeroesTests ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(HeroesTests Heroes_lib)