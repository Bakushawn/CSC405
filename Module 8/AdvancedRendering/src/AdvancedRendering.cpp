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

#include "stb_image.h"

#include "Shader.h"


// function definitions
std::string importShader(const std::string& fileName);
void processInput(GLFWwindow *window, glm::vec3 &cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float deltaTime);

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;
const unsigned int STRIDE = 5;
const unsigned int VERTEX_CHANNELS = 3;

// delta time
float deltaTime = 0.0f;
float lastframe = 0.0f;

const std::string PROJECT_DIRECTORY = std::filesystem::current_path().string();
//const std::string PROJECT_DIRECTORY = "C:\\Users\\Shawn\\OneDrive\\Documents\\CSUGlobal\\CSC405\\Module 5\\InteractiveViewer" ;

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


    // texture 
    float textureCoords[] = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };
    // building a cube
    float vertices[] = {
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // vertex (x,y,z), texture (x,y)
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // left face
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

        // right face
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

         // bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    // parameters: index of 0, Take off alpha channel, datatype, Stride, pointer
    // Pointing to positions
    glVertexAttribPointer(0, VERTEX_CHANNELS, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // pointing to coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // enabling point size to be changed by vertex renderer
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Loading and creating textures

    unsigned int texture1;

    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image and create the texture + mipmap
    int textureWidth, textureHeight, nrChannels;
    std::string texture1Path = PROJECT_DIRECTORY + "\\shaders\\Textures\\StarshipTroopers.jpg";
    stbi_set_flip_vertically_on_load(true); // flip the image
    unsigned char *imageData = stbi_load(texture1Path.c_str(), &textureWidth, &textureHeight, &nrChannels, 0);
    

    // check for loaded
    if(imageData){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else{
        std::cout << "ERROR: Texture was not loaded" << std::endl;
    }
    // free data we do not need anymore
    stbi_image_free(imageData);

    // passing texture into shaders
    CubeShader.activate();
    CubeShader.setInt("texture1", 0);

    // passing projection into shaders
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    CubeShader.setMat4("projection", projection);

    // defining camera
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  5.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);


    /* rendering time baby!*/
    while (!glfwWindowShouldClose(window))
    {
        // calculating delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastframe;
        lastframe = currentFrame;
        // input
        processInput(window, cameraPos, cameraFront, cameraUp , deltaTime);

        // Black Background
        glClearColor(0.0f, 0.12f, 0.23f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        CubeShader.activate();
        
        //----------------render the box

        // creating transformations
        glm::mat4 model          = glm::mat4(1.0f); // identity matrix
        glm::mat4 view           = glm::mat4(1.0f);
        
        // Camera definition (spinning in a circle)
        float radius = 10.f;
        float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        float camZ = static_cast<float>(cos(glfwGetTime()) * radius);

        view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp);

        CubeShader.setMat4("view", view);

        // render the box

        CubeShader.setMat4("model", model);

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

void processInput(GLFWwindow *window, glm::vec3 &cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float deltaTime)
{
    const float cameraSpeed = 0.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
        
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraSpeed * cameraFront;
        //cameraPos += cameraSpeed * cameraUp;
    }
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraSpeed * cameraFront;
        //cameraPos -= cameraSpeed * cameraUp;
    }
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
        
}