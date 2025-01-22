#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Window.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "Square.h"
#include "deff.h"
#include <vector>
#include <thread>
#include "Log.h"

int OpenGLVersion;
spdlog::logger logger("none");
std::mutex ready;

bool running = true;
Window* window;
unsigned int uniformBuffer;

struct matricies {
	glm::mat4 projection;
	glm::mat4 view;
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam) {
	if (type == GL_DEBUG_TYPE_ERROR) {
		ERR("GL CALLBACK: ** GL ERROR ** type = {}, severity = {}, message = {}", type, severity, message);
	} else {
		DEBUG("GL CALLBACK: type = {}, severity = {}, message = {}", type, severity, message);
	}
}

void openGLContextInit() {
	OpenGLVersion = gladLoadGL();
	if (OpenGLVersion == 0) {
		ERR("Unable to load GLAD");
		throw ("Failed to load GLAD");
	}
	INFO("GLAD Initialized");
	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)){
		ERR("Failed to create Debug Context");
	}
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageCallback(MessageCallback, 0);
	INFO("GL Debug Callback Initialized");
	int count = 0;
	do {
		GLenum sources, types, severities;
		int lengths;
		GLuint ids;
		int len = 0;
		glGetIntegerv(GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH, &len);
		char* message = new char[len];
		count = glGetDebugMessageLog(1, len, &sources, &types, &ids, &severities, &lengths, message);
		MessageCallback(sources, types, ids, severities, lengths, message, 0);
		delete[] message;
	} while (count != 0);
}

void init() {
	Log::Init();
	if (!glfwInit()) {
		ERR("Unable to Initialize GLFW");
		throw ("Unable to init GLFW");
	}
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	INFO("GLFW Initialized!");
	window = new Window(640, 480, "Test Window");
}

void renderInit() {
	//window->setWindowedFullscreen();
	window->beginRender();
	glfwSwapInterval(0);
	//window->setWindowedFullscreen();
	openGLContextInit();

	glGenBuffers(1, &uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, new matricies{glm::ortho(0.0f, 800.0f, 600.0f, 0.0f), glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))}, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, PROJ_MAT_UNIFORM_BUFFER, uniformBuffer);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void deinit() {
	glfwTerminate();
}

void render() {
	INFO("Render thread starting");
	renderInit();
	SpriteRenderer* renderer = new SpriteRenderer();
	std::vector<Square*> squares;
	for (int i = 0; i < 100 * 100; i++) {
		int x = i % 100;
		int y = i / 100;
		squares.push_back(new Square({x*6, y*6, 0}, {6, 6, 1}, {0, 0, 1, 1}));
	}
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		window->beginRender();
		for (auto square : squares) {
			square->Draw(renderer);
		}
		renderer->flush();
		window->endRender();
	}
	delete renderer;
}

void update() {
	glfwMakeContextCurrent(NULL);
	INFO("Update Thread Starting");
	while (!window->isClosing()) {
		glfwPollEvents();
	}
	running = false;
}

int main() {
	init();

	std::thread renderThread(render);
	update();
	renderThread.join();

	deinit();

	return 0;
}