//
// Created by luka on 30.1.21..
//

#ifndef RG_TARGETSHOOTING_SHADER_HPP
#define RG_TARGETSHOOTING_SHADER_HPP

#include "glad/glad.h"
#include <string>
#include <sstream>
#include <fstream>
#include "Error.hpp"

std::string readFileContents(std::string path)
{
    //otvaramo fajl na zadatoj putanji
    std::ifstream in(path);
    std::stringstream buffer;
    std::string line;

    if(in.is_open()) {
        //  buffer << in.rdbuf();
        while (std::getline(in, line))
            buffer << line << std::endl;
    }
    else{
        ASSERT(false,"Failed to open file!");
    }

    in.close();
    return buffer.str();
}

class Shader{
public:
    //konstruktor prima putanju do shadera
    Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
    {
        std::string vsString = readFileContents(vertexShaderPath);
        ASSERT(!vsString.empty(),"Vertex shader is empty!");
        const char* vertexShaderSource = vsString.c_str();
        unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        int successs = 0;
        char infoLog[512];
        glad_glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&successs);
        if(!successs)
        {
            glGetShaderInfoLog(vertexShader,512, nullptr,infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl
                      <<infoLog << std::endl;
        }

        std::string fsString = readFileContents(fragmentShaderPath);
        ASSERT(!fsString.empty(),"Fragment shader is empty!");
        const char* fragmentShaderSource = fsString.c_str();
        unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glad_glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&successs);
        if(!successs)
        {
            glGetShaderInfoLog(fragmentShader,512, nullptr,infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl
                      <<infoLog << std::endl;
        }

        unsigned shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram,vertexShader);
        glAttachShader(shaderProgram,fragmentShader);
        glLinkProgram(shaderProgram);
        glad_glGetProgramiv(shaderProgram,GL_LINK_STATUS,&successs);
        if(!successs)
        {
            glad_glGetProgramInfoLog(shaderProgram,512, nullptr,infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
        }

        //brisemo resurse jer je program preveden i linkovan
        glDeleteProgram(vertexShader);
        glDeleteProgram(fragmentShader);

        m_Id = shaderProgram;
    }
    void  use()
    {
        ASSERT(m_Id > 0 , "Use of undefined or delete shader program!");
        glUseProgram(m_Id);
    }

    void setUniform4f(std::string name, float x, float y, float z, float w)
    {
        int uniform_id = glGetUniformLocation(m_Id,name.c_str());
        glUniform4f(uniform_id, x, y, z, w);
    }
    void setUniform1i(std::string name , int value)
    {
        int uniform_id = glGetUniformLocation(m_Id,name.c_str());
        glUniform1i(uniform_id,value);
    }
    void deleteProgram()
    {
        glDeleteProgram(m_Id);
        m_Id = -1;
    }

private:
    //id_od_Shader_programa
    int m_Id;
};

#endif //RG_TARGETSHOOTING_SHADER_HPP
