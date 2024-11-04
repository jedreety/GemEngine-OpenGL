#include "cameraProgram.h"

#include <iostream>

namespace Engine {

    // Constructor
    Camera::Camera() noexcept {
        // Default values are already set in the member initializer list.
    }

    // Initialize the camera
    void Camera::init() {
        if (!are_attributes_set()) {
            std::cerr << "ERROR::Camera::init: Camera attributes not properly set before initialization." << std::endl;
            throw std::runtime_error("Camera attributes not set.");
        }
    }

    // Check if attributes are set
    [[nodiscard]] bool Camera::are_attributes_set() const noexcept {
        return width_ > 0 && height_ > 0;
    }

    // Set the uniform locations for matrices
    void Camera::set_matrix_location(const Engine::Graphics::Shader* shader) {
        projection_matrix_location_ = glGetUniformLocation(shader->get_ID(), "projectionMatrix");
        view_matrix_location_ = glGetUniformLocation(shader->get_ID(), "viewMatrix");

        if (projection_matrix_location_ == -1 || view_matrix_location_ == -1) {
            std::cerr << "ERROR::Camera::set_matrix_location: Failed to get uniform locations." << std::endl;
            throw std::runtime_error("Failed to get uniform locations.");
        }
    }

    // Update and send matrices to the shader
    void Camera::update_matrices() const {
        // Ensure uniform locations are valid
        if (projection_matrix_location_ == -1 || view_matrix_location_ == -1) {
            std::cerr << "ERROR::Camera::update_matrices: Uniform locations not set. Call set_matrix_location() first." << std::endl;
            throw std::runtime_error("Uniform locations not set.");
        }

        // Calculate view matrix
        glm::mat4 view = glm::lookAt(position_, position_ + orientation_, up_);

        // Calculate projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(fov_), static_cast<float>(width_) / height_, near_plane_, far_plane_);

        // Send matrices to the shader
        glUniformMatrix4fv(projection_matrix_location_, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(view_matrix_location_, 1, GL_FALSE, glm::value_ptr(view));
    }

    // Process inputs
    void Camera::process_inputs(GLFWwindow* window) {
        process_keyboard_input(window);
        process_mouse_input(window);
    }

    // Process keyboard input
    void Camera::process_keyboard_input(GLFWwindow* window) {
        float adjusted_speed = speed_;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            adjusted_speed *= 2.0f; // Increase speed when shift is held
        }

        // Forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position_ += adjusted_speed * orientation_;
        }
        // Backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position_ -= adjusted_speed * orientation_;
        }
        // Left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position_ -= glm::normalize(glm::cross(orientation_, up_)) * adjusted_speed;
        }
        // Right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position_ += glm::normalize(glm::cross(orientation_, up_)) * adjusted_speed;
        }
        // Up
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            position_ += adjusted_speed * up_;
        }
        // Down
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            position_ -= adjusted_speed * up_;
        }
    }

    // Process mouse input
    void Camera::process_mouse_input(GLFWwindow* window) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // Hide cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            // Prevent sudden jump on first click
            if (first_click_) {
                glfwSetCursorPos(window, width_ / 2.0, height_ / 2.0);
                first_click_ = false;
            }

            // Get cursor position
            double mouse_x, mouse_y;
            glfwGetCursorPos(window, &mouse_x, &mouse_y);

            // Calculate offsets
            float offset_x = static_cast<float>(mouse_x - width_ / 2.0);
            float offset_y = static_cast<float>(mouse_y - height_ / 2.0);

            // Apply sensitivity
            offset_x *= sensitivity_ / width_;
            offset_y *= sensitivity_ / height_;

            // Update orientation
            glm::vec3 right = glm::normalize(glm::cross(orientation_, up_));

            orientation_ = glm::rotate(orientation_, glm::radians(-offset_x), up_);
            orientation_ = glm::rotate(orientation_, glm::radians(-offset_y), right);
            orientation_ = glm::normalize(orientation_);

            // Reset cursor position
            glfwSetCursorPos(window, width_ / 2.0, height_ / 2.0);
        }
        else {
            // Show cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

} // namespace Engine
