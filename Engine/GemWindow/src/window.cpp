#include <Gem/Window/window.h>
#include <stdexcept>

namespace Gem {

    namespace Window {

        // Callback function for key events
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

            Window* self_window = static_cast<Window*>(GLFW::get_window_user_pointer(window));

            if (self_window && self_window->get_inputs()) {
				
                if (self_window->user_key_callback_) {
					self_window->user_key_callback_(window, key, scancode, action, mods);
				}

                self_window->get_inputs()->key_callback(key, action);
            }
            else {
                std::cerr << "ERROR::Window::key_callback: Failed to get Window or Input instance." << std::endl;
            }

            // Close window when ESC key is pressed
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                GLFW::set_window_should_close(window, GLFW_TRUE);
            }
        }

        // Callback function for window resize events
        void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
			// Retrieve the Window instance from the user pointer
			Window* self_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if (self_window) {
				// Call user-provided callback if set
				if (self_window->user_framebuffer_size_callback_) {
					self_window->user_framebuffer_size_callback_(window, width, height);
				}

				// Default behavior
				if (width <= 0 || height <= 0) {
					std::cerr << "ERROR::Window::framebuffer_size_callback: Invalid window dimensions." << std::endl;
					return;
				}

				glViewport(0, 0, width, height);
				self_window->set_width(width);
				self_window->set_height(height);

				// Notify the camera of the new dimensions
				Graphics::Camera* camera = self_window->get_camera();
				if (camera) {
					camera->set_dimensions(width, height);
				}
			}
        }
        
        // Callback function for the mouse button events
        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

            Window* self_window = static_cast<Window*>(GLFW::get_window_user_pointer(window));

            if (self_window && self_window->get_inputs()) {

				if (self_window->user_mouse_button_callback_) {
					self_window->user_mouse_button_callback_(window, button, action, mods);
				}

                self_window->get_inputs()->mouse_button_callback(button, action);
            }
            else {
                std::cerr << "ERROR::Window::mouse_button_callback: Failed to get Window or Input instance." << std::endl;
            }
        }

        // Constructor
        Window::Window(int width, int height, const char* title , bool vsync):
			width_(width), height_(height), title_(title), vsync_(vsync) {
            
            init();
        }

        // Destructor
        Window::~Window() {
            if (window_ != nullptr) {
                GLFW::destroy_window(window_);
                window_ = nullptr;
            }
        }

        // Initialize the window
        void Window::init() {

            if (!are_attributes_set()) {
                std::cerr << "ERROR::Window::init: Window attributes not properly set before initialization." << std::endl;
                throw std::runtime_error("Window attributes not set.");
            }

            inputs_ = new Input::Inputs();

            create_window();

            // Set the user pointer to this Window instance
            GLFW::set_window_user_pointer(window_, this);

            make_context_current();
            set_callbacks();


            // Enable or disable VSync
            GLFW::set_swap_interval(vsync_ ? 1 : 0);

            GLAD::init();
        }

        // Check if attributes are set
        [[nodiscard]] bool Window::are_attributes_set() const noexcept {
            return width_ > 0 && height_ > 0 && title_ != nullptr;
        }

        // Create the GLFW window
        void Window::create_window() {
            window_ = GLFW::create_window(width_, height_, title_);
        }

        // Make the window's context current
        void Window::make_context_current() const {
            GLFW::make_context_current(window_);
        }

		// Sets the window width
		void Window::set_width(int width) noexcept {
			width_ = width;
			glViewport(0, 0, width, height_);
		}

		// Sets the window height
		void Window::set_height(int height) noexcept {
			height_ = height;
			glViewport(0, 0, width_, height);
		}

        // Set input callbacks
        void Window::set_callbacks() {

            GLFW::set_window_user_pointer(window_, this);

			GLFW::set_key_callback(window_, key_callback);
			GLFW::set_framebuffer_size_callback(window_, framebuffer_size_callback);
			GLFW::set_mouse_button_callback(window_, mouse_button_callback);
        }

        // Clears the color and depth buffers
        void Window::clear_frame() const {
            GL::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // Prepares the frame for rendering
        void Window::pre_frame() const {

            // Set the background color
            GL::clear_color(0.15f, 0.15f, 0.15f, 1.0f);
            clear_frame();

            // Update inputs
            if (get_inputs())
                inputs_->update();

        }

        // Finalizes the frame after rendering
        void Window::post_frame() const {

            // Swap front and back buffers
            GLFW::swap_buffers(window_);

            // Poll for and process events
            GLFW::poll_events();
        }

        // Checks if the window should close
        [[nodiscard]] bool Window::should_close() const noexcept {
            return GLFW::window_should_close(window_);
        }

        // Sets the camera associated with the window
        void Window::set_camera(Graphics::Camera* camera, bool update_dimentions) noexcept {
            
            camera_ = camera;
            
            if (update_dimentions)
				camera_->set_dimensions(width_, height_);
        }

        // Gets the window width
        [[nodiscard]] int Window::get_width() const noexcept {
            return width_;
        }

        // Gets the window height
        [[nodiscard]] int Window::get_height() const noexcept {
            return height_;
        }

        // Gets the pointer to the GLFW window
        [[nodiscard]] GLFWwindow* Window::get_window_ptr() const noexcept {
            return window_;
        }

        // Gets the pointer to the associated Camera object
        [[nodiscard]] Graphics::Camera* Window::get_camera() const noexcept {
            return camera_;
        }

        // Gets the pointer to the associated Inputs object
        [[nodiscard]] Input::Inputs* Window::get_inputs() const noexcept {
            return inputs_;
        }

		void Window::set_framebuffer_size_callback(std::function<void(GLFWwindow*, int, int)> callback) {
			user_framebuffer_size_callback_ = callback;
		}

		void Window::set_key_callback(std::function<void(GLFWwindow*, int, int, int, int)> callback) {
			user_key_callback_ = callback;
		}

		void Window::set_mouse_button_callback(std::function<void(GLFWwindow*, int, int, int)> callback) {
			user_mouse_button_callback_ = callback;
		}

        // Equality operator
        bool Window::operator==(const Window& other) const noexcept {
            return window_ == other.window_;
        }

        // Inequality operator
        bool Window::operator!=(const Window& other) const noexcept {
            return !(*this == other);
        }

	} // namespace Window

} // namespace Gem
