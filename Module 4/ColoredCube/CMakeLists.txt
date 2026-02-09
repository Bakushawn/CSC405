cmake_minimum_required(VERSION 4.2)

project(ColoredCube
    VERSION 1.0
)

# Fetching GLFW
include(FetchContent)
FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw 
    GIT_TAG 3.3.8)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)


# executables
add_executable(ColoredCube src/ColoredCube.cpp src/glad.c src/Shader.cpp)

# linking libraries
target_link_libraries(ColoredCube glm glfw opengl32 gdi32 user32 shell32)
target_include_directories(ColoredCube 
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
    PRIVATE 
        lib/glad/include/
        )

enable_testing()
add_test(NAME VisualTesting COMMAND ColoredCube --test)
