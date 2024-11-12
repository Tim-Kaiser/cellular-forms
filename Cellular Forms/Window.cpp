

#include "Window.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void window_close_callback(GLFWwindow* window)
{
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

Window::Window(int width, int height):
	m_width(width),
	m_height(height)
{
	if (!glfwInit())
	{
		// Initialization failed
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMP_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, "OpenGL", NULL, NULL);

	if (!&m_window) {
		std::printf("window broken");
	};

	glfwSetWindowCloseCallback(m_window, window_close_callback);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);
	glfwSetKeyCallback(m_window, key_callback);

	glfwMakeContextCurrent(m_window);

	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glViewport(0, 0, m_width, m_height);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


void Window::getSize(int* width, int* height)
{
	glfwGetWindowSize(m_window, width, height);
}

void Window::Update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();

}

bool Window::Open()
{
	return !glfwWindowShouldClose(m_window);
}

void Window::setTitle(char* title)
{
	glfwSetWindowTitle(m_window, title);
}
