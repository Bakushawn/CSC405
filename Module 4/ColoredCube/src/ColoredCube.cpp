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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// function defin-tions
std::string importShader(const std::string& fileName);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;
const unsigned int STRIDE = 3;

//const std::string PROJECT_DIRECTORY = std::filesystem::current_path().string();
const std::string PROJECT_DIRECTORY = "C:\\Users\\Shawn\\OneDrive\\Documents\\CSUGlobal\\CSC405\\Module 4\\ColoredCube" ;

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

    // Creating and building shaders
    std::string VertexPath = PROJECT_DIRECTORY + "\\shaders\\Vertex.vert";
    std::string FragmentPath = PROJECT_DIRECTORY + "\\shaders\\Fragment.frag";
    Shader CubeShader(VertexPath.c_str(),FragmentPath.c_str()); //takes in c-style strings

    // enabling depth test
    glEnable(GL_DEPTH_TEST);

    // building a cube
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
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

    // index of 0, Take off alpha channel, datatype, Stride, pointer
    glVertexAttribPointer(0, STRIDE, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // enabling point size to be changed by vertex renderer
    glEnable(GL_PROGRAM_POINT_SIZE);

    CubeShader.activate();

    /* rendering time baby!*/
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        // Black Background
        glClearColor(0.0f, 0.12f, 0.23f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        CubeShader.activate();
        
        //----------------render the box

        // creating transformations
        glm::mat4 model          = glm::mat4(1.0f); // identity matrix
        glm::mat4 projection     = glm::mat4(1.0f);
        glm::mat4 view           = glm::mat4(1.0f);

        // set box color
        glm::vec4 boxColor = glm::vec4(0.35f, 0.0f, 0.5f, 1.0f);
        CubeShader.setVec4("boxColor", boxColor);
        
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.5f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // pass them to the shaders (3 different ways)
        CubeShader.setMat4("model", model);
        CubeShader.setMat4("view", view);
        CubeShader.setMat4("projection", projection);

        // render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * STRIDE));

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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}