#pragma once

#include "Buffer.h"
#include <glm.hpp>
#include "include/glad/gl.h"
#include "Loader.h"

#define INSTANCE_STRIDE 3
#define MAX_INSTANCES 15000

typedef struct
{
	Object obj;
	GLuint VAO;
	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint textureVBO;
	GLuint instancedPosVBO;
}Mesh;

class Model {

private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	Mesh m_mesh;
	bool m_isInstanced;

	Mesh createMesh(Object obj, bool isInstanced);

public:
	Model(Object obj, bool isInstanced);
	~Model();

	Mesh getMesh();

	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rot);
	void Render();
	void RenderInstanced(int instanceCount);
};