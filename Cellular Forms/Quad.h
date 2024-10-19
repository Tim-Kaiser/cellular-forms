#pragma once

#include "Buffer.h"
#include <glm.hpp>
#include "include/glad/gl.h"

class Quad {

private:
	Buffer m_buffer;
	glm::vec3 m_position;

public:
	Quad(int width, int height);
	~Quad();

	void Render();

};