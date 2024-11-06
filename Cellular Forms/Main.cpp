#define GLFW_INCLUDE_NONE

#include "Particle.h"
#include "Vec2.h"
#include "Shader.h"
#include "Quad.h"
#include "Loader.h"
#include "Buffer.h"
#include "Model.h"

#include <vector>
#include <iostream>
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include "include/glad/gl.h"


int frames = 0;
std::string title = "Cellular Forms";

constexpr auto PARTICLE_COUNT = 15000;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void window_close_callback(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void updateWindowTitle(GLFWwindow* window, float deltaT)
{
	if (deltaT >= 1000.0)
	{
		float fps = frames / deltaT;
		char title[30];
		sprintf(title, "Cellular Forms FPS: %.1f", fps);
		glfwSetWindowTitle(window, title);
	}
}

void applyLorenz(GLfloat& x, GLfloat& y, GLfloat& z)
{
	float a = 10;
	float b = 28; 
	float c = 8.0 / 3.0;

	float dt = 0.0001;

	float dx = (a * (y - x)) * dt;
	float dy = (x * (b - z) - y) * dt;
	float dz = (x * y - c * z) * dy;

	x += dx;
	y += dy;
	z += dz;
}

int main(int argc, char* arfv[]) {

	if (!glfwInit())
	{
		// Initialization failed
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMP_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL - 15.000 Spheres", NULL, NULL);

	if (!window) {
		std::printf("window broken");
	};

	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
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

	Object obj;
	loadObject("Objects/sphere.obj", obj);

	Model sphereModel(obj, true);

	// PARTICLE MOVEMENT

	int elements = PARTICLE_COUNT * 3;

	//GLfloat* particles = (GLfloat*) malloc(sizeof(GLfloat) * elements);

	std::vector < GLfloat > particles(elements);

	int ROWS = 100;
	int index = 0;
	for (int i = 0; i < PARTICLE_COUNT; i = i + 1)
	{
		float x = i % ROWS;
		float y = (float) floor(i / ROWS);
		particles[index++] = (GLfloat)x * 4.0f;
		particles[index++] = (GLfloat)y * 4.0f;
		particles[index++] = (GLfloat)0.0f;
	}

	// PROJECTION

	glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0), glm::vec3(-1.0, -1.0, 0.0)), glm::vec3(0.005));
	glm::mat4 perspective = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Shader::Instance()->SendUniformData("projection", perspective);
	Shader::Instance()->SendUniformData("view", view);

	Shader::Instance()->SendUniformData("model", model);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	float lastT = 0;
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();

		//glm::mat4 rotate = glm::rotate(model, (float) glm::radians(time * 50), glm::vec3(0.0, 1.0, 0.0));
		//Shader::Instance()->SendUniformData("model", rotate);


		frames++;
		float deltaT = time - lastT;
		if (deltaT >= 1.0)
		{
			float fps = frames / deltaT;
			char title[30];
			sprintf(title, "15.000 Spheres FPS: %.1f", fps);
			glfwSetWindowTitle(window, title);
			lastT = time;
			frames = 0;
		}

		//for (int i = 0; i <= PARTICLE_COUNT - 1; i = i + 3)
		//{
			//applyLorenz(particles[i], particles[i + 1], particles[i + 2]);
		//}
		glBindVertexArray(sphereModel.getMesh().VAO);

		glBindBuffer(GL_ARRAY_BUFFER, sphereModel.getMesh().instancedPosVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * INSTANCE_STRIDE * PARTICLE_COUNT, &particles[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		sphereModel.RenderInstanced(PARTICLE_COUNT);

		

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
