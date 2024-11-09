#include "engine_window.h"
#include <stdexcept>

namespace Engine {

    // Callback function for key events
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        Window* self_window = static_cast<Window*>(GLFW::get_window_user_pointer(window));
       
        if (self_window && self_window->get_inputs()) {
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
        // Verify that the width and height are positive
        if (width <= 0 || height <= 0) {
            std::cerr << "ERROR::Window::framebuffer_size_callback: Invalid window dimensions." << std::endl;
            return;
        }

        // Update the viewport size
        glViewport(0, 0, width, height);

        // Retrieve the Window instance from the user pointer
        Window* self_window = static_cast<Window*>(GLFW::get_window_user_pointer(window));

        if (self_window) {
            // Update the window dimensions
            self_window->set_width(width);
            self_window->set_height(height);

            // Notify the camera of the new dimensions
            Camera* camera = self_window->get_camera();
            if (camera) {
                camera->set_dimensions(width, height);
            }
        }
    }

	// Callback function for the mouse button events
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

        Engine::Window* self_window = static_cast<Engine::Window*>(GLFW::get_window_user_pointer(window));
        
        if (self_window && self_window->get_inputs()) {
            self_window->get_inputs()->mouse_button_callback(button, action);
        }
        else {
            std::cerr << "ERROR::Window::mouse_button_callback: Failed to get Window or Input instance." << std::endl;
        }
    }

    // Constructor
    Window::Window() {
        // Default values are already set in the member initializer list
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

        inputs_ = new Inputs();

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

    // Set input callbacks
    void Window::set_callbacks() {
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
            get_inputs()->update();
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

    // Sets window dimensions and title
    void Window::set_attributes(int width, int height, const char* title) {
        set_width(width);
        set_height(height);
        set_title(title);
    }

    // Sets the window width
    void Window::set_width(int width) noexcept {
        width_ = width;
    }

    // Sets the window height
    void Window::set_height(int height) noexcept {
        height_ = height;
    }

    // Sets the window title
    void Window::set_title(const char* title) noexcept {
        title_ = title;
    }

    // Enables or disables VSync
    void Window::set_vsync(bool vsync) noexcept {
        vsync_ = vsync;
    }

    // Sets the camera associated with the window
    void Window::set_camera(Camera* camera) noexcept {
        camera_ = camera;
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
    [[nodiscard]] Camera* Window::get_camera() const noexcept {
        return camera_;
    }

	// Gets the pointer to the associated Inputs object
	[[nodiscard]] Inputs* Window::get_inputs() const noexcept {
		return inputs_;
	}

    // Equality operator
    bool Window::operator==(const Window& other) const noexcept {
        return window_ == other.window_;
    }

    // Inequality operator
    bool Window::operator!=(const Window& other) const noexcept {
        return !(*this == other);
    }

} // namespace Engine
