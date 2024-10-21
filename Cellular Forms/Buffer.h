#pragma once

#include "include/glad/gl.h"
#include <string>

class Buffer {
private:
	GLuint m_vertexArrayObj;
	GLuint m_vertexVBO;
	GLuint m_colorVBO;
	GLuint m_normalVBO;
	GLuint m_textureVBO;
	GLuint m_vertexCount;

public:
	enum VBOType { VERTEX_BUFFER, COLOR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER };
	enum ComponentType { XY = 2, XYZ = 3, RGB = 3, RGBA = 4, UV = 2 };
	enum FillType { SINGLE = GL_STATIC_DRAW, MULTIPLE = GL_DYNAMIC_DRAW };
	enum DataType { INT = GL_INT, FLOAT = GL_FLOAT, UINT = GL_UNSIGNED_INT };
	enum DrawType { POINTS = GL_POINTS, TRIANGLES = GL_TRIANGLES, LINES = GL_LINES };

	Buffer();
	void CreateBuffer(GLuint vertexCount);
	void FillVBO(VBOType vboType, GLfloat* data, GLsizeiptr bufferSize, FillType fillType);
	void FillEBO(GLuint* data, GLsizeiptr bufferSize, FillType fillType);
	void LinkBuffer(const std::string& attribute, VBOType vboType, ComponentType componentType, DataType dataType);

	void Draw(DrawType drawType);

	void DestroyBuffer();

};