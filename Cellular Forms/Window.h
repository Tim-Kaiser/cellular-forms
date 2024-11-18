#pragma once

#include "include/glad/gl.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <format>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void window_close_callback(GLFWwindow* window);
static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
static void error_callback(int error, const char* description);

class Window
{

private:
	int m_width;
	int m_height;

	std::string m_title;
	int m_frames;
	float m_lastFrametime;

	GLFWwindow* m_window;
public:
	Window(int width, int height);
	~Window();

	void getSize(int* width, int* height);
	void Update();
	bool Open();
};