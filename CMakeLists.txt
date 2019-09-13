cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME BigInt)

project(${PROJECT_NAME})

include_directories(./include)

set(SRC_LIB bigint.cpp)

#Produce library
add_library(${PROJECT_NAME} STATIC ${SRC_LIB})

