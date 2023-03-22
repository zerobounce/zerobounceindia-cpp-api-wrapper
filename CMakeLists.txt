cmake_minimum_required(VERSION 3.20)
project(ZeroBounce VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

include(FetchContent)

# Download and build libcpr
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
# Download and build nlohmann_json
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)

FetchContent_MakeAvailable(cpr nlohmann_json)

target_link_libraries(${PROJECT_NAME} PUBLIC cpr::cpr nlohmann_json::nlohmann_json)

add_executable(zb_test main.cpp)
target_link_libraries(zb_test PRIVATE ${PROJECT_NAME})