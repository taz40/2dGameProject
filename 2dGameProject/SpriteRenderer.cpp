#include "SpriteRenderer.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Log.h"

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
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(sizeof(float) * 12));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(NULL);
	INFO("Sprite renderer initialized");

}

SpriteRenderer::~SpriteRenderer() {
	unsigned int buffers[] = { VBO, EBO, instanceVBO };
	glDeleteBuffers(3, buffers);
	glDeleteVertexArrays(1, &VAO);
}

void SpriteRenderer::drawSprite(glm::mat4 transform) {
	matrices.push_back(transform);
}

void SpriteRenderer::flush() {
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * matrices.size(), matrices.data(), GL_STREAM_DRAW);
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, matrices.size());
	glBindVertexArray(NULL);
	glFinish();
	matrices.clear();
}