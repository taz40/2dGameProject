#pragma once
#include <vector>
#include <glm/glm.hpp>

class SpriteRenderer {
private:
	unsigned int VAO, VBO, EBO, instanceVBO;
	std::vector<glm::mat4> matrices;

public:
	SpriteRenderer();
	~SpriteRenderer();
	void drawSprite(glm::mat4 transform);
	void flush();
};

