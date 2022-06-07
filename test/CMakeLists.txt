cmake_minimum_required(VERSION 3.5.1)
project(crossguid-tests
    LANGUAGES C CXX
)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestMain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Test.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        crossguid::crossguid
)
