#pragma once
#include <glm.hpp>
#include <vector>
#include "include/glad/gl.h"

typedef struct {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> uvs;
	std::vector<GLfloat> normals;
} Model;

bool loadModel(const char* path, Model& model);
void indexing(std::vector< int >& vertexIndices, std::vector< int >& uvIndices, std::vector< int >& normalIndices, std::vector< glm::vec3 > temp_v, std::vector< glm::vec2 > temp_uv, std::vector< glm::vec3 > temp_n,Model& model );