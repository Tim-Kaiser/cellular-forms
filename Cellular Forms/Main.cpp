#define GLFW_INCLUDE_NONE

#include "ShaderLoader.h"
#include "ObjectLoader.h"
#include "Model.h"

#include <vector>
#include <iostream>
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include "include/glad/gl.h"


std::string title = "Cellular Forms";

constexpr auto PARTICLE_COUNT = 60000;


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

void applyLorenz(GLfloat& x, GLfloat& y, GLfloat& z)
{
	float a = 10;
	float b = 28; 
	float c = 8.0f / 3.0f;

	float dt = 0.0001f;

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

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);

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
	ShaderLoader shaderLoader;
	std::unique_ptr<Shader> mainShader = shaderLoader.CreateShaders();
	shaderLoader.CompileShaders("Shaders/main.vert", mainShader->m_vertexShaderID);
	shaderLoader.CompileShaders("Shaders/main.frag", mainShader->m_fragmentShaderID);

	shaderLoader.AttachShaders(*mainShader);
	shaderLoader.LinkProgram(*mainShader);

	std::unique_ptr<Shader> ssaoShader = shaderLoader.CreateShaders();
	shaderLoader.CompileShaders("Shaders/ssao.vert", ssaoShader->m_vertexShaderID);
	shaderLoader.CompileShaders("Shaders/ssao.frag", ssaoShader->m_fragmentShaderID);

	shaderLoader.AttachShaders(*ssaoShader);
	shaderLoader.LinkProgram(*ssaoShader);
	glUseProgram(mainShader->m_shaderProgramID);


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Object obj;
	loadObject("Objects/sphere_small.obj", obj);

	Model sphereModel(obj, true);

	// PARTICLE MOVEMENT

	int elements = PARTICLE_COUNT * 3;

	std::vector < GLfloat > particles;
	particles.reserve(elements);

	int ROWS = 100;
	int index = 0;
	for (int i = 0; i < PARTICLE_COUNT; i = i + 1)
	{
		int x = i % ROWS;
		float y = (float) floor(i / ROWS);
		particles.emplace_back((GLfloat)x * 4.0f);
		particles.emplace_back((GLfloat)y * 4.0f);
		particles.emplace_back((GLfloat)0.0f);
	}

	// PROJECTION

	glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0), glm::vec3(-1.0, -1.0, 0.0)), glm::vec3(0.05f));
	glm::mat4 perspective = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shaderLoader.SendUniformData("projection", perspective);
	shaderLoader.SendUniformData("view", view);

	shaderLoader.SendUniformData("model", model);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	char title[64];
	float lastFrameTime = (float)glfwGetTime();
	int frames = 0;

	bool switchShader = true;
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 rotate = glm::rotate(model, (float) glm::radians(time * 50), glm::vec3(0.0, 1.0, 0.0));
		//Shader::Instance()->SendUniformData("model", rotate);

		glBindBuffer(GL_ARRAY_BUFFER, sphereModel.getMesh().instancedPosVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* INSTANCE_STRIDE* PARTICLE_COUNT, &particles[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		sphereModel.RenderInstanced(PARTICLE_COUNT);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if ((float)glfwGetTime() > 10.0f && switchShader)
		{
			switchShader = false;
			glUseProgram(ssaoShader->m_shaderProgramID);
			glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0), glm::vec3(-1.0, -1.0, 0.0)), glm::vec3(0.05f));
			glm::mat4 perspective = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f));

			glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			shaderLoader.SendUniformData("projection", perspective);
			shaderLoader.SendUniformData("view", view);

			shaderLoader.SendUniformData("model", model);
		}


		// FRAME COUNT
		if (frames % 60 == 0)
		{
			float frametime = (float)glfwGetTime() - lastFrameTime;
			float fps = 1.0f / frametime;

			sprintf(title, "%.0i Spheres FPS: %.2f Frametime in ms: %.4f", PARTICLE_COUNT ,fps, frametime * 1000.0f);
			glfwSetWindowTitle(window, title);
		}

		lastFrameTime = (float)glfwGetTime();
		frames++;
	}

	shaderLoader.DetachShaders(*ssaoShader);
	shaderLoader.DestroyShaders(*mainShader);
	shaderLoader.DestroyProgram(*mainShader);
	shaderLoader.DestroyProgram(*ssaoShader);


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
