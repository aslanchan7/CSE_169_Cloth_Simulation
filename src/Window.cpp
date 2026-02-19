#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Cloth* Window::cloth;

// Plane Mesh
Mesh* Window::planeMesh;
std::vector<glm::vec3> Window::planeVertices;
std::vector<glm::vec3> Window::planeNormals;
std::vector<glm::ivec3> Window::planeIndices;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects() {
    // Create cloth
	cloth = new Cloth(
		500.0f,                         // springConstant
		20.0f,                          // dampingConstant
		1.225f,     		            // fluidDensity of air
		1.28f,                          // dragCoefficient
		0.05f,                          // restitutionCoefficient
		0.75f,  					    // frictionCoefficient
		glm::vec3(0.0f, 0.0f, 1.0f),    // windDir
		3.0f    					    // windSpeed
    );

    // Create plane
	planeVertices.resize(4);
	planeNormals.resize(4);
	planeIndices.resize(2);
	planeVertices = {
		glm::vec3(-5.0f, -2.1f, -5.0f),
		glm::vec3(5.0f, -2.1f, -5.0f),
		glm::vec3(5.0f, -2.1f, 5.0f),
		glm::vec3(-5.0f, -2.1f, 5.0f)
	};
    planeNormals = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    planeIndices = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(0, 2, 3)
    };
    planeMesh = new Mesh(planeVertices, planeNormals, planeIndices);

    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    //delete cube;

    delete cloth;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    //cube->update();
	cloth->Update(0.00016f); // TODO: Change deltaTime as needed
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render cloth
    cloth->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

    // Render plane
	planeMesh->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

	renderImGui(window);

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::renderImGui(GLFWwindow* window) {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Wind Speed");

		ImGui::SetWindowSize(ImVec2(250, 75));

        // Wind Speed Slider
		ImGui::SliderFloat("Wind Speed", &cloth->windSpeed, -5.0f, 5.0f);

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_A:
			cloth->MoveFixedParticles(-0.01f);
			break;
		case GLFW_KEY_D:
			cloth->MoveFixedParticles(0.01f);
			break;
		default:
			break;
		}
	}

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {  
            case GLFW_KEY_A:
                cloth->MoveFixedParticles(-0.01f);
                break;
            case GLFW_KEY_D:
                cloth->MoveFixedParticles(0.01f);
                break;
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	// Ignore mouse when ImGui wants to capture it
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}