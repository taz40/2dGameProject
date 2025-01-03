#pragma once
#include <GLFW/glfw3.h>

class Window {
private:
	GLFWwindow* glfw_window;
	int width, height;
	const char* title;
	bool is_fullscreen;
	bool is_resizable;

public:
	Window(int width, int height, const char* title);
	~Window();
	int getWidth();
	int getHeight();
	void setWidth(int width);
	void setHeight(int height);
	const char* getTitle();
	void setTitle(const char* title);
	bool isFullscreen();
	bool isResizable();
	void setFullscreen(bool fullscreen);
	void setResizable(bool resizable);
};

