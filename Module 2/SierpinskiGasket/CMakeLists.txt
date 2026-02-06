cmake_minimum_required(VERSION 4.2)

project(SierpinskiGasket
    VERSION 1.0
)

# Fetching GLFW
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw GIT_TAG 3.3.8)
FetchContent_MakeAvailable(glfw)


# executables
add_executable(SierpinskiGasket src/SierpinskiGasket.cpp src/glad.c)

# linking libraries
target_link_libraries(SierpinskiGasket glfw opengl32 gdi32 user32 shell32)
target_include_directories(SierpinskiGasket PRIVATE lib/glad/include)

enable_testing()
add_test(NAME VisualTesting COMMAND SierpinskiGasket --test)
