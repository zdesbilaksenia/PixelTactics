cmake_minimum_required(VERSION 3.1)
project(PixelTactics)

find_library(PTHREAD_LIBRARY pthread)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.75.0 COMPONENTS system thread regex REQUIRED)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${PTHREAD_LIBRARY})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-lmysqlclient")


include_directories(lib/)
include_directories(DataBase/ Mapper/ /usr/include/mysql /usr/mysql /usr/lib/mysql)
add_library(DataBase_lib DataBase/Connection.cpp DataBase/MySQL.cpp Mapper/PowerMapper.cpp)
include_directories(Request/)

add_library(game_libs STATIC 
    src/Game.cpp 
    src/position.cpp
    src/deck.cpp
    src/hero.cpp
    src/pole.cpp
    src/player.cpp)
add_library(request Request/request.cpp)
add_library(powers Mapper/PowerMapper.cpp src/Powers.cpp)
add_library(network STATIC
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
add_executable(cl src/client.cpp)
add_executable(serv src/server.cpp)

target_link_libraries(main game_libs)
target_link_libraries(main DataBase_lib mysqlclient)
target_link_libraries(main request)
target_link_libraries(main powers)
target_link_libraries(cl PUBLIC network ${USED_LIBS})
target_link_libraries(serv PUBLIC network game_libs DataBase_lib mysqlclient request powers ${USED_LIBS})


enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})

add_executable(tests  Tests/Abilities.cpp Tests/MyTests.cpp Tests/main.cpp)

target_link_libraries(tests ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(tests game_libs)
target_link_libraries(tests DataBase_lib mysqlclient)
target_link_libraries(tests request)
target_link_libraries(tests powers)




