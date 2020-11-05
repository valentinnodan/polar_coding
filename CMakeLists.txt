cmake_minimum_required(VERSION 2.8)
project(PolarCoding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")

include_directories(utils)

add_executable(PolarCoding main.cpp
        coder/PolarCoder.cpp
        coder/PolarCoder.h
        decoder/PolarDecoder.cpp
        decoder/PolarDecoder.h
        construct/PolarCodeConstruct.cpp
        construct/PolarCodeConstruct.h
        utils/polar_utils.h
        utils/Matrix.h)
target_link_libraries(PolarCoding -lpthread)