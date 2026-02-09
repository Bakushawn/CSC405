#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);

    void activate();

    void setBool(const std::string &name, bool value) const; // setters modify GPU uniform and not self so they remain const
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    
    void setVec2(const std::string &name, glm::vec2 &vec) const;
    void setVec2(const std::string &name, float x, float y) const;

    void setVec3(const std::string &name, glm::vec3 &vec) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, glm::vec4 &vec) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat2(const std::string &name, glm::mat2 &mat) const;
    void setMat3(const std::string &name, glm::mat3 &mat) const;
    void setMat4(const std::string &name, glm::mat4 &mat) const;



};
#endif