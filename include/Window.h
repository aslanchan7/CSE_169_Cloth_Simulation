#pragma once

#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "core.h"
#include "Mesh.h"
#include "Particle.h"

class Window {
public:
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;

    // Objects to render
	static Particle* particle;

	// Plane Mesh
    static Mesh* planeMesh;
	static std::vector<glm::vec3> planeVertices;
	static std::vector<glm::vec3> planeNormals;
	static std::vector<glm::ivec3> planeIndices;
 
    // Global Variables (Adjustable Environment Settings)
    static float gravityScale;

    static float windSpeed;
    static glm::vec3 windDir;
	static float airDensity;

    static float particleRadius;
	static float dragCoefficient;
	static float restitutionCoefficient;
	static float frictionCoefficient;

    // Shader Program
    static GLuint shaderProgram;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static bool initializeObjects();
    static void cleanUp();

    // for the Window
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    // update and draw functions
    static void idleCallback();
    static void displayCallback(GLFWwindow*);

    // helper to reset the camera
    static void resetCamera();

    // helper to render imgui
    static void renderImGui(GLFWwindow*);

    // callbacks - for interaction
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};