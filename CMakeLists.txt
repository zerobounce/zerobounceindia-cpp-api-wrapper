cmake_minimum_required(VERSION 3.20)
project(ZeroBounce VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

include(FetchContent)

# Download and build libcurl
FetchContent_Declare(
    libcurl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_0_1
)
# Download and build nlohmann_json
FetchContent_Declare(
    nlohmann_json 
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
# Download and build cpputest
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG v4.0
)

FetchContent_MakeAvailable(libcurl nlohmann_json CppUTest)

target_include_directories(${PROJECT_NAME} PUBLIC ${libcurl_SOURCE_DIR}/include)
link_directories(${libcurl_BINARY_DIR}/lib)

target_link_libraries(${PROJECT_NAME} PUBLIC libcurl nlohmann_json::nlohmann_json)

add_executable(zb_test main.cpp)
target_link_libraries(zb_test PRIVATE ${PROJECT_NAME})

add_executable(tests test/ZeroBounceTest.cpp)
target_link_libraries(tests PRIVATE CppUTest CppUTestExt ${PROJECT_NAME})