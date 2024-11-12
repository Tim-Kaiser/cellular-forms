#pragma once
#include <fstream>
#include <iostream>

#include "ShaderLoader.h"


std::unique_ptr<Shader> ShaderLoader::CreateShaders()
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (vertexShaderID == 0) {
        std::cout << "Error creating vertex shader object" << std::endl;
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (vertexShaderID == 0) {
        std::cout << "Error creating fragment shader object" << std::endl;
    }
    std::unique_ptr<Shader> shader(new Shader);
    shader->m_shaderProgramID = glCreateProgram();
    shader->m_fragmentShaderID = fragmentShaderID;
    shader->m_vertexShaderID = vertexShaderID;

    return shader;
}

bool ShaderLoader::CompileShaders(const std::string& filename, GLuint& shaderID)
{
    std::fstream file;
    std::string shaderCode;
    std::string line;

    file.open(filename);

    if (!file) {
        std::cout << "Error reading shader file: " << filename << std::endl;
        return false;
    }

    while (!file.eof())
    {
        std::getline(file, line);
        shaderCode += line + "\n";
    }
    file.close();

    const GLchar* src = reinterpret_cast<const GLchar*>(shaderCode.c_str());

    // pass in the shader ID (either vertex or fragment), the amount of shaders, the pointer to the source code, and NULL(only relevant for multiple shaders at the same time)
    glShaderSource(shaderID, 1, &src, nullptr);

    glCompileShader(shaderID);

    GLint errorCode;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &errorCode);

    if (errorCode == GL_TRUE) {
        std::cout << "Shader compiled successfully" << std::endl;
    }
    else {
        GLsizei bufferSize = 1000;
        GLchar errorMessage[1000];

        glGetShaderInfoLog(shaderID, bufferSize, &bufferSize, errorMessage);
        std::cout << errorMessage << std::endl;
        return false;
    }
    return true;
}

void ShaderLoader::AttachShaders(Shader& shader)
{
    glAttachShader(shader.m_shaderProgramID, shader.m_fragmentShaderID);
    glAttachShader(shader.m_shaderProgramID, shader.m_vertexShaderID);
}

bool ShaderLoader::LinkProgram(Shader& shader)
{
    glLinkProgram(shader.m_shaderProgramID);
    glUseProgram(shader.m_shaderProgramID);

    GLint errorCode;
    glGetProgramiv(shader.m_shaderProgramID, GL_LINK_STATUS, &errorCode);

    if (errorCode == GL_TRUE) {
        std::cout << "Program linked successfully" << std::endl;
    }
    else {
        GLsizei bufferSize = 1000;
        GLchar errorMessage[1000];

        glGetProgramInfoLog(shader.m_shaderProgramID, bufferSize, &bufferSize, errorMessage);
        std::cout << errorMessage << std::endl;
        return false;
    }

    return true;
}


void ShaderLoader::DetachShaders(Shader& shader)
{
    glDetachShader(shader.m_shaderProgramID, shader.m_vertexShaderID);
    glDetachShader(shader.m_shaderProgramID, shader.m_fragmentShaderID);

}

void ShaderLoader::DestroyShaders(Shader& shader)
{
    glDeleteShader(shader.m_vertexShaderID);
    glDeleteShader(shader.m_fragmentShaderID);
}

void ShaderLoader::DestroyProgram(Shader& shader)
{
    glDeleteProgram(shader.m_shaderProgramID);
}

GLint ShaderLoader::GetUniformID(const std::string& uniformName) {

    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    GLint id = glGetUniformLocation(currentProgram, uniformName.c_str());
    if (id == -1) {
        return -1;
    }
    return id;

}

bool ShaderLoader::SendUniformData(const std::string& uniformName, GLint& data)
{
    GLint id = GetUniformID(uniformName);

    if (id != -1) {
        return false;
    }

    glUniform1i(id, data);
    return true;
}

bool ShaderLoader::SendUniformData(const std::string& uniformName, GLfloat& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform1f(id, data);
    return true;
}

bool ShaderLoader::SendUniformData(const std::string& uniformName, GLuint& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform1ui(id, data);
    return true;
}

bool ShaderLoader::SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform2f(id, x, y);
    return true;
}

bool ShaderLoader::SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y, GLfloat& z)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform3f(id, x, y, z);
    return true;
}

bool ShaderLoader::SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y, GLfloat& z, GLfloat& w)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform4f(id, x, y, z, w);
    return true;
}

bool ShaderLoader::SendUniformData(const std::string& uniformName, const glm::mat4& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniformMatrix4fv(id, 1, GL_FALSE, &data[0][0]);
    return true;
}

bool ShaderLoader::SendUniformData(const std::string& uniformName, const glm::vec3& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform3fv(id, 1, glm::value_ptr(data));
    return true;
}



