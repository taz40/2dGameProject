#include "Square.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Square::Square(glm::vec3 position, glm::vec3 scale, glm::vec4 color) {
	//Initialize Member Variables
	this->position = position;
	this->scale = scale;
	this->color = color;
	model_transform = glm::mat4(1.0f);
	model_transform = glm::translate(model_transform, position);
	model_transform = glm::scale(model_transform, scale);
}

Square::~Square() {

}

void Square::Draw(SpriteRenderer* renderer) {
	//glUniformMatrix4fv(UNIFORM_MODEL_MAT, 1, GL_FALSE, glm::value_ptr(model_transform));
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(NULL);
	renderer->drawSprite(model_transform);
}