cmake_minimum_required(VERSION 3.0.0)
project(minRSA VERSION 0.1.0)

include(CTest)
enable_testing()

list(APPEND INCLUDE_PATH include)

list(APPEND SOURCE_FILE source/bignum.c)
list(APPEND SOURCE_FILE source/minRSA.c)
list(APPEND SOURCE_FILE source/main.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_PATH})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
