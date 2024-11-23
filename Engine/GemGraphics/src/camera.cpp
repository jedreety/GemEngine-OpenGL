#include <Gem/Graphics/camera.h>
#include <iostream>

namespace Gem {

    namespace Graphics {

        // Constructor
        Camera::Camera() noexcept {
            // Default values are already set in the member initializer list.
			init();
        }

        // Initialize the camera
        void Camera::init() {
            if (!are_attributes_set()) {
                std::cerr << "ERROR::Camera::init: Camera attributes not properly set before initialization." << std::endl;
                throw std::runtime_error("Camera attributes not set.");
            }

			shader_ = new Gem::Graphics::Shader();
			shader_->set_path("../Engine/ThirdParty/assets/shaders/");

			try {
				shader_->add_shader(GL_VERTEX_SHADER, "GemDefaultCamera.vert"); // Add vertex shader
				shader_->add_shader(GL_FRAGMENT_SHADER, "GemDefaultCamera.frag"); // Add fragment shader
				shader_->link_program(); // Link shaders into a shader program
			}
			catch (const std::exception& e) {
				std::cerr << "Shader compilation/linking failed: " << e.what() << std::endl;
				exit(EXIT_FAILURE); // Exit if shaders fail to compile/link
			}

			// Generate the buffer
			matrices_ubo_.generate();

			// Bind the buffer
			matrices_ubo_.bind();

			// Allocate memory for the UBO (size of two mat4 matrices)
			matrices_ubo_.set_data(sizeof(glm::mat4) * 2, nullptr, GL_DYNAMIC_DRAW);

			// Bind the buffer base to the binding point
			glBindBufferBase(GL_UNIFORM_BUFFER, matrices_binding_point_, matrices_ubo_.get_ID());

			// Unbind the buffer
			matrices_ubo_.unbind();

			// Bind the uniform block in the shader to the binding point
			shader_->bind_uniform_block("Matrices", matrices_binding_point_);
        }

        // Check if attributes are set
        [[nodiscard]] bool Camera::are_attributes_set() const noexcept {
            return width_ > 0 && height_ > 0;
        }

        // Set the uniform locations for matrices
        void Camera::set_matrix_location(Gem::Graphics::Shader* shader) const {

			// Bind the uniform block 'Matrices' to the same binding point
			shader->bind_uniform_block("Matrices", matrices_binding_point_);
        }

        // Update and send matrices to the shader
        void Camera::update_matrices() const {
			// Calculate view matrix
			glm::mat4 view = glm::lookAt(position_, position_ + orientation_, up_);

			// Calculate projection matrix
			glm::mat4 projection = glm::perspective(glm::radians(fov_), static_cast<float>(width_) / height_, near_plane_, far_plane_);

			// Update the UBO with the matrices using the Buffer class

			// Bind the buffer
			matrices_ubo_.bind();

			// Update the projection matrix (offset 0)
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));

			// Update the view matrix (offset sizeof(mat4))
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

			// Unbind the buffer
			matrices_ubo_.unbind();

        }

        // Process inputs
        void Camera::process_inputs(GLFWwindow* window, const Input::Inputs* inputs, float deltaTime) {
            process_keyboard_input(inputs, deltaTime);
            process_mouse_input(window, inputs);
        }

		// Process keyboard input
		void Camera::process_keyboard_input(const Input::Inputs* inputs, float deltaTime) {
			glm::vec3 direction(0.0f);

			float adjusted_speed = speed_;

			if (inputs->is_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
				adjusted_speed *= 2.0f; // Increase speed when shift is held
			}

			// Collect direction inputs
			if (inputs->is_key_pressed(GLFW_KEY_W)) {
				direction += orientation_;
			}
			if (inputs->is_key_pressed(GLFW_KEY_S)) {
				direction -= orientation_;
			}
			if (inputs->is_key_pressed(GLFW_KEY_A)) {
				direction -= glm::normalize(glm::cross(orientation_, up_));
			}
			if (inputs->is_key_pressed(GLFW_KEY_D)) {
				direction += glm::normalize(glm::cross(orientation_, up_));
			}
			if (inputs->is_key_pressed(GLFW_KEY_SPACE)) {
				direction += up_;
			}
			if (inputs->is_key_pressed(GLFW_KEY_C)) {
				direction -= up_;
			}

			// Normalize the direction and move the camera
			if (glm::length(direction) > 0.0f) {
				direction = glm::normalize(direction);
				position_ += direction * adjusted_speed * deltaTime;
			}
		}

		// Process mouse input
		void Camera::process_mouse_input(GLFWwindow* window, const Input::Inputs* inputs) {
			if (inputs->is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
				// Hide cursor
				GLFW::set_input_mode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

/*				// Get current mouse position
				double mouse_x = inputs->get_mouse_x();
				double mouse_y = inputs->get_mouse_y();*/
				double mouse_x, mouse_y;
				GLFW::get_cursor_pos(window, &mouse_x, &mouse_y);

				if (first_click_) {
					last_mouse_x_ = mouse_x;
					last_mouse_y_ = mouse_y;
					first_click_ = false;
				}

				// Calculate mouse offsets
				float offset_x = static_cast<float>(mouse_x - last_mouse_x_);
				float offset_y = static_cast<float>(last_mouse_y_ - mouse_y); // Inverted since y-coordinates go from bottom to top

				// Update last mouse positions
				last_mouse_x_ = mouse_x;
				last_mouse_y_ = mouse_y;

				// Apply sensitivity
				offset_x *= sensitivity_;
				offset_y *= sensitivity_;

				// Update yaw and pitch
				yaw_ += offset_x;
				pitch_ += offset_y;

				// Clamp the pitch angle to prevent screen flipping
				if (pitch_ > 89.0f)
					pitch_ = 89.0f;
				if (pitch_ < -89.0f)
					pitch_ = -89.0f;

				// Calculate the new orientation
				glm::vec3 front;
				front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
				front.y = sin(glm::radians(pitch_));
				front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
				orientation_ = glm::normalize(front);
			}
			else {
				// Show cursor
                GLFW::set_input_mode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				first_click_ = true;
			}
		}

        // Set window dimensions
        void Camera::set_dimensions(int width, int height) noexcept {
            width_ = width;
            height_ = height;
        }

        // Set camera position
        void Camera::set_position(const glm::vec3& position) noexcept {
            position_ = position;
        }

        // Set movement speed
        void Camera::set_speed(float speed) noexcept {
            speed_ = speed;
        }

        // Set mouse sensitivity
        void Camera::set_sensitivity(float sensitivity) noexcept {
            sensitivity_ = sensitivity;
        }

        // Set field of view
        void Camera::set_fov(float fov) noexcept {
            fov_ = fov;
        }

        // Get camera position
        [[nodiscard]] glm::vec3 Camera::get_position() const noexcept {
            return position_;
        }

        // Equality operator
        bool Camera::operator==(const Camera& other) const noexcept {
            return position_ == other.position_ && orientation_ == other.orientation_;
        }

        // Inequality operator
        bool Camera::operator!=(const Camera& other) const noexcept {
            return !(*this == other);
        }

	} // namespace Graphics

} // namespace Gem
