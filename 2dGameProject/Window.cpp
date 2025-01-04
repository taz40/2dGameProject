#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const char* title) {
	glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfw_window)
		throw ("Failed to create GLFW Window!");
	setSize(width, height);
	setTitle(title);
	glfwSetFramebufferSizeCallback(glfw_window, onResize);
	glfwSetWindowUserPointer(glfw_window, this);
	resized = false;
}

Window::~Window() {
	glfwDestroyWindow(glfw_window);
}

void Window::setWidth(int width) {
	setSize(width, height);
}

int Window::getWidth() {
	return width;
}

void Window::setHeight(int height) {
	setSize(width, height);
}

int Window::getHeight() {
	return height;
}

void Window::setSize(int width, int height) {
	this->height = height;
	this->width = width;
	glfwSetWindowSize(glfw_window, width, height);
}

void Window::resizeViewport(int width, int height) {
	this->width = width;
	this->height = height;
	resized = true;
}

void Window::setTitle(const char* title) {
	this->title = title;
	glfwSetWindowTitle(glfw_window, title);
}

const char* Window::getTitle() {
	return title;
}

void Window::setFullscreen(int width, int height, int refreshRate) {
	setFullscreen(width, height, refreshRate, glfwGetPrimaryMonitor());
}

void Window::setFullscreen(int width, int height, int refreshRate, GLFWmonitor* monitor) {
	is_fullscreen = true;
	glfwGetWindowPos(glfw_window, &windowed_mode_x, &windowed_mode_y);
	glfwSetWindowMonitor(glfw_window, monitor, 0, 0, width, height, refreshRate);
}

void Window::setWindowed() {
	is_fullscreen = false;
	glfwSetWindowMonitor(glfw_window, NULL, windowed_mode_x, windowed_mode_y, width, height, NULL);
}

void Window::setWindowedFullscreen() {
	setWindowedFullscreen(glfwGetPrimaryMonitor());
}

void Window::setWindowedFullscreen(GLFWmonitor* monitor) {
	if (is_fullscreen)
		setWindowed();
	glfwGetWindowPos(glfw_window, &windowed_mode_x, &windowed_mode_y);
	is_fullscreen = true;
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(glfw_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

}

std::vector<GLFWvidmode> Window::getVideoModes() {
	return getVideoModes(glfwGetPrimaryMonitor());
}

std::vector<GLFWvidmode> Window::getVideoModes(GLFWmonitor* monitor) {
	int count;
	const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);
	std::vector<GLFWvidmode> modesArray;
	for (int i = 0; i < count; i++) {
		modesArray.push_back(modes[i]);
	}
	return modesArray;
}

std::vector<GLFWmonitor*> Window::getMonitors() {
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	std::vector<GLFWmonitor*> monitorsArray;
	for (int i = 0; i < count; i++) {
		monitorsArray.push_back(monitors[i]);
	}
	return monitorsArray;
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

void Window::beginRender() {
	glfwMakeContextCurrent(glfw_window);
	if (resized) {
		std::cout << "Resizing viewport\n";
		glViewport(0, 0, width, height);
		resized = false;
	}
}

void Window::endRender() {
	glfwSwapBuffers(glfw_window);
}

bool Window::isClosing() {
	return glfwWindowShouldClose(glfw_window);
}

void Window::onResize(GLFWwindow* window, int width, int height) {
	((Window*)glfwGetWindowUserPointer(window))->resizeViewport(width, height);
}