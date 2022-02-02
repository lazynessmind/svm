cmake_minimum_required(VERSION 3.21)
project(svm C)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O1 -Wall -Wno-missing-braces")

add_executable(svm main.c)
