#include "Window.h"
#include <stdexcept>

namespace Engine {

    // Callback function for key events
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Close the window if the ESC key is pressed
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
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
        Window* self_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
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

    // Constructor
    Window::Window() {
        // Default values are already set in the member initializer list
    }

    // Destructor
    Window::~Window() {
        if (window_ != nullptr) {
            glfwDestroyWindow(window_);
            window_ = nullptr;
        }
    }

    // Initialize the window
    void Window::init() {
        if (!are_attributes_set()) {
            std::cerr << "ERROR::Window::init: Window attributes not properly set before initialization." << std::endl;
            throw std::runtime_error("Window attributes not set.");
        }

        create_window();
        make_context_current();
        set_callbacks();

        // Set the user pointer to this Window instance
        glfwSetWindowUserPointer(window_, this);

        // Enable or disable VSync
        glfwSwapInterval(vsync_ ? 1 : 0);

        // Load OpenGL functions using GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "ERROR::Window::init: Failed to initialize GLAD." << std::endl;
            throw std::runtime_error("Failed to initialize GLAD.");
        }
    }

    // Check if attributes are set
    [[nodiscard]] bool Window::are_attributes_set() const noexcept {
        return width_ > 0 && height_ > 0 && title_ != nullptr;
    }

    // Create the GLFW window
    void Window::create_window() {
        window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
        if (!window_) {
            std::cerr << "ERROR::Window::create_window: Failed to create GLFW window." << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window.");
        }
    }

    // Make the window's context current
    void Window::make_context_current() const {
        glfwMakeContextCurrent(window_);
    }

    // Set input callbacks
    void Window::set_callbacks() {
        glfwSetKeyCallback(window_, key_callback);
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    }

    // Clears the color and depth buffers
    void Window::clear_frame() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Prepares the frame for rendering
    void Window::pre_frame() const {
        // Set the background color
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        clear_frame();
    }

    // Finalizes the frame after rendering
    void Window::post_frame() const {
        // Swap front and back buffers
        glfwSwapBuffers(window_);
        // Poll for and process events
        glfwPollEvents();
    }

    // Checks if the window should close
    [[nodiscard]] bool Window::should_close() const noexcept {
        return glfwWindowShouldClose(window_);
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

    // Equality operator
    bool Window::operator==(const Window& other) const noexcept {
        return window_ == other.window_;
    }

    // Inequality operator
    bool Window::operator!=(const Window& other) const noexcept {
        return !(*this == other);
    }

} // namespace Engine
