cmake_minimum_required(VERSION 2.8.4)
project(proj1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    practice/fileopen.c
    practice/forkprac.c
    practice/itChar.c
    practice/parse.c
    practice/redir.c
    practice/strprac.c
    practice/strtokOnlyOnce.c
    kash.c)

add_executable(proj1 ${SOURCE_FILES})