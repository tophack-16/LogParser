cmake_minimum_required(VERSION 3.13)
project(player)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include /opt/ncurses/include)

link_directories(/opt/ncurses/lib)

add_executable(player jsoncpp.cpp
        main.cpp
        Player.cpp Player.h
        Card.cpp Card.h
        GemCluster.cpp GemCluster.h
        Board.cpp Board.h
        Table.cpp Table.h
        SplendorBoardParser.cpp SplendorLogParser.cpp Move.cpp Move.h cerrLogParser.cpp frontend.cpp Frontend.cpp Frontend.h)

target_link_libraries(player
        /opt/ncurses/lib/libncurses++.a
        /opt/ncurses/lib/libncurses.a)