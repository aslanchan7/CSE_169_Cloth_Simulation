#pragma once
#include "core.h"
#include <vector>

class Sphere {
public:
	Sphere(glm::vec3 offset);
	~Sphere();
	
	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Update(glm::vec3 offset);

	glm::mat4 model;

private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::vec3 color;
	glm::vec3 lightColor1;

	GLsizei indexCount;
	float scale = 0.1f;

	// Sphere Information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};