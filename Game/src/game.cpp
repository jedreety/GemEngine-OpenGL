#include "game.h"

void my_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// User-specific code...
	std::cout << "Framebuffer size changed to: " << width << "x" << height << std::endl;
}

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

	Gem::Core::ScopedTimer F("Game Init");

	Gem::GLFW::init();
	Gem::GLFW::set_context_version(4, 6);
	Gem::GLFW::set_openGL_profile(GLFW_OPENGL_CORE_PROFILE);

	// Use make_unique for smart pointer creation
	window_ = std::make_unique<Gem::Window::Window>(800, 600, "OpenGL Window", false);
	window_->set_framebuffer_size_callback(my_framebuffer_size_callback);

	Gem::GLAD::init();

	shader_ = std::make_unique<Gem::Graphics::Shader>();
	try {
		shader_->add_shader(GL_VERTEX_SHADER, "default.vert");
		shader_->add_shader(GL_FRAGMENT_SHADER, "default.frag");
		shader_->link_program();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	textureManager_ = std::make_unique<Gem::Graphics::Texture2DArray>(16, 16, 10);

	textureManager_->set_wrap(GL_REPEAT);
	textureManager_->set_min_filter(GL_NEAREST_MIPMAP_LINEAR);
	textureManager_->set_mag_filter(GL_NEAREST);

	textureManager_->add_texture("dirt.png");
	textureManager_->add_texture("grass.png");

	textureManager_->generate_mipmaps();

	textureBinder_ = std::make_unique<Gem::Core::TextureBinder>();
	textureBinder_->bind_texture(textureManager_.get(), 0);

	Gem::GLFW::enable_parameters();

	// Generate VAO and buffers
	VAO_.generate();

	VBO_.set_type(GL_ARRAY_BUFFER);
	VBO_.generate();

	VBO_.set_data(sizeof(vertices), vertices, GL_STATIC_DRAW);

	VAO_.link_attrib(VBO_, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO_.link_attrib(VBO_, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	IBO_.set_type(GL_ELEMENT_ARRAY_BUFFER);
	IBO_.generate();

	IBO_.set_data(sizeof(indices), indices, GL_STATIC_DRAW);

	VAO_.unbind();

	playerPosition_ = glm::vec3(0.0f, 0.0f, 2.0f);

	camera_ = std::make_unique<Gem::Graphics::Camera>();
	camera_->set_position(glm::vec3(20, 20, 20));
	camera_->set_matrix_location(shader_.get());

	// Dont forget to set the camera to the window
	window_->set_camera(camera_.get());

	// Initialize network client
	networkClient_ = new Network::Client("127.0.0.1", 1234); // Use your server IP and port
	networkClient_->Start();

	gameTimer_.set_debugFps(2);

}

void Game::run() {

	shader_->add_uniform_location("texture_array");
	shader_->add_uniform_location("modelMatrix");

	playerPosition_ = oldPosition_ = camera_->get_position();

	networkClient_->SendPosition(playerPosition_);
	float movementThreshold = 0.125f;

	Gem::Voxel::Chunk chunk;
	glm::mat4 model = glm::mat4(1.0f);

	// Main game loop
	while (!window_->should_close()) {
		window_->pre_frame();

		gameTimer_.update();

		// Activate shader program
		shader_->activate();

		// Update camera
		camera_->process_inputs(window_->get_window_ptr(), window_->get_inputs(), gameTimer_.getDeltaMillis());
		camera_->update_matrices();

		// Get other players' positions
		otherPlayersPositions_ = networkClient_->GetOtherPlayersPositions();

		// Check if position has changed
		if (glm::distance(playerPosition_, oldPosition_) > movementThreshold) {

			networkClient_->SendPosition(playerPosition_);
			oldPosition_ = playerPosition_;
		}

		// Bind texture
		shader_->set_uniform("texture_array", 0);

		// Bind VAO
		VAO_.bind();

		// Set model matrix for player's cube

		// Render chunk
		for (size_t i = 0; i < chunk.getVolume(); i++) {
			// Set model matrix for player's cube
			model = glm::mat4(1.0f);

			// Call the static function correctly
			auto pos = Gem::Voxel::Chunk::delinearize(i);

			// Translate the cube to the voxel's position
			model = glm::translate(model, glm::vec3(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos)));

			// Set the model matrix in the shader
			shader_->set_uniform_matrix("modelMatrix", glm::value_ptr(model), 1, GL_FALSE, GL_FLOAT_MAT4);

			// Draw elements
			Gem::GL::draw_elements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		}

		// Render other players
		for (const auto& player : otherPlayersPositions_) {
			// Set model matrix for other player's cube
			model = glm::mat4(1.0f);
			// Translate the cube to the other player's position
			model = glm::translate(model, player.second);
			shader_->set_uniform_matrix("modelMatrix", glm::value_ptr(model), 1, GL_FALSE, GL_FLOAT_MAT4);

			// Draw other player's cube
			Gem::GL::draw_elements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		// Unbind VAO (optional)
		VAO_.unbind();

		// Swap buffers and poll events
		window_->post_frame();
	}
}

Game::~Game() {

	shader_->cleanup();
	
	networkClient_->Stop();
	delete networkClient_;
	
	VAO_.cleanup();
	VBO_.cleanup();
	IBO_.cleanup();

	Gem::GLFW::terminate();  // GLFW cleanup is still required
}