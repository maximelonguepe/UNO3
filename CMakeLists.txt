cmake_minimum_required(VERSION 3.15)
project(UNO3 C)

set(CMAKE_C_STANDARD 99)

add_executable(UNO3 main.c testserveur.c testclient.c)