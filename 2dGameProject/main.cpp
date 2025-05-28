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
#include <thread>
#include "Log.h"

int OpenGLVersion;
spdlog::logger logger("none");
std::mutex ready;

bool running = true;
Window* window;
unsigned int uniformBuffer;

double lastTime;

Square *square1, *square2, *square3, *square4;

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
}

void deinit() {
	glfwTerminate();
}

void render() {
	INFO("Render thread starting");
	renderInit();
	SpriteRenderer* renderer = new SpriteRenderer();
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		window->beginRender();
		square1->Draw(renderer);
		square2->Draw(renderer);
		square3->Draw(renderer);
		square4->Draw(renderer);
		renderer->flush();
		window->endRender();
	}
	delete renderer;
}

float sq1_vel = -.5f;
float sq2_scale_vel = 0.995f;
float sq3_color_vel = -0.01f;
float sq4_rot_vel = 0.01f;

void update() {
	glfwMakeContextCurrent(NULL);
	INFO("Update Thread Starting");
	lastTime = glfwGetTime();
	while (!window->isClosing()) {
		double currentTime = glfwGetTime();
		if(currentTime - lastTime >= (1/60.0)) { //Update 60 times per second
			lastTime = currentTime;
			glm::vec3 pos = square1->GetPosition();
			pos.y += sq1_vel;
			square1->SetPosition(pos);
			if(pos.y > 150 || pos.y < 50) {
				sq1_vel = -sq1_vel;
			}
			glm::vec3 scale = square2->GetScale();
			scale.x *= sq2_scale_vel;
			scale.y *= sq2_scale_vel;
			square2->SetScale(scale);
			if(scale.x < 50 || scale.x > 200) {
				sq2_scale_vel = 1.0f/sq2_scale_vel;
			}
			glm::vec4 col = square3->GetColor();
			col.b += sq3_color_vel;
			square3->SetColor(col);
			if(col.b <= 0.1 || col.b >= 1.0) {
				sq3_color_vel = -sq3_color_vel;
			}
			float rot = square4->GetRotation();
			rot += sq4_rot_vel;
			square4->SetRotation(rot);
		}
		glfwPollEvents();
	}
	running = false;
}

int main() {
	init();

	square1 = new Square(glm::vec3(100, 100, 0), glm::vec3(100, 100, 1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	square2 = new Square(glm::vec3(600, 100, 0), glm::vec3(100, 100, 1), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	square3 = new Square(glm::vec3(100, 400, 0), glm::vec3(100, 100, 1), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	square4 = new Square(glm::vec3(600, 400, 0), glm::vec3(100, 100, 1), glm::vec4(0.7f, 0.7f, 0.3f, 1.0f));

	std::thread renderThread(render);
	update();
	renderThread.join();

	deinit();

	return 0;
}