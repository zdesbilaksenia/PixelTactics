cmake_minimum_required(VERSION 3.1)
project(PixelTactics)

find_package(SFML 2 COMPONENTS network audio graphics window system)
find_library(PTHREAD_LIBRARY pthread)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
#FIND_PACKAGE(Boost 1.75.0 COMPONENTS system thread regex REQUIRED)
FIND_PACKAGE(Boost 1.75.0 REQUIRED)
#IF(Boost_FOUND)
#  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
#  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
#ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${PTHREAD_LIBRARY})

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
add_library(DataBase_lib DataBase/Connection.cpp DataBase/MySQL.cpp Mapper/PowerMapper.cpp)
include_directories(Request/)

add_library(game src/Game.cpp)
add_library(position src/position.cpp)
add_library(deck src/deck.cpp)
add_library(hero src/hero.cpp)
add_library(pole src/pole.cpp)
add_library(player src/player.cpp)
add_library(study_std_lib STATIC ${LIB_SOURCE_FILES})
add_library(request Request/request.cpp)
add_library(powers Mapper/PowerMapper.cpp src/Powers.cpp)
add_library(NETWORK_LIB STATIC
    lib/Message.cpp
    lib/OwnedMessage.cpp
    lib/TsQueue.cpp
    lib/TcpConnection.cpp
    lib/TcpServer.cpp
    lib/TcpClient.cpp
    lib/Lobby.cpp
    lib/GameTcpClient.cpp
    lib/GameTcpServer.cpp)


add_executable(main src/newmain.cpp DataBase/Connection.cpp src/hero.cpp Mapper/PowerMapper.cpp)
add_executable(prog src/main.cpp)    
add_executable(cl src/client.cpp)
add_executable(serv src/server.cpp)

target_link_libraries(main game)
target_link_libraries(main position)
target_link_libraries(main pole)
target_link_libraries(main deck)
target_link_libraries(main hero)
target_link_libraries(main player)
target_link_libraries(main DataBase_lib mysqlclient)
target_link_libraries(prog study_std_lib)
target_link_libraries(main request)
target_link_libraries(main powers)
target_link_libraries(cl PUBLIC NETWORK_LIB ${USED_LIBS})
target_link_libraries(serv PUBLIC NETWORK_LIB ${USED_LIBS})


if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(prog sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
endif()





