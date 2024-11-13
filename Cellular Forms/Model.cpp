#include "Model.h"

Mesh Model::createMesh(Object obj, bool isInstanced)
{
	Mesh m;
	m.obj = obj;
	
	glGenVertexArrays(1, &m.VAO);
	glBindVertexArray(m.VAO);

	glGenBuffers(1, &m.vertexVBO);
	glGenBuffers(1, &m.normalVBO);
	glGenBuffers(1, &m.textureVBO);

	// VERTEX
	glBindBuffer(GL_ARRAY_BUFFER, m.vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, obj.vertices.size() * sizeof(GLfloat), &m.obj.vertices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// NORMAL
	glBindBuffer(GL_ARRAY_BUFFER, m.normalVBO);
	glBufferData(GL_ARRAY_BUFFER, obj.normals.size() * sizeof(GLfloat), &m.obj.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


	// TEXTURE
	glBindBuffer(GL_ARRAY_BUFFER, m.textureVBO);
	glBufferData(GL_ARRAY_BUFFER, obj.uvs.size() * sizeof(GLfloat), &m.obj.uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	

	if (isInstanced) 
	{
		glGenBuffers(1, &m.instancedPosVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m.instancedPosVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * INSTANCE_STRIDE * MAX_INSTANCES, NULL, GL_STREAM_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * INSTANCE_STRIDE, nullptr);
		glVertexAttribDivisor(3, 1);

	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return m;
}

Model::Model(Object obj, bool isInstanced):
m_pos(glm::vec3(0)),
m_rot(glm::vec3(0)),
m_isInstanced(isInstanced)
{
	m_mesh = createMesh(obj, isInstanced);
}

Model::~Model()
{
	glDeleteVertexArrays(1, &m_mesh.VAO);
	glDeleteBuffers(1, &m_mesh.vertexVBO);
	glDeleteBuffers(1, &m_mesh.normalVBO);
	glDeleteBuffers(1, &m_mesh.textureVBO);
	if (m_isInstanced)
	{
		glDeleteBuffers(1, &m_mesh.instancedPosVBO);
	}
}

void Model::setPosition(glm::vec3 pos)
{
}

void Model::setRotation(glm::vec3 rot)
{
}

void Model::Render()
{
	glBindVertexArray(m_mesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_mesh.obj.vertices.size());
	glBindVertexArray(0);
}

void Model::RenderInstanced(int instanceCount)
{
	glBindVertexArray(m_mesh.VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, m_mesh.obj.vertices.size(), instanceCount);
	glBindVertexArray(0);
}

Mesh Model::getMesh()
{
	return m_mesh;
}
