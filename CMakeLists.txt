cmake_minimum_required(VERSION 3.16)
project(Network)

set(CMAKE_CXX_STANDARD 17)

find_library(PTHREAD_LIBRARY pthread)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
#FIND_PACKAGE(Boost 1.75.0 COMPONENTS system thread regex REQUIRED)
FIND_PACKAGE(Boost 1.75.0 REQUIRED)
#IF(Boost_FOUND)
#  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
#  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
#ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

file(GLOB SOURCES src/main.cpp)
include_directories(lib/)
add_library(Network_lib ${SOURCES} ${PTHREAD_LIBRARY} ${USED_LIBS})

add_executable(prog ${SOURCES})
target_link_libraries(prog Network_lib ${PTHREAD_LIBRARY} ${USED_LIBS})

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})

add_executable(tests tests/main.cpp tests/tests.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} ${PTHREAD_LIBRARY} ${USED_LIBS})

