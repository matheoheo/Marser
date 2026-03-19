#==================
#MarserLib
#==================

add_library(MarserLib
    matt/encryption/EncryptionRegistry.cpp
    matt/encryption/IEncryptionAlgorithm.cpp
    matt/encryption/KeyDerivation.cpp
    matt/encryption/KeyVault.cpp
    matt/encryption/ShiftEncryption.cpp
    matt/encryption/XorEncryption.cpp
    matt/io/FileLoader.cpp
    matt/io/FilePacker.cpp
    matt/io/MattFile.cpp
    matt/parser/Parser.cpp
    matt/parser/Tokenizer.cpp
    matt/parser/Value.cpp
    matt/utils/Crc32.cpp)

target_compile_features(MarserLib PUBLIC cxx_std_20)
target_include_directories(MarserLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MarserLib PUBLIC 
    matt::logger 
    matt::random)