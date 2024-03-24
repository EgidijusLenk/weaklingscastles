# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.12)
project(weaklingscastles)

# Create a variable containing all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add your project executable and include the source files
add_executable(weaklingscastles ${SOURCES})

# Add include directories
target_include_directories(weaklingscastles PRIVATE "${CMAKE_SOURCE_DIR}/include")

find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(weaklingscastles PRIVATE ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost library not found.")
endif()

# Specify the path to the SFMLConfig.cmake file
set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.0/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics REQUIRED)
target_include_directories(weaklingscastles PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(weaklingscastles PRIVATE sfml-graphics)
