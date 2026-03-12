cmake_minimum_required(VERSION 3.15...4.0)
project(Marser VERSION 1.0
               DESCRIPTION "Matt Parser"
               LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(dependencies)
add_subdirectory(src)
add_subdirectory(tests)

add_executable(MarserMain src/main.cpp)
target_link_libraries(MarserMain MarserLib)

add_custom_command(
    TARGET MarserMain 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/structureFile.txt"
    "$<TARGET_FILE_DIR:MarserMain>/structureFile.txt"
    COMMENT "Copying structureFile.txt to executable directory")

