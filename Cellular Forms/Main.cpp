#define GLFW_INCLUDE_NONE

#include "Window.h"
#include "ShaderLoader.h"
#include "ObjectLoader.h"
#include "Model.h"

#include <vector>
#include <glm.hpp>

constexpr auto PARTICLE_COUNT = 60000;

int main(int argc, char* arfv[]) {

	Window window(2540, 1080);

	//===== SHADER INIT =====
	ShaderLoader shaderLoader;

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
	glUseProgram(gBufferShader->m_shaderProgramID);

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

	Object obj;
	loadObject("Objects/sphere_small.obj", obj); 
	Model sphereModel(&obj, true);


	Object objQuad;
	loadObject("Objects/quad.obj", objQuad);
	Model quadModel(&objQuad, false);


	//SSAO
	GLuint gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	GLuint gPos, gNormal, gColor;

	int width, height;

	window.getSize(&width, &height);

	// SSAO
	glGenTextures(1, &gPos);
	glBindTexture(GL_TEXTURE_2D, gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);

	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	glGenTextures(1, &gColor);
	glBindTexture(GL_TEXTURE_2D, gColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);


	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);


	GLuint rboDepthBuffer;
	glGenRenderbuffers(1, &rboDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepthBuffer);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(ssaoShader->m_shaderProgramID);
	GLuint pos = 0;
	GLuint normal = 1;
	GLuint color = 2;
	shaderLoader.SendUniformData("gPos", pos);
	shaderLoader.SendUniformData("gNormal", normal);
	shaderLoader.SendUniformData("gColor", color);
	
	while (window.Open()) {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glUseProgram(gBufferShader->m_shaderProgramID);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//////glm::mat4 rotate = glm::rotate(model, (float) glm::radians(time * 50), glm::vec3(0.0, 1.0, 0.0));
		//////Shader::Instance()->SendUniformData("model", rotate);

		glBindVertexArray(sphereModel.getMesh()->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, sphereModel.getMesh()->instancedPosVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* INSTANCE_STRIDE* PARTICLE_COUNT, &particles[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		sphereModel.RenderInstanced(PARTICLE_COUNT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(ssaoShader->m_shaderProgramID);

		
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPos);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColor);

		quadModel.Render();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		window.Update();
	}

	glDeleteFramebuffers(1, &gBuffer);


	shaderLoader.DetachShaders(*ssaoShader);

	shaderLoader.DestroyShaders(*gBufferShader);
	shaderLoader.DestroyProgram(*gBufferShader);

	shaderLoader.DestroyShaders(*ssaoShader);
	shaderLoader.DestroyProgram(*ssaoShader);
	return 0;
}
