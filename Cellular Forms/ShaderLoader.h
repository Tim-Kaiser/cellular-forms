#pragma once

#include <string>
#include "include/glad/gl.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <memory> 


typedef struct
{
	GLuint m_shaderProgramID;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;

} Shader;

class ShaderLoader {

private:
	GLuint m_shaderProgramID;

public:
	ShaderLoader();

	GLuint GetShaderProgramID();

	std::unique_ptr<Shader> CreateShaders();

	bool CompileShaders(const std::string& filename, GLuint& shaderID);
	void AttachShaders(Shader& shader);
	bool LinkProgram(Shader& shader);

	void DetachShaders(Shader& shader);
	void DestroyShaders(Shader& shader);
	void DestroyProgram(Shader& shader);

	GLint GetUniformID(const std::string& uniformName);

	bool SendUniformData(const std::string& uniformName, GLint& data);
	bool SendUniformData(const std::string& uniformName, GLfloat& data);
	bool SendUniformData(const std::string& uniformName, GLuint& data);

	bool SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y);
	bool SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y, GLfloat& z);
	bool SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y, GLfloat& z, GLfloat& w);

	bool SendUniformData(const std::string& uniformName, const glm::mat4& data);
	bool SendUniformData(const std::string& uniformName, const glm::vec3& data);
};