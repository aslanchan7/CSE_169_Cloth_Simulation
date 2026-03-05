#include "Sphere.h"

Sphere::Sphere(glm::vec3 offset) {
	// Model matrix. Scale the unit sphere down to a particle-sized sphere.
	model = glm::scale(glm::translate(glm::mat4(1.0f), offset), glm::vec3(scale));

    // The color of the mesh
    color = glm::vec3(0.0f, 0.0f, 0.0f);

    // Set light colors
    lightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);

    // Fixed positions to form unit sphere
    positions = {
        {0.0f, 1.0f, 0.0f},

        {0.5f, 0.866f, 0.0f},
        {0.433f,0.866f,0.25f},
        {0.25f,0.866f,0.433f},
        {0.0f,0.866f,0.5f},
        {-0.25f,0.866f,0.433f},
        {-0.433f,0.866f,0.25f},
        {-0.5f,0.866f,0.0f},
        {-0.433f,0.866f,-0.25f},
        {-0.25f,0.866f,-0.433f},
        {0.0f,0.866f,-0.5f},
        {0.25f,0.866f,-0.433f},
        {0.433f,0.866f,-0.25f},

        {1.0f,0.0f,0.0f},
        {0.866f,0.0f,0.5f},
        {0.5f,0.0f,0.866f},
        {0.0f,0.0f,1.0f},
        {-0.5f,0.0f,0.866f},
        {-0.866f,0.0f,0.5f},
        {-1.0f,0.0f,0.0f},
        {-0.866f,0.0f,-0.5f},
        {-0.5f,0.0f,-0.866f},
        {0.0f,0.0f,-1.0f},
        {0.5f,0.0f,-0.866f},
        {0.866f,0.0f,-0.5f},

        {0.5f,-0.866f,0.0f},
        {0.433f,-0.866f,0.25f},
        {0.25f,-0.866f,0.433f},
        {0.0f,-0.866f,0.5f},
        {-0.25f,-0.866f,0.433f},
        {-0.433f,-0.866f,0.25f},
        {-0.5f,-0.866f,0.0f},
        {-0.433f,-0.866f,-0.25f},
        {-0.25f,-0.866f,-0.433f},
        {0.0f,-0.866f,-0.5f},
        {0.25f,-0.866f,-0.433f},
        {0.433f,-0.866f,-0.25f},

        {0.0f,-1.0f,0.0f}
    };

    normals = positions;

    indices = {
        0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,7,
        0,7,8, 0,8,9, 0,9,10, 0,10,11, 0,11,12, 0,12,1,

        1,13,14, 1,14,2,
        2,14,15, 2,15,3,
        3,15,16, 3,16,4,
        4,16,17, 4,17,5,
        5,17,18, 5,18,6,
        6,18,19, 6,19,7,
        7,19,20, 7,20,8,
        8,20,21, 8,21,9,
        9,21,22, 9,22,10,
        10,22,23, 10,23,11,
        11,23,24, 11,24,12,
        12,24,13, 12,13,1,

        13,25,26, 13,26,14,
        14,26,27, 14,27,15,
        15,27,28, 15,28,16,
        16,28,29, 16,29,17,
        17,29,30, 17,30,18,
        18,30,31, 18,31,19,
        19,31,32, 19,32,20,
        20,32,33, 20,33,21,
        21,33,34, 21,34,22,
        22,34,35, 22,35,23,
        23,35,36, 23,36,24,
        24,36,25, 24,25,13,

        37,26,25, 37,27,26, 37,28,27, 37,29,28,
        37,30,29, 37,31,30, 37,32,31, 37,33,32,
        37,34,33, 37,35,34, 37,36,35, 37,25,36
    };

	indexCount = indices.size();

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

Sphere::~Sphere() {
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Sphere::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
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

void Sphere::Update(glm::vec3 offset) {
	// Update the model matrix to reflect the new position of the sphere.
    model = glm::translate(glm::mat4(1.0f), offset) * glm::scale(glm::mat4(1.0f), glm::vec3(scale));
}