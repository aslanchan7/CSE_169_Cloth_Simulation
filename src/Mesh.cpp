#include "Mesh.h"

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::ivec3>& triangleIndices) {
    // Model matrix.
    model = glm::mat4(1.0f);

    // The color of the mesh
    color = glm::vec3(1.0f, 1.0f, 1.0f);

    // Set light colors
    lightColor1 = glm::vec3(1.0f, 0.0f, 0.0f);
    //lightColor2 = glm::vec3(0.0f, 0.0f, 1.0f);

    indexCount = triangleIndices.size() * 3;
    std::vector<unsigned int> indices;
    indices.reserve(indexCount);
    for (glm::ivec3 triangle : triangleIndices) {
        indices.push_back(triangle.x);
        indices.push_back(triangle.y);
        indices.push_back(triangle.z);
    }

    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::UpdateVertices(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * positions.size(), positions.data());

    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * normals.size(), normals.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    glUniform3fv(glGetUniformLocation(shader, "LightColor"), 1, &lightColor1[0]);
    //glUniform3fv(glGetUniformLocation(shader, "LightColor2"), 1, &lightColor2[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}