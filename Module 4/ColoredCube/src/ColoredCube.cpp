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

// function defintions
std::string importShader(const std::string& fileName);

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;

int main(void)
{
    // loading shadercode (OpenGL expects c style strings)
    std::string VertexShaderFileName = std::filesystem::current_path().string() + "\\src\\Vertex.vert";
    std::string fragmentShaderFileName = std::filesystem::current_path().string() + "\\src\\Fragment.frag";

    std::string vss = importShader(VertexShaderFileName);
    std::string fss = importShader(fragmentShaderFileName);

    const char* vertexShaderSource = vss.c_str();
    const char* fragmentShaderSource = fss.c_str();

    // seeding random function
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


    /*------------------- build and compile shaders ---------------------------------------*/
    // create our vertex shader object
    unsigned int vertexShader; // this is the ID of the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach source code to vertex shader and compile shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Status check on our vertex Shader compiler
    int  success; // success code
    char infoLog[512]; // error log buffer
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // actual code to check shader
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
    }


    // create our fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // sanity check on fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // actual code to check shader
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
    }
    // now that we have shaders. it is time to combine them into a program and link them
    // create shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // attach and link vertex and fragment shaders. ORDER MATTERS!!!!
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // sanity check on shader program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER PROGRAM FAILED\n" << infoLog << std::endl;
    }

    // shaders are linked and attached so we can delete them now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  

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

        // Draw our triangle
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
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
    glDeleteProgram(shaderProgram);

    // terminate the window
    glfwTerminate();
    return 0;
}

std::string importShader(const std::string& fileName){
    std::ifstream file;
    std::string shaderCode;

    // Ensure ifstream objects can throw exceptions
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Open file
        file.open(fileName);
        std::stringstream shaderStream;

        // Read file's buffer contents into streams
        shaderStream << file.rdbuf();

        // Close file handler
        file.close();

        // Convert stream into string
        shaderCode = shaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR: SHADERFILE FAILED TO READ: " << fileName << std::endl;
    }
    return shaderCode; 
}