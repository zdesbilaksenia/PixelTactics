cmake_minimum_required(VERSION 3.1)
project(PixelTactics)

FIND_PACKAGE(Boost 1.75.0 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-lmysqlclient")


include_directories(lib/)
include_directories(DataBase/ Mapper/ /usr/include/mysql /usr/mysql /usr/lib/mysql)
add_library(DataBase_lib src/newmain.cpp DataBase/Connection.cpp DataBase/MySQL.cpp Mapper/PowerMapper.cpp)


add_library(position src/position.cpp)
add_library(deck src/deck.cpp)
add_library(hero src/hero.cpp)
add_library(pole src/pole.cpp)
add_library(player src/player.cpp)


add_executable(main src/newmain.cpp DataBase/Connection.cpp src/Powers.cpp src/hero.cpp Mapper/PowerMapper.cpp)

target_link_libraries(main position)
target_link_libraries(main pole)
target_link_libraries(main deck)
target_link_libraries(main hero)
target_link_libraries(main player)
target_link_libraries(main DataBase_lib mysqlclient)

