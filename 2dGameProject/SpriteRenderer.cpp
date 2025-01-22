#include "SpriteRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Log.h"
#include "deff.h"

const char* vertexShaderSource = "#version 330 core\n"
"#extension GL_ARB_explicit_uniform_location: enable\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 color;\n"
"layout (std140) uniform matricies \n"
"{\n"
"	mat4 projection;\n"
"	mat4 view;\n"
"};\n"
"layout (location = 2) in mat4 model;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = color;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"
"}\0";

SpriteRenderer::SpriteRenderer() {
	//Generate Resources
	glGenVertexArrays(1, &VAO);
	unsigned int buffers[3];
	glGenBuffers(3, buffers);
	VBO = buffers[0];
	EBO = buffers[1];
	instanceVBO = buffers[2];

	//Initialize Resources
	float vertexData[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	unsigned int indices[]{
		0, 1, 2,
		0, 2, 3
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 20 * sizeof(float), (void*)(16 * sizeof(float)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 20 * sizeof(float), (void*)0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 20 * sizeof(float), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 20 * sizeof(float), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 20 * sizeof(float), (void*)(sizeof(float) * 12));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);

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

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		ERR("Program Link Failed: {}", infoLog);
		return;
	}

	glUseProgram(shaderProgram);
	unsigned int proj_index = glGetUniformBlockIndex(shaderProgram, "matricies");
	glUniformBlockBinding(shaderProgram, proj_index, PROJ_MAT_UNIFORM_BUFFER);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	INFO("Sprite renderer initialized");

}

SpriteRenderer::~SpriteRenderer() {
	unsigned int buffers[] = { VBO, EBO, instanceVBO };
	glDeleteBuffers(3, buffers);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);
}

void SpriteRenderer::drawSprite(glm::mat4 transform, glm::vec4 color) {
	matrices.push_back(instance{transform, color});
}

void SpriteRenderer::flush() {
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(instance) * matrices.size(), matrices.data(), GL_STREAM_DRAW);
	glBindVertexArray(VAO);
	glUseProgram(shaderProgram);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, matrices.size());
	glBindVertexArray(0);
	glFinish();
	matrices.clear();
}