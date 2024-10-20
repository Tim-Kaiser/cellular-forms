#define GLFW_INCLUDE_NONE

#include "Particle.h"
#include "Vec2.h"
#include "Shader.h"
#include "Quad.h"
#include "Loader.h"
#include "Buffer.h"

#include <vector>
#include <iostream>
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include "include/glad/gl.h"


std::vector<Particle> setup() {
	std::vector<Particle> particles;
	Particle p = Particle(0, 0, 0);
	particles.push_back(p);
	return particles;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void window_close_callback(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char* arfv[]) {
	//std::vector<Particle> particles = setup();

	if (!glfwInit())
	{
		// Initialization failed
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Cellular Forms", NULL, NULL);

	if (!window) {
		std::printf("window broken");
	};

	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);



	//===== SHADER INIT =====
	if (!Shader::Instance()->CreateProgram()) {
		return 0;
	}

	if (!Shader::Instance()->CreateShaders()) {
		return 0;
	}
	if (!Shader::Instance()->CompileShaders("Shaders/main.vert", Shader::ShaderType::VERTEX_SHADER)) {
		return -1;
	}
	if (!Shader::Instance()->CompileShaders("Shaders/main.frag", Shader::ShaderType::FRAGMENT_SHADER)) {
		return -1;
	}
	Shader::Instance()->AttachShaders();

	if (!Shader::Instance()->LinkProgram()) {
		return 0;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Model model;
	loadModel("Objects/Cube.obj",model);

	// MODEL
	GLuint vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(glm::vec3), &model.vertices[0], GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glVertexAttribPointer(
			0,                  
			3,                  
			GL_FLOAT,          
			GL_FALSE,           
			0,                  
			(void*)0            
		);
		glDrawArrays(GL_TRIANGLES, 0, model.vertices.size());
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Shader::Instance()->DetachShaders();
	Shader::Instance()->DestroyShaders();
	Shader::Instance()->DestroyProgram();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
