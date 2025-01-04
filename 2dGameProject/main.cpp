#define GLFW_INCLUDE_NONE
#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <chrono>
#include "Square.h"
#include "deff.h"
#include <array>
#include <thread>

int OpenGLVersion;
spdlog::logger logger("none");
std::mutex ready;

float square[] = {
	 1.0f,  1.0f, 0.0f,  // top right
	 1.0f, -1.0f, 0.0f,  // bottom right
	-1.0f, -1.0f, 0.0f,  // bottom left
	-1.0f,  1.0f, 0.0f   // top left 
};

unsigned int squareIndices[]{
	0, 1, 3,
	1, 2, 3
};

float squareColors[] = {
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f
};

const char* vertexShaderSource = "#version 330 core\n"
"#extension GL_ARB_explicit_uniform_location: enable\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 color;\n"
"layout (location = 0) uniform mat4 projection;\n"
"layout (location = 1) uniform mat4 model;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = color;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"
"}\0";

#define TRACE(...) logger.trace(__VA_ARGS__)
#define DEBUG(...) logger.debug(__VA_ARGS__)
#define INFO(...) logger.info(__VA_ARGS__)
#define WARN(...) logger.warn(__VA_ARGS__)
#define ERR(...) logger.error(__VA_ARGS__)
#define CRITICAL(...) logger.critical(__VA_ARGS__)

bool running = true;
Window* window;
unsigned int shaderProgram;

void openGLContextInit() {
	OpenGLVersion = gladLoadGL();
	if (OpenGLVersion == 0) {
		ERR("Unable to load GLAD");
		throw ("Failed to load GLAD");
	}
	INFO("GLAD Initialized");
}

void init() {
	auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
	logger = spdlog::logger("Logger", {console, file});
	INFO("spdlog Initialized");

	if (!glfwInit()) {
		ERR("Unable to Initialize GLFW");
		throw ("Unable to init GLFW");
	}
	INFO("GLFW Initialized!");
	window = new Window(640, 480, "Test Window");
}

void renderInit() {
	//window->setWindowedFullscreen();
	window->beginRender();
	//glfwSwapInterval(1);
	window->setWindowedFullscreen();
	openGLContextInit();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		ERR("Vertex Shader Compile Failed: {}", infoLog);
		return;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		ERR("Fragment Shader Compile Failed: {}", infoLog);
		return;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		ERR("Program Link Failed: {}", infoLog);
		return;
	}

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniformMatrix4fv(UNIFORM_PROJ_MAT, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.0f, 800.0f, 600.0f, 0.0f)));
}

void deinit() {
	glDeleteProgram(shaderProgram);
	glfwTerminate();
}

void render() {
	INFO("Render thread starting");
	renderInit();
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		window->beginRender();
		window->endRender();
	}
}

void update() {
	INFO("Update Thread Starting");
	while (!window->isClosing()) {
		glfwPollEvents();
	}
	running = false;
}

int main() {

	init();

	std::thread renderThread(render);
	Sleep(10);
	update();
	renderThread.join();

	deinit();

	return 0;
}