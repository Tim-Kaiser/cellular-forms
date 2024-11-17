#pragma once

#include <glm.hpp>
#include "include/glad/gl.h"
#include "ObjectLoader.h"

#define INSTANCE_STRIDE 3
#define MAX_INSTANCES 60000

struct Mesh
{
	Object* obj;
	GLuint VAO;
	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint textureVBO;
	GLuint instancedPosVBO;

	Mesh():
		VAO(0),
		vertexVBO(0),
		normalVBO(0),
		textureVBO(0),
		instancedPosVBO(0)
	{
		obj = new Object;
	}
};

class Model {

private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	Mesh* m_mesh;
	bool m_isInstanced;

	Mesh* createMesh(Object* obj, bool isInstanced);

public:
	Model(Object* obj, bool isInstanced);
	~Model();

	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rot);
	void Render();
	void RenderInstanced(int instanceCount);

	Mesh* getMesh();
};