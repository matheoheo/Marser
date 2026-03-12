enable_testing()

add_executable(MarserTests
    MattFileTests.cpp
    TokenizerTests.cpp
    ParserTests.cpp
    EncryptionTests.cpp)

target_link_libraries(MarserTests
    MarserLib
    gtest_main)
    
include(GoogleTest)

gtest_discover_tests(MarserTests)
