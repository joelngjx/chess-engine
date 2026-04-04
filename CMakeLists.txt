cmake_minimum_required(VERSION 4.0.2)

project(ChessEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(chess 
    main.cpp
    Bitboard.cpp
    Board.cpp
)