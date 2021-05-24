cmake_minimum_required(VERSION 3.16)
project(PixelTactics CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(PTHREAD_LIBRARY pthread)

include_directories(${PROJECT_SOURCE_DIR}/lib)
find_library(NETWORK_LIB NETWORK_LIB "${PROJECT_SOURCE_DIR}/lib")

SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.75.0 COMPONENTS log REQUIRED)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.75.0 REQUIRED)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${PTHREAD_LIBRARY})

include_directories(lib/)

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

#Скопировал с какого-то сайта
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic")

#Чтобы не перечислять всё, можно написать "src/*.cpp"
set(LIB_SOURCE_FILES 
    src/DrawableBox.cpp 
    src/Clickable.cpp
    src/Command.cpp
    src/Button.cpp
    src/Card.cpp
    src/Tile.cpp
    src/Unit.cpp
    src/Background.cpp
    src/GameManager.cpp
    src/Commands/CommandAttack.cpp
    src/Commands/CommandTakeCard.cpp
    src/Commands/CommandMakeLobby.cpp
    src/Commands/CommandJoinLobby.cpp
    src/Functions.cpp)

# Что бы не было относительных ссылок на файлы
include_directories(include)

add_library(study_std_lib STATIC ${LIB_SOURCE_FILES})

add_executable(prog src/main.cpp)
target_link_libraries(prog study_std_lib network ${USED_LIBS} Boost::log)

find_package(SFML 2 COMPONENTS network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(prog sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
endif()
