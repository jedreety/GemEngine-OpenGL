#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../Graphics/Shader/engine_shaderProgram.h"

namespace Engine {

    /**
     * @brief Camera class for handling camera movements and view/projection matrices.
     *
     * The Camera class encapsulates the functionality required to control the camera in a 3D environment.
     * It handles user input for movement and orientation, and provides view and projection matrices for rendering.
     */
    class Camera {
    public:
        /**
         * @brief Constructs a Camera object with default parameters.
         *
         * Initializes camera position, orientation, and other parameters to default values.
         */
        Camera() noexcept;

        /**
         * @brief Initializes the camera with the required attributes.
         *
         * Must be called after setting the necessary attributes like window dimensions and position.
         *
         * @throws std::runtime_error if required attributes are not set.
         */
        void init();

        /**
         * @brief Sets the uniform locations for the view and projection matrices in the shader.
         *
         * @param shader Pointer to the Shader object.
         */
        void set_matrix_location(const Engine::Graphics::Shader* shader);

        /**
         * @brief Updates and sends the view and projection matrices to the shader.
         *
         * Calculates the view and projection matrices based on the camera's current state.
         */
        void update_matrices() const;

        /**
         * @brief Handles camera input processing.
         *
         * Processes both keyboard and mouse inputs to update the camera's position and orientation.
         *
         * @param window Pointer to the GLFW window.
         */
        void process_inputs(GLFWwindow* window);

        /**
         * @brief Sets the window dimensions.
         *
         * @param width Window width in pixels.
         * @param height Window height in pixels.
         */
        void set_dimensions(int width, int height) noexcept;

        /**
         * @brief Sets the camera's position in the world.
         *
         * @param position The position vector.
         */
        void set_position(const glm::vec3& position) noexcept;

        /**
         * @brief Sets the movement speed of the camera.
         *
         * @param speed Movement speed.
         */
        void set_speed(float speed) noexcept;

        /**
         * @brief Sets the mouse sensitivity for camera rotation.
         *
         * @param sensitivity Mouse sensitivity.
         */
        void set_sensitivity(float sensitivity) noexcept;

        /**
         * @brief Sets the field of view (FOV) for the projection matrix.
         *
         * @param fov Field of view in degrees.
         */
        void set_fov(float fov) noexcept;

        /**
         * @brief Gets the camera's current position.
         *
         * @return The position vector.
         */
        [[nodiscard]] glm::vec3 get_position() const noexcept;

        /**
         * @brief Equality operator.
         *
         * Compares two Camera objects based on their positions and orientations.
         *
         * @param other The other Camera object to compare with.
         * @return True if both cameras have the same position and orientation, false otherwise.
         */
        bool operator==(const Camera& other) const noexcept;

        /**
         * @brief Inequality operator.
         *
         * Compares two Camera objects based on their positions and orientations.
         *
         * @param other The other Camera object to compare with.
         * @return True if the cameras have different positions or orientations, false otherwise.
         */
        bool operator!=(const Camera& other) const noexcept;

    private:
        /**
         * @brief Checks if the required attributes are set before initialization.
         *
         * @return True if attributes are set, false otherwise.
         */
        [[nodiscard]] bool are_attributes_set() const noexcept;

        /**
         * @brief Processes keyboard inputs to move the camera.
         *
         * @param window Pointer to the GLFW window.
         */
        void process_keyboard_input(GLFWwindow* window);

        /**
         * @brief Processes mouse inputs to rotate the camera.
         *
         * @param window Pointer to the GLFW window.
         */
        void process_mouse_input(GLFWwindow* window);

        glm::vec3 position_{ 0.0f, 0.0f, 0.0f };       ///< Camera position.
        glm::vec3 orientation_{ 0.0f, 0.0f, -1.0f };   ///< Camera forward direction.
        glm::vec3 up_{ 0.0f, 1.0f, 0.0f };             ///< Camera up direction.

        bool first_click_{ true };                     ///< Flag to prevent sudden jumps on first click.

        int width_{ 800 };                             ///< Window width.
        int height_{ 600 };                            ///< Window height.

        float speed_{ 0.01f };                          ///< Movement speed.
        float sensitivity_{ 10.5f };                    ///< Mouse sensitivity.

        float fov_{ 45.0f };                           ///< Field of view.
        float near_plane_{ 0.1f };                     ///< Near clipping plane.
        float far_plane_{ 100.0f };                    ///< Far clipping plane.

        GLint projection_matrix_location_{ -1 };       ///< Shader uniform location for projection matrix.
        GLint view_matrix_location_{ -1 };             ///< Shader uniform location for view matrix.
    };

} // namespace Engine
