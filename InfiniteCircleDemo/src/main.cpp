#include <iostream>
#include <limits>
#include <glm/glm.hpp>

// Gem library includes for window management, graphics, networking, etc.
#include <Gem/Window/window.h>
#include <Gem/Graphics/camera.h>
#include <Gem/Graphics/shader.h>
#include <Gem/Graphics/shapes/sphere.h>
#include <Gem/Core/timer.h>
#include <Gem/Networking/network_server.h>
#include <Gem/Networking/network_client.h>

#pragma warning(disable:4996) // Disable specific compiler warning

// Function declarations
void game(); // Main game loop for the client
void checkAndMovePlayer(glm::vec3& position); // Handles teleportation logic for the player

int main() {
	std::cout << "Enter 0 to run as server, or 1 to run as client: ";

	int choice;
	std::cin >> choice;

	if (choice == 0) {
		// Initialize and start the server
		Network::Server server(1234); // Server listens on port 1234
		server.Start();
		std::cout << "Server running. Press Enter to stop." << std::endl;
		std::cin >> choice; // Wait for user input to terminate the server
		server.Stop();
	}
	else if (choice == 1) {
		// Launch the game as a client
		game();
	}

	return 0;
}

void game() {
	// Initialize GLFW for window and context creation
	Gem::GLFW::init();
	Gem::GLFW::set_context_version(4, 6); // Set OpenGL version to 4.6
	Gem::GLFW::set_openGL_profile(GLFW_OPENGL_CORE_PROFILE); // Use core profile

	// Create a window with dimensions 800x600 and title "Infinite Circle Demo"
	Gem::Window::Window window(800, 600, "Infinite Circle Demo");

	Gem::GLAD::init(); // Initialize GLAD to manage OpenGL functions

	// Load and compile shaders
	Gem::Graphics::Shader shader;
	shader.set_path("src/"); // Set the path where shader files are located

	try {
		shader.add_shader(GL_VERTEX_SHADER, "default.vert"); // Add vertex shader
		shader.add_shader(GL_FRAGMENT_SHADER, "default.frag"); // Add fragment shader
		shader.link_program(); // Link shaders into a shader program
	}
	catch (const std::exception& e) {
		std::cerr << "Shader compilation/linking failed: " << e.what() << std::endl;
		exit(EXIT_FAILURE); // Exit if shaders fail to compile/link
	}

	// Enable essential OpenGL parameters like depth testing and face culling
	Gem::GLFW::enable_parameters(true, false, true, true);

	// Set up the camera with a wide field of view and initial position
	Gem::Graphics::Camera camera;
	camera.set_fov(60); // Field of view of 120 degrees for a wide perspective
	camera.set_position(glm::vec3(0, 0, 225)); // Position the camera along the Z-axis
	camera.set_matrix_location(&shader);
	window.set_camera(&camera); // Attach the camera to the window for view transformations

	// Initialize the network client to connect to the server at localhost:1234
	Network::Client client("jedreety.ddns.net", 1234);
	client.Start();

	// Create spheres representing different boundaries and the player
	Gem::Graphics::Shapes::Sphere inner_sphere(150, 500, 500); // Inner boundary with radius 50
	Gem::Graphics::Shapes::Sphere outer_sphere(300, 500, 500); // Outer boundary with radius 100
	Gem::Graphics::Shapes::Sphere boxed_sphere(350); // Additional sphere for visual reference
	Gem::Graphics::Shapes::Sphere player_sphere(1); // Small sphere representing the player

	Gem::Core::Timer timer; // Timer to keep track of frame times

	glm::vec3 moved_position(0, 0, 0); // Player's current position
	glm::vec3 oldPosition_ = camera.get_position(); // Store previous position to detect movement

	float movementThreshold = 0.125f; // Threshold to determine significant movement
	std::unordered_map<enet_uint32, glm::vec3> otherPlayersPositions_; // Store positions of other players

	// Get the location of the "modelMatrix" uniform in the shader
	shader.add_uniform_location("modelMatrix");
	glm::mat4 model = glm::mat4(1.0f); // Initialize model matrix

	// Main game loop
	while (!window.should_close()) {
		window.pre_frame(); // Prepare for a new frame

		moved_position = camera.get_position(); // Get the current camera (player) position

		timer.update(); // Update the timer for delta time calculations

		shader.activate(); // Activate the shader program for rendering

		checkAndMovePlayer(moved_position); // Handle teleportation if needed
		camera.set_position(moved_position); // Update camera position after potential teleport

		// Process user inputs and update camera based on delta time
		camera.process_inputs(window.get_window_ptr(), window.get_inputs(), timer.getDeltaMillis());
		camera.update_matrices(); // Update view and projection matrices

		otherPlayersPositions_ = client.GetOtherPlayersPositions(); // Retrieve positions of other connected players

		// If the player has moved beyond the threshold, send the new position to the server
		if (glm::distance(moved_position, oldPosition_) > movementThreshold) {
			client.SendPosition(moved_position);
			oldPosition_ = moved_position; // Update the old position
		}

		// Render all other players in the scene
		for (const auto& player : otherPlayersPositions_) {
			model = glm::mat4(1.0f); // Reset model matrix
			model = glm::translate(model, player.second); // Move to the other player's position
			shader.set_uniform_matrix("modelMatrix", glm::value_ptr(model), 1, GL_FALSE, GL_FLOAT_MAT4);
			player_sphere.render(); // Draw the other player's sphere
		}

		// Render the boundary spheres and the boxed sphere
		model = glm::mat4(1.0f); // Reset model matrix for static objects
		shader.set_uniform_matrix("modelMatrix", glm::value_ptr(model), 1, GL_FALSE, GL_FLOAT_MAT4);

		inner_sphere.render(); // Draw inner boundary
		outer_sphere.render(); // Draw outer boundary
		boxed_sphere.render(); // Draw additional sphere

		window.post_frame(); // Swap buffers and poll events
	}

	// Cleanup resources after the game loop ends
	shader.cleanup();
	client.Stop(); // Disconnect the client from the server
	Gem::GLFW::terminate(); // Terminate GLFW and clean up resources
}

// Function to teleport the player when they cross sphere boundaries
void checkAndMovePlayer(glm::vec3& position) {
	float radius = glm::length(position); // Calculate distance from origin

	if (radius < 150.0f) {
		// Player entered the inner sphere: teleport to the outer sphere
		float scaleFactor = 300.0f / radius; // Calculate scaling factor to maintain direction
		position *= scaleFactor; // Scale position to place player on the outer sphere
	}
	else if (radius > 300.0f) {
		// Player exited the outer sphere: teleport back to the inner sphere
		float scaleFactor = 150.0f / radius; // Calculate scaling factor to maintain direction
		position *= scaleFactor; // Scale position to place player on the inner sphere
	}
	// If within the bounds (50 <= radius <= 100), no action is needed
}
