cmake_minimum_required(VERSION 3.16)
project(PixelTactics CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Скопировал с какого-то сайта
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")

set(LIB_SOURCE_FILES src/DrawableBox.cpp src/Clickable.cpp src/Command.cpp src/Button.cpp src/Card.cpp src/Label.cpp src/Tile.cpp src/Unit.cpp src/Commands/CommandAttackTile.cpp src/Commands/CommandSetUnit.cpp)
# Что бы не было относительных ссылок на файлы
include_directories(include)

add_library(study_std_lib STATIC ${LIB_SOURCE_FILES})

add_executable(prog src/main.cpp)
target_link_libraries(prog study_std_lib)

find_package(SFML 2 COMPONENTS network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(prog sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
endif()