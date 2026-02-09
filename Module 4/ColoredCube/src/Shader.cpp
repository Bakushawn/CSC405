#include "Shader.h"

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

Shader::Shader(const char* vertexPath, const char* fragmentPath){
    // 1. retrieve the source code from filepaths
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vFile;
    std::ifstream fFile;
    // Allowing for exceptions to be thrown
    vFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try{
        // opening the shaderCode
        vFile.open(vertexPath);
        fFile.open(fragmentPath);
        
        // for transfering filestream into string
        std::stringstream vStream, fStream;

        // reading files
        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();

        // closing files
        vFile.close();
        fFile.close();

        vertexCode = vStream.str();
        fragmentCode = fStream.str();
    }
    catch(std::ifstream::failure e){
        std::cout << "ERROR: SHADER FILES WERE NOT READ" << std::endl;
    }

    // converting shader code into c-style string
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    // Creating and compiling vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    // Send Error if failed
    if(!success){
        glGetShaderInfoLog(vertexShader,512, NULL, infoLog);
        std::cout << "ERROR: VERTEX SHADER FAILED TO COMPILE\n" << infoLog << std::endl;
    }

    // creating and compiling fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    // Send Error if failed
    if(!success){
        glGetShaderInfoLog(fragmentShader,512, NULL, infoLog);
        std::cout << "ERROR: FRAGMENT SHADER FAILED TO COMPILE\n" << infoLog << std::endl;
    }

    // creating the shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // looking for linker errors
    glGetProgramiv(ID,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR: THE LINKING OF THE SHADER PROGAM HAS FAILED\n" << infoLog << std::endl;
    }

    // deleting shaders to be responsible
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::activate(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}   

void Shader::setFloat(const std::string &name, float value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}