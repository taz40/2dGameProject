#pragma once
#include <vector>
#include <glm/glm.hpp>

struct instance {
	glm::mat4 model;
	glm::vec4 color;
};

class SpriteRenderer {
private:
	unsigned int VAO, VBO, EBO, instanceVBO, shaderProgram;
	std::vector<instance> matrices;

public:
	SpriteRenderer();
	~SpriteRenderer();
	void drawSprite(glm::mat4 transform, glm::vec4 color);
	void flush();
};

