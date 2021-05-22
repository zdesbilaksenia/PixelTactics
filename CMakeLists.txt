cmake_minimum_required(VERSION 3.1)
project(PixelTactics)

find_package(SFML 2 COMPONENTS network audio graphics window system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-lmysqlclient")
set(LIB_SOURCE_FILES 
    src/DrawableBox.cpp 
    src/Clickable.cpp 
    src/Command.cpp 
    src/Button.cpp 
    src/Card.cpp 
    src/Label.cpp 
    src/Tile.cpp 
    src/Unit.cpp 
    src/Commands/CommandAttack.cpp 
    src/Commands/CommandTakeCard.cpp)

    include_directories(include)

include_directories(lib/)
include_directories(DataBase/ Mapper/ /usr/include/mysql /usr/mysql /usr/lib/mysql)
add_library(DataBase_lib src/newmain.cpp DataBase/Connection.cpp DataBase/MySQL.cpp Mapper/PowerMapper.cpp)
include_directories(Request/)

add_library(position src/position.cpp)
add_library(deck src/deck.cpp)
add_library(hero src/hero.cpp)
add_library(pole src/pole.cpp)
add_library(player src/player.cpp)
add_library(study_std_lib STATIC ${LIB_SOURCE_FILES})
add_library(request Request/request.cpp)


add_executable(main src/newmain.cpp DataBase/Connection.cpp src/hero.cpp Mapper/PowerMapper.cpp)
add_executable(prog src/main.cpp)

target_link_libraries(main position)
target_link_libraries(main pole)
target_link_libraries(main deck)
target_link_libraries(main hero)
target_link_libraries(main player)
target_link_libraries(main DataBase_lib mysqlclient)
target_link_libraries(prog study_std_lib)
target_link_libraries(main request)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(prog sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
endif()

