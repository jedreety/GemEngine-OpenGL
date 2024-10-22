
#include "game.h"

GLfloat vertices[] = {
	// Positions          // Texture Coords
	// Front face
	-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  // Vertex 0
	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  // Vertex 1
	 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  // Vertex 2
	 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  // Vertex 3

	 // Back face
	  0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  // Vertex 4
	  0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  // Vertex 5
	 -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  // Vertex 6
	 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  // Vertex 7

	 // Left face
	 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  // Vertex 8
	 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  // Vertex 9
	 -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  // Vertex 10
	 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  // Vertex 11

	 // Right face
	  0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  // Vertex 12
	  0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  // Vertex 13
	  0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  // Vertex 14
	  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  // Vertex 15

	  // Top face
	  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  // Vertex 16
	  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  // Vertex 17
	   0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  // Vertex 18
	   0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  // Vertex 19

	   // Bottom face
	   -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,  // Vertex 20
	   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  // Vertex 21
		0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  // Vertex 22
		0.5f, -0.5f,  0.5f,   1.0f, 1.0f   // Vertex 23
};

GLuint indices[] = {
	// Front face (counter-clockwise)
	0, 3, 2,
	2, 1, 0,

	// Back face (counter-clockwise)
	4, 7, 6,
	6, 5, 4,

	// Left face (counter-clockwise)
	8, 11, 10,
	10, 9, 8,

	// Right face (counter-clockwise)
	12, 15, 14,
	14, 13, 12,

	// Top face (counter-clockwise)
	16, 19, 18,
	18, 17, 16,

	// Bottom face (counter-clockwise)
	20, 23, 22,
	22, 21, 20
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
	window_->set_attr(800, 600, "OpenGL Game");
	// Initialize the window
	window_->Init();

	// Load OpenGL functions
	load_GLAD();

	// Create the shader program
	shader_ = new Engine::Graphics::Shader();
	shader_->create_shader("default.vert", "default.frag");

	// Create the texture manager
	textureManager_ = new Engine::Graphics::Texture::TextureManager();
	textureManager_->set_attr(16, 16, 16);
	textureManager_->Init();

	// Add a texture to the texture manager
	textureManager_->AddTexture("dirt");

	// Generate the mipmaps
	textureManager_->GenerateMipMaps();

	// Set OpenGL parameters

	// Generate the VAO (Vertex Array Object) and VBO (Vertex Buffer Object) with only one object each
	VAO_.generate();
	VBO_.generate();
	IBO_.generate();

	// Make the VAO the current Vertex Array Object by binding it
	VAO_.Bind();

	// Upload the vertex data to the VBO
	VBO_.insert_vertices(vertices, sizeof(vertices));
	
	// Upload the indices data to the IBO
	IBO_.insert_indices(indices, sizeof(indices));

	// Link the VBO to the VAO with the layout 0
	VAO_.LinkAttrib(VBO_, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO_.LinkAttrib(VBO_, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	set_parameters();

	camera_ = new Engine::Camera();
	camera_->set_attr(window_->get_width(), window_->get_height(), glm::vec3(0.0f, 0.0f, 2.0f));

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

		camera_->Inputs(window_->get_windowPtr());
		camera_->Matrix(shader_);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureManager_->get_textureArrayID());
		glUniform1i(glGetUniformLocation(shader_->get_ID(), "texture_array"), 0);

		// Bind the VAO so OpenGL knows to use it
		VAO_.Bind();

		// Draw
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		window_->afterFrame();
	}

	std::cout << "Game loop exited" << std::endl;

}

Game::~Game() {

	VAO_.Delete();
	VBO_.Delete();
	IBO_.Delete();
	shader_->Delete();

	delete textureManager_;
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
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
