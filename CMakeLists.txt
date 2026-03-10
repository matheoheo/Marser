cmake_minimum_required(VERSION 3.15...4.0)
project(Marser VERSION 1.0
               DESCRIPTION "Matt Parser"
               LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#==================
#MarserLib
#==================

add_library(MarserLib
    src/encryption/EncryptionRegistry.cpp
    src/encryption/IEncryptionAlgorithm.cpp
    src/encryption/KeyDerivation.cpp
    src/encryption/KeyVault.cpp
    src/encryption/ShiftEncryption.cpp
    src/encryption/XorEncryption.cpp
    src/io/FileLoader.cpp
    src/io/FilePacker.cpp
    src/io/MattFile.cpp
    src/logger/Logger.cpp
    src/parser/Parser.cpp
    src/parser/Tokenizer.cpp
    src/parser/Value.cpp
    src/utils/Crc32.cpp)

target_compile_features(MarserLib PUBLIC cxx_std_20)
target_include_directories(MarserLib PUBLIC src)

add_executable(MarserMain src/main.cpp)
target_link_libraries(MarserMain MarserLib)

#==================
#GoogleTests
#==================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(MarserTests
    tests/MattFileTests.cpp
    tests/TokenizerTests.cpp
    tests/ParserTests.cpp
    tests/EncryptionTests.cpp)

target_link_libraries(
    MarserTests
    MarserLib
    gtest_main
    gtest)
    
include(GoogleTest)

gtest_discover_tests(MarserTests)
