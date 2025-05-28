#include "Square.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Square::Square(glm::vec3 position, glm::vec3 scale, glm::vec4 color) {
	//Initialize Member Variables
	this->position = position;
	this->scale = scale;
	this->color = color;
	this->rotation = 0;
	updateTransform();
}

Square::~Square() {

}

void Square::SetPosition(glm::vec3 position) {
	this->position = position;
	updateTransform();
}

void Square::SetRotation(float rotation) {
	this->rotation = rotation;
	updateTransform();
}

void Square::SetScale(glm::vec3 scale) {
	this->scale = scale;
	updateTransform();
}

void Square::SetColor(glm::vec4 color) {
	this->color = color;
}

glm::vec3 Square::GetPosition() {
	return this->position;
}

float Square::GetRotation() {
	return this->rotation;
}

glm::vec3 Square::GetScale() {
	return this->scale;
}

glm::vec4 Square::GetColor() {
	return this->color;
}

void Square::updateTransform() {
	model_transform = glm::mat4(1.0f);
	model_transform = glm::translate(model_transform, position);
	model_transform = glm::scale(model_transform, scale);
	model_transform = glm::rotate(model_transform, this->rotation, glm::vec3{0.0f, 0.0f, 1.0f});
}

void Square::Draw(SpriteRenderer* renderer) {
	//glUniformMatrix4fv(UNIFORM_MODEL_MAT, 1, GL_FALSE, glm::value_ptr(model_transform));
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(NULL);
	renderer->drawSprite(model_transform, color);
}