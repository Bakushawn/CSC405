#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// function defintion
void constructSierpinshi(int iterations, float vertices[]);

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   gl_PointSize =   5.0;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(0.21f, 0.0f, 0.25f, 1.0f);\n"
    "}\0"; 

int main(void)
{
    // seeding random function
    srand(static_cast<unsigned int>(time(0)));

    /* creating GLFW window*/
    // initialize GLFW
    GLFWwindow* window;

    // get out here if GLFW don't do
    if (!glfwInit())
        return -1;

    // create window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Damn bro. GLFW didn't work" << std::endl;
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

    int iterations = 10000;
    float vertices[iterations * 3];
    constructSierpinshi(iterations, vertices);

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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, iterations);

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

void constructSierpinshi(int iterations, float vertices[]){
    // picking an actual random initial point is hard so intial point is 0,0 for now
    float Px = 0.0f;
    float Py = 0.0f;
    
    // bounding triangle vertices
    float v1x = -0.5f;
    float v1y = -0.5f;
    float v2x =  0.0f;
    float v2y =  0.5f;
    float v3x =  0.5f;
    float v3y = -0.5f;
    
    // shoving bounding triangle in first three of buffer
    vertices[0] = v1x;
    vertices[1] = v1y;
    vertices[2] = 0.0f; //z-coord
    vertices[3] = v2x;
    vertices[4] = v2y;
    vertices[5] = 0.0f; //z-coord
    vertices[6] = v3x;
    vertices[7] = v3y;
    vertices[8] = 0.0f; //z-coord

    int vertex = 0;
    int offset = 9; // offset to jump past bounding triangle
    for(int i = 0; i < iterations; i++){
        // choose random vertex of bounding triangle
        vertex = (rand() % 3) + 1;
        switch(vertex){
            // find midpoint between working point and randomly chosen vertex
            case 2:
                Px = (Px + v2x)/2;
                Py = (Py + v2y)/2;
            break;
            case 3:
                Px = (Px + v3x)/2;
                Py = (Py + v3y)/2;
            break;
            default:
                Px = (Px + v1x)/2;
                Py = (Py + v1y)/2;
        }
        
        // mark midpoint
        vertices[(i * 3) + offset] = Px;
        vertices[(i * 3) + 1 + offset] = Py;
        vertices[(i * 3) + 2 + offset] = 0.0f; // staying 2D so z axis needs to be zero
    }


}