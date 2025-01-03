#include "Window.h"

Window::Window(int width, int height, const char* title) {
	glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfw_window)
		throw ("Failed to create GLFW Window!");
	setWidth(width);
	setHeight(height);
	setTitle(title);

}

Window::~Window() {
	glfwDestroyWindow(glfw_window);
}

void Window::setWidth(int width) {
	this->width = width;
	glfwSetWindowSize(glfw_window, width, height);
}

int Window::getWidth() {
	return width;
}

void Window::setHeight(int height) {
	this->height = height;
	glfwSetWindowSize(glfw_window, width, height);
}

int Window::getHeight() {
	return height;
}

void Window::setTitle(const char* title) {
	this->title = title;
	glfwSetWindowTitle(glfw_window, title);
}

const char* Window::getTitle() {
	return title;
}

void Window::setFullscreen(bool fullscreen) {
	is_fullscreen = fullscreen;
	if (fullscreen)
		glfwSetWindowMonitor(glfw_window, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);
	else
		glfwSetWindowMonitor(glfw_window, NULL);
}

bool Window::isFullscreen() {
	return is_fullscreen;
}

void Window::setResizable(bool resizable) {
	is_resizable = resizable;
	if (resizable)
		glfwSetWindowAttrib(glfw_window, GLFW_RESIZABLE, GLFW_TRUE);
	else
		glfwSetWindowAttrib(glfw_window, GLFW_RESIZABLE, GLFW_FALSE);
}

bool Window::isResizable() {
	return is_resizable;
}