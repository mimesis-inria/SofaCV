cmake_minimum_required(VERSION 3.1)

project(SofaCV_test)

set(HEADER_FILES
)

set(SOURCE_FILES
    ImplicitDataEngine_test.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SofaTest SofaGTestMain SofaCV)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
