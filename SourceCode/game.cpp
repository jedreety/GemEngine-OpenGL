
#include "game.h"

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES   
	-0.5f, 0.0f,  0.5f,
	-0.5f, 0.0f, -0.5f,
	 0.5f, 0.0f, -0.5f,
	 0.5f, 0.0f,  0.5f,
	 0.0f, 0.8f,  0.0f,
};

// Indices for vertices order
GLuint indices[] =
{
	2, 1, 0,
	3, 2, 0,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

namespace Game
{
	Game::Game() {
		// Initialize GLFW
		load_GLFW();

		// Set the OpenGL version to 3.3
		set_openGL_version(3, 3);
		// Tell GLFW to use the core profile
		set_openGL_profile(GLFW_OPENGL_CORE_PROFILE);

		// Create the window
		window_ = new Engine::Window();
		// Set the window attributes
		window_->set_width(800);
		window_->set_height(600);
		window_->set_title("Minecraft Clone");
		// Initialize the window
		window_->Init();

		// Load OpenGL functions
		load_GLAD();

		// Create the shader program
		shader_ = new Engine::Graphics::Shader();
		shader_->create_shader("default.vert", "default.frag");

		// Vertices coordinates
		GLfloat vertices[] =
		{
			-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
			0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
			0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
		};

		// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
		GLuint VBO;

		// Generate the VAO and VBO with only 1 object each
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// Make the VAO the current Vertex Array Object by binding it
		glBindVertexArray(VAO);

		// Bind the VBO specifying it's a GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Introduce the vertices into the VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// Enable the Vertex Attribute so that OpenGL knows to use it
		glEnableVertexAttribArray(0);

		// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void Game::run() {

		// The main game loop
		while (!window_->is_closed()) // Check if the window is closed
		{
			// Draw the frame

			window_->postFrame();

			// Tell OpenGL which Shader Program we want to use
			shader_->Activate();
			// Bind the VAO so OpenGL knows to use it
			glBindVertexArray(VAO);
			// Draw the triangle using the GL_TRIANGLES primitive
			glDrawArrays(GL_TRIANGLES, 0, 3);

			window_->afterFrame();
		}

		std::cout << "Game loop exited" << std::endl;

	}

	Game::~Game() {

		delete window_;
		// Terminate GLFW
		glfwTerminate();
	}

	void Game::load_GLFW() {

		if (!glfwInit()) {
			// Handle GLFW initialization failure
			std::cerr << "Failed to initialize GLFW" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void Game::set_openGL_version(const int major, const int minor) {
		// Set the OpenGL version to major.minor
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	}

	void Game::set_openGL_profile(const int profile) {
		// Set the OpenGL profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	}

	void Game::load_GLAD() {

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			// Handle GLAD initialization failure
			glGetError();
			std::cerr << "Failed to initialize GLAD" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

}