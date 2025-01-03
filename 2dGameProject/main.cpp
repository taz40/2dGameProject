#define GLFW_INCLUDE_NONE

#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>

int OpenGLVersion;
spdlog::logger logger("none");

#define TRACE(...) logger.trace(__VA_ARGS__)
#define DEBUG(...) logger.debug(__VA_ARGS__)
#define INFO(...) logger.info(__VA_ARGS__)
#define WARN(...) logger.warn(__VA_ARGS__)
#define ERROR(...) logger.error(__VA_ARGS__)
#define CRITICAL(...) logger.critical(__VA_ARGS__)

void init() {
	auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
	logger = spdlog::logger("Logger", {console, file});
	INFO("spdlog Initialized");

	if (!glfwInit()) {
		ERROR("Unable to Initialize GLFW");
		throw ("Unable to init GLFW");
	}
	INFO("GLFW Initialized!");
}

void openGLContextInit() {
	OpenGLVersion = gladLoadGL();
	if (OpenGLVersion == 0) {
		ERROR("Unable to load GLAD");
		throw ("Failed to load GLAD");
	}
	INFO("GLAD Initialized");
}

void deinit() {
	glfwTerminate();
}

int main() {

	init();

	Window* window = new Window(640, 480, "Test Window");

	window->beginRender();
	openGLContextInit();
	glm::vec2 vector(4, 3);
	INFO("GLM TEST: Vector is ({},{})", vector.x, vector.y);

	while (!window->isClosing()) {
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, 0.0f);
		glVertex2f(-0.5f, 0.0f);
		glEnd();

		window->endRender();
		glfwPollEvents();
	}

	deinit();

	return 0;
}