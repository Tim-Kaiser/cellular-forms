#pragma once
#include <glm.hpp>
#include <vector>

typedef struct {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
} Model;

bool loadModel(const char* path, Model& model);
void indexing(std::vector< int >& vertexIndices, std::vector< int >& uvIndices, std::vector< int >& normalIndices, std::vector< glm::vec3 > temp_v, std::vector< glm::vec2 > temp_uv, std::vector< glm::vec3 > temp_n,Model& model );