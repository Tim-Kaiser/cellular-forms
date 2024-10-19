#include "Quad.h"
#include "Shader.h"
#include <gtc/matrix_transform.hpp>

Quad::Quad(int width, int height)
{
	GLfloat vertices[] = {
	-1.0f,  1.0f, 0.0f, // triangle 1
	 1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,

	-1.0f, -1.0f, 0.0f, // triangle 2
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
	};

	GLfloat uvs[] = {
		0.0f, 1.0f, // triangle 1
		1.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f, // triangle 2
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	m_buffer.CreateBuffer(6);
	m_buffer.FillVBO(Buffer::VERTEX_BUFFER, vertices, sizeof(vertices), Buffer::SINGLE);
	m_buffer.FillVBO(Buffer::TEXTURE_BUFFER, uvs, sizeof(uvs), Buffer::SINGLE);


	m_buffer.LinkBuffer("vertex", Buffer::VERTEX_BUFFER, Buffer::XYZ, Buffer::FLOAT);
	m_buffer.LinkBuffer("uv", Buffer::TEXTURE_BUFFER, Buffer::UV, Buffer::FLOAT);
	m_position = glm::vec3(0.0f);
}

Quad::~Quad()
{
	m_buffer.DestroyBuffer();
}

void Quad::Render()
{
	m_buffer.Draw(Buffer::TRIANGLES);
}
