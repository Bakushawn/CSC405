#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>


#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"


// function defintions
std::string importShader(const std::string& fileName);

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;
const std::string PROJECT_DIRECTORY = std::filesystem::current_path().string();

int main(void)
{
    srand(static_cast<unsigned int>(time(0)));

    /* creating GLFW window*/
    // initialize GLFW
    GLFWwindow* window;

    // Ensure GLFW exists
    if (!glfwInit())
        return -1;

    // create window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "ERROR: GLFW didn't create a window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // make current context window
    glfwMakeContextCurrent(window);

    // Initialize GLAD (Must happen AFTER context is current)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::string VertexPath = PROJECT_DIRECTORY + "\\shaders\\Vertex.vert";
    std::string FragmentPath = PROJECT_DIRECTORY + "\\shaders\\Fragment.frag";
    Shader CubeShader(VertexPath.c_str(),FragmentPath.c_str()); //takes in c-style strings

    /* building and creating buffers*/

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // create a Vertex Buffer Object and Vertex Attribute Object to send to the GPU
    unsigned int VBO, VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);

    // bind VAO 
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // specify how Vertex buffer data is formatted (32bit, positions have 3 values, and tightly packed)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // enabling point size to be changed by vertex renderer
    glEnable(GL_PROGRAM_POINT_SIZE);

    /* rendering time baby!*/
    while (!glfwWindowShouldClose(window))
    {
        // Black Background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader
        CubeShader.activate();

        // Draw our triangle
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        float redValue = (cos(timeValue) / 2.0f) + 0.5f;

        int vertexColorLocation = glGetUniformLocation(CubeShader.ID, "ourColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_POINTS, 0, 3);
        glDrawArrays(GL_TRIANGLES,0,3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // terminate the window
    glfwTerminate();
    return 0;
}