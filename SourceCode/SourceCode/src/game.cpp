
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
	load_GLFW();
	set_openGL_version(4, 6);
	set_openGL_profile(GLFW_OPENGL_CORE_PROFILE);

	// Use make_unique for smart pointer creation
	window_ = std::make_unique<Engine::Window>();
	window_->set_attr(800, 600, "OpenGL Game");
	window_->set_vsync(false);
	window_->Init();

	load_GLAD();

	shader_ = std::make_unique<Engine::Graphics::Shader>();
	try {
		shader_->add_shader(GL_VERTEX_SHADER, "default.vert");
		shader_->add_shader(GL_FRAGMENT_SHADER, "default.frag");
		shader_->link_program();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	textureManager_ = std::make_unique<Engine::Graphics::Texture::TextureManager>();
	textureManager_->Init();

	textureManager_->AddTexture("dirt");
	textureManager_->AddTexture("grass");

	textureManager_->GenerateMipMaps();

	set_parameters();

	// Generate VAO and buffers
	VAO_.generate();
	VAO_.bind();

	VBO_.set_attr(GL_ARRAY_BUFFER, 1);
	VBO_.generate();
	VBO_.bind();
	VBO_.set_data(sizeof(vertices), vertices, GL_STATIC_DRAW);

	VAO_.link_attrib(VBO_, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO_.link_attrib(VBO_, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	IBO_.set_attr(GL_ELEMENT_ARRAY_BUFFER, 1);
	IBO_.generate();
	IBO_.bind();
	IBO_.set_data(sizeof(indices), indices, GL_STATIC_DRAW);

	VAO_.unbind();

	playerPosition_ = glm::vec3(0.0f, 0.0f, 2.0f);

	camera_ = std::make_unique<Engine::Camera>();
	camera_->set_attr(window_->get_width(), window_->get_height(), playerPosition_);
	camera_->set_Matrix_location(shader_.get());

	// Dont forget to set the camera to the window
	window_->set_camera(camera_.get());

	// Initialize network client
	networkClient_ = new NetworkClient("127.0.0.1", 1234); // Use your server IP and port
	networkClient_->Start();

}

void Game::run() {

	GLint shaderlocation_textureArray = glGetUniformLocation(shader_->get_ID(), "texture_array");
	GLint shaderlocation_modelMatrix = glGetUniformLocation(shader_->get_ID(), "modelMatrix");
	networkClient_->SendPosition(playerPosition_);
	float movementThreshold = 0.5f;

	// Main game loop
	while (!window_->is_closed()) {
		window_->postFrame();
		updateDeltaTime();

		// Activate shader program
		shader_->Activate();

		// Update camera
		camera_->Inputs(window_->get_windowPtr());
		camera_->Matrix();

		// Get other players' positions
		otherPlayersPositions_ = networkClient_->GetOtherPlayersPositions();

		// Check if position has changed
		glm::vec3 newPosition = camera_->get_position();
		if (glm::distance(newPosition, playerPosition_) > movementThreshold) {
			playerPosition_ = newPosition;
			positionChanged_ = true;
		}
		else {
			positionChanged_ = false;
		}

		// Send player position to server only if position has changed
		if (positionChanged_) {
			networkClient_->SendPosition(playerPosition_);
		}

		// Bind texture
		textureManager_->Bind();
		glUniform1i(shaderlocation_textureArray, 0);

		// Bind VAO
		VAO_.bind();

		// Set model matrix for player's cube
		glm::mat4 model = glm::mat4(1.0f);
		// Translate the cube to the player's position
		model = glm::translate(model, playerPosition_);
		// Set the model matrix in the shader
		glUniformMatrix4fv(shaderlocation_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));


		// Draw elements
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		// Render other players
		for (const auto& player : otherPlayersPositions_) {
			// Set model matrix for other player's cube
			model = glm::mat4(1.0f);
			// Translate the cube to the other player's position
			model = glm::translate(model, player.second);
			glUniformMatrix4fv(shaderlocation_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));

			// Draw other player's cube
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		// Unbind VAO (optional)
		VAO_.unbind();

		// Swap buffers and poll events
		window_->afterFrame();
	}
}

Game::~Game() {

	shader_->Delete();
	// Stop and delete network client
	networkClient_->Stop();
	delete networkClient_;
	VAO_.cleanup();
	VBO_.cleanup();
	IBO_.cleanup();

	glfwTerminate();  // GLFW cleanup is still required
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
	glCullFace(GL_BACK);
	// Uses clock-wise standard
	glFrontFace(GL_CW);
}

void Game::updateDeltaTime() {

	const float seconds = 3.5f;

	// DELTA TIME
	double current_frameTime = glfwGetTime();
	deltaTime_ = static_cast<float>(current_frameTime - last_frameTime_);
	last_frameTime_ = current_frameTime;
	
	// FPS COUNT
	fpsAccumulator_ += deltaTime_;
	frameCount_++;

	if (fpsAccumulator_ >= seconds) {
		float fps = frameCount_ / fpsAccumulator_;
		std::cout << "FPS: " << fps << std::endl;

		fpsAccumulator_ = 0.0f;
		frameCount_ = 0;
	}
}