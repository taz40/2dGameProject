#pragma once
#include <glm/glm.hpp>
#include "SpriteRenderer.h"

class Square {

private:
	unsigned int VAO, VBO, EBO;
	glm::mat4 model_transform;
	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::vec4 color;

public:
	Square(glm::vec3 position, glm::vec3 scale, glm::vec4 color);
	~Square();
	void Draw(SpriteRenderer*);
	void SetPosition(glm::vec3 position);
	void SetRotation(float rotation);
	void SetScale(glm::vec3 scale);
	void SetColor(glm::vec4 color);

	glm::vec3 GetPosition();
	float GetRotation();
	glm::vec3 GetScale();
	glm::vec4 GetColor();
	
private:
	void updateTransform();
};

