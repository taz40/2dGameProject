#pragma once
#include <GLFW/glfw3.h>
#include <vector>

class Window {
private:
	GLFWwindow* glfw_window;
	int width, height;
	const char* title;
	bool is_fullscreen;
	bool is_resizable;
	int windowed_mode_x, windowed_mode_y;
	bool resized;

public:
	Window(int width, int height, const char* title);
	~Window();
	int getWidth();
	int getHeight();
	void setWidth(int width);
	void setHeight(int height);
	void setSize(int width, int height);
	void resizeViewport(int width, int height);
	const char* getTitle();
	void setTitle(const char* title);
	std::vector<GLFWvidmode> getVideoModes();
	std::vector<GLFWvidmode> getVideoModes(GLFWmonitor* monitor);
	std::vector<GLFWmonitor*> getMonitors();
	bool isFullscreen();
	bool isResizable();
	void setFullscreen(int width, int height, int refreshRate);
	void setFullscreen(int width, int height, int refreshRate, GLFWmonitor* monitor);
	void setWindowed();
	void setWindowedFullscreen();
	void setWindowedFullscreen(GLFWmonitor* monitor);
	void setResizable(bool resizable);
	void beginRender();
	void endRender();
	bool isClosing();
private:
	static void onResize(GLFWwindow* window, int width, int height);
};

