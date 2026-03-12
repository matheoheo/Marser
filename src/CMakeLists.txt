#==================
#MarserLib
#==================

add_library(MarserLib
    encryption/EncryptionRegistry.cpp
    encryption/IEncryptionAlgorithm.cpp
    encryption/KeyDerivation.cpp
    encryption/KeyVault.cpp
    encryption/ShiftEncryption.cpp
    encryption/XorEncryption.cpp
    io/FileLoader.cpp
    io/FilePacker.cpp
    io/MattFile.cpp
    parser/Parser.cpp
    parser/Tokenizer.cpp
    parser/Value.cpp
    utils/Crc32.cpp)

target_compile_features(MarserLib PUBLIC cxx_std_20)
target_include_directories(MarserLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MarserLib PUBLIC 
    matt::logger 
    matt::random)