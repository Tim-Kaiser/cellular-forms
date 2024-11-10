#pragma once
#include <glm.hpp>
#include <vector>
#include "include/glad/gl.h"

typedef struct {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> uvs;
	std::vector<GLfloat> normals;
} Object;

bool loadObject(const char* path, Object& obj);
void indexing(std::vector< size_t >& vertexIndices, std::vector< size_t >& uvIndices, std::vector< size_t >& normalIndices, std::vector< glm::vec3 >& temp_v, std::vector< glm::vec2 >& temp_uv, std::vector< glm::vec3 >& temp_n, Object& obj );