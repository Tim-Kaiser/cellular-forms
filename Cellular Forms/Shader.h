#pragma once

#include <string>
#include "include/glad/gl.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>


class Shader {

private:
	Shader();
	Shader(const Shader&);
	Shader& operator=(Shader&);

	GLuint m_shaderProgramID;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;


public:
	enum class ShaderType { VERTEX_SHADER, FRAGMENT_SHADER };

	static Shader* Instance();

	GLuint GetShaderProgramID();

	bool CreateProgram();
	bool CreateShaders();

	bool CompileShaders(const std::string& filename, ShaderType shaderType);
	void AttachShaders();
	bool LinkProgram();

	void DetachShaders();
	void DestroyShaders();
	void DestroyProgram();

	GLint GetUniformID(const std::string& uniformName);

	bool SendUniformData(const std::string& uniformName, GLint data);
	bool SendUniformData(const std::string& uniformName, GLfloat data);
	bool SendUniformData(const std::string& uniformName, GLuint data);

	bool SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y);
	bool SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y, GLfloat z);
	bool SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	bool SendUniformData(const std::string& uniformName, const glm::mat4& data);
	bool SendUniformData(const std::string& uniformName, const glm::vec3 data);


};