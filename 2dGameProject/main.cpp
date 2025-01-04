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

int OpenGLVersion;
spdlog::logger logger("none");

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
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 color;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
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
}

void openGLContextInit() {
	OpenGLVersion = gladLoadGL();
	if (OpenGLVersion == 0) {
		ERR("Unable to load GLAD");
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

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

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
		return -1;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		ERR("Fragment Shader Compile Failed: {}", infoLog);
		return -1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		ERR("Program Link Failed: {}", infoLog);
		return -1;
	}

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	unsigned int colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareColors), squareColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	bool up = false;
	float interval = 0.001;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	unsigned int projMatLocation = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f)));

	unsigned int modelMatLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::translate(glm::mat4(1.0), glm::vec3(400, 300, 0)), glm::vec3(100, 100, 1))));
	while (!window->isClosing()) {
		if (up) {
			squareColors[0] += interval;
			if (squareColors[0] >= 1.0) {
				squareColors[0] = 1.0;
				up = false;
			}
		} else {
			squareColors[0] -= interval;
			if (squareColors[0] <= 0.0) {
				squareColors[0] = 0.0;
				up = true;
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareColors), squareColors, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(NULL);

		window->endRender();
		glfwPollEvents();
	}

	deinit();

	return 0;
}