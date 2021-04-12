cmake_minimum_required(VERSION 2.8)
project(PolarCoding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")

include_directories(utils)

add_executable(PolarCoding main.cpp
        coder/PolarCoder.cpp
        coder/PolarCoder.h
        decoder/PolarDecoderSC.cpp
        decoder/PolarDecoderSC.h
        construct/PolarCodeConstruct.cpp
        construct/PolarCodeConstruct.h
        utils/polar_utils.h
        utils/Matrix.h
        utils/Symbol.h
        channel/Channel.cpp
        channel/Channel.h
        decoder/PolarDecoderBP.cpp
        decoder/PolarDecoderBP.h
        ldpc/DecoderBP.cpp
        ldpc/DecoderBP.h
        utils/Transform.h
        ldpc/encode/LDPCCoder.cpp
        ldpc/encode/LDPCCoder.h
        ldpc/LDPCConstruct.h
        ldpc/ldpc_utils.h
        decoder/PolarDecoder.h
        ldpc/DecoderLP.cpp
        ldpc/DecoderLP.h)
target_link_libraries(PolarCoding -lpthread)