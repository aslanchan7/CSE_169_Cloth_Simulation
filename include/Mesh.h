#pragma once
#include <vector>
#include "core.h"

class Mesh {
public:
	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals,
		const std::vector<glm::ivec3>& triangleIndices);
	~Mesh();

	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void UpdateVertices(const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals);

	bool culling = false;

private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	GLsizei indexCount;

	glm::mat4 model;
	glm::vec3 color;
	glm::vec3 lightColor1;
	glm::vec3 lightColor2;

};