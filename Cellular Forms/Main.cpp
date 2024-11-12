#define GLFW_INCLUDE_NONE

#include "Window.h"
#include "ShaderLoader.h"
#include "ObjectLoader.h"
#include "Model.h"

#include <vector>
#include <glm.hpp>


std::string title = "Cellular Forms";

constexpr auto PARTICLE_COUNT = 60000;


int main(int argc, char* arfv[]) {

	Window window(1920, 1080);

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

	std::unique_ptr<Shader> gBufferShader = shaderLoader.CreateShaders();
	shaderLoader.CompileShaders("Shaders/gBuffer.vert", gBufferShader->m_vertexShaderID);
	shaderLoader.CompileShaders("Shaders/gBuffer.frag", gBufferShader->m_fragmentShaderID);

	shaderLoader.AttachShaders(*gBufferShader);
	shaderLoader.LinkProgram(*gBufferShader);
	glUseProgram(mainShader->m_shaderProgramID);

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

	////SSAO
	//GLuint gBuffer;
	//glGenFramebuffers(1, &gBuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	//GLuint gPos, gNormal, gColor;

	//// SSAO
	//glGenTextures(1, &gPos);
	//glBindTexture(GL_TEXTURE_2D, gPos);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gWidth, gHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);

	//glGenTextures(1, &gNormal);
	//glBindTexture(GL_TEXTURE_2D, gNormal);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gWidth, gHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//glGenTextures(1, &gColor);
	//glBindTexture(GL_TEXTURE_2D, gColor);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gWidth, gHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);


	//GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	//glDrawBuffers(3, attachments);

	 
	while (window.Open()) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 rotate = glm::rotate(model, (float) glm::radians(time * 50), glm::vec3(0.0, 1.0, 0.0));
		//Shader::Instance()->SendUniformData("model", rotate);

		glBindBuffer(GL_ARRAY_BUFFER, sphereModel.getMesh().instancedPosVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* INSTANCE_STRIDE* PARTICLE_COUNT, &particles[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		sphereModel.RenderInstanced(PARTICLE_COUNT);

		window.Update();

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
			window.setTitle(title);
		}

		lastFrameTime = (float)glfwGetTime();
		frames++;
	}

	shaderLoader.DetachShaders(*ssaoShader);
	shaderLoader.DestroyShaders(*mainShader);
	shaderLoader.DestroyProgram(*mainShader);
	shaderLoader.DestroyProgram(*ssaoShader);

	return 0;
}
