cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME BigIntTest)

project(${PROJECT_NAME})

include_directories(./../include)

set(SOURCE_FILES main.cpp test_bigint.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#Link libraries
set(LIB_NAMES BigInt)
#BigInt
add_library(BigInt STATIC ./../bigint.cpp)

target_link_libraries(${PROJECT_NAME} ${LIB_NAMES})
