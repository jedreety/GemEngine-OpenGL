
#include "game.h"


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES   
	-0.5f,  0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,
	 0.5f,  0.5f,  0.0f,
	 0.0f,  0.0f,  0.0f,
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

	// Set OpenGL parameters
	set_parameters();

	// Create the shader program
	shader_ = new Engine::Graphics::Shader();
	shader_->create_shader("default.vert", "default.frag");

	// Generate the VAO (Vertex Array Object) and VBO (Vertex Buffer Object) with only one object each
	VAO_.gen_VAO();
	VBO_.gen_VBO();
	IBO_.gen_IBO();

	// Make the VAO the current Vertex Array Object by binding it
	VAO_.Bind();

	// Upload the vertex data to the VBO
	VBO_.insert_vertices(vertices, sizeof(vertices));
	
	// Upload the indices data to the IBO
	IBO_.insert_indices(indices, sizeof(indices));

	// Link the VBO to the VAO with the layout 0
	VAO_.LinkAttrib(VBO_, 0, 3, GL_FLOAT, 0, (void*)0);

	// Unbind both the VBO and VAO so that we don't accidentally modify them later
	VBO_.Unbind();
	VAO_.Unbind();
	IBO_.Unbind();

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
		VAO_.Bind();
		// Draw
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		window_->afterFrame();
	}

	VAO_.Delete();
	VBO_.Delete();
	IBO_.Delete();
	shader_->Delete();

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

void Game::set_parameters() {
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
