cmake_minimum_required(VERSION 3.13)
project(player)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(player jsoncpp.cpp
        main.cpp
        Player.cpp Player.h
        Card.cpp Card.h
        GemCluster.cpp GemCluster.h
        Board.cpp Board.h
        Table.cpp Table.h
        SplendorBoardParser.cpp)