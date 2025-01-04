#include "Square.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "deff.h"

Square::Square(glm::vec3 position, glm::vec3 scale, glm::vec4 color) {
	//Initialize Member Variables
	this->position = position;
	this->scale = scale;
	this->color = color;
	model_transform = glm::mat4(1.0f);
	model_transform = glm::translate(model_transform, position);
	model_transform = glm::scale(model_transform, scale);

	//Generate Resources
	glGenVertexArrays(1, &VAO);
	unsigned int buffers[2];
	glGenBuffers(2, buffers);
	VBO = buffers[0];
	EBO = buffers[1];
	
	//Initialize Resources
	float vertexData[] = {
		0.0f, 0.0f, 0.0f, color.r, color.g, color.b, color.a,
		1.0f, 0.0f, 0.0f, color.r, color.g, color.b, color.a,
		1.0f, 1.0f, 0.0f, color.r, color.g, color.b, color.a,
		0.0f, 1.0f, 0.0f, color.r, color.g, color.b, color.a
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
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(NULL);
}

Square::~Square() {
	unsigned int buffers[] = { VBO, EBO };
	glDeleteBuffers(2, buffers);
	glDeleteVertexArrays(1, &VAO);
}

void Square::Draw() {
	glUniformMatrix4fv(UNIFORM_MODEL_MAT, 1, GL_FALSE, glm::value_ptr(model_transform));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}