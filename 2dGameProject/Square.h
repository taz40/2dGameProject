#pragma once
#include <glm/glm.hpp>

class Square {

private:
	unsigned int VAO, VBO, EBO;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec4 color;
	glm::mat4 model_transform;

public:
	Square(glm::vec3 position, glm::vec3 scale, glm::vec4 color);
	~Square();
	void Draw();
};

