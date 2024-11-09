#pragma once

#include <GlfwGlad.h>
#include <iostream>
#include "../Camera/engine_camera.h"
#include "../Input/engine_inputs.h"

namespace Engine {

    /**
     * @brief Callback function for window resize events.
     *
     * Updates the viewport and notifies the camera of the new window dimensions.
     *
     * @param window Pointer to the GLFW window.
     * @param width New width of the window.
     * @param height New height of the window.
     */
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    /**
     * @brief Callback function for key events.
     *
     * Update the Inputs and closes the window when the Escape key is pressed.
     *
     * @param window Pointer to the GLFW window.
     * @param key Keyboard key that was pressed or released.
     * @param scancode System-specific scancode of the key.
     * @param action GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT.
     * @param mods Bit field describing which modifier keys were held down.
     */
     void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

     /**
      * @brief Callback function for Mouse events.
      *
      * Update the mouse button Inputs.
      *
      * @param window Pointer to the GLFW window.
      * @param button Mouse button that was pressed or released.
      * @param scancode System-specific scancode of the key.
      * @param action GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT.
      * @param mods Bit field describing which modifier keys were held down.
      */
     void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    /**
     * @brief Window class for managing the GLFW window and context.
     *
     * The Window class encapsulates the creation and management of a GLFW window,
     * including context setup, input callbacks, and frame management.
     */
    class Window {
    public:
        /**
         * @brief Constructs a Window object with default parameters.
         */
        Window();

        /**
         * @brief Destructor that cleans up the GLFW window.
         */
        ~Window();

        /**
         * @brief Initializes the window.
         *
         * Creates the GLFW window, sets up the context, callbacks, and VSync.
         *
         * @throws std::runtime_error if required attributes are not set or window creation fails.
         */
        void init();

        /**
         * @brief Checks if the window attributes are set.
         *
         * @return True if attributes are set, false otherwise.
         */
        [[nodiscard]] bool are_attributes_set() const noexcept;

        /**
         * @brief Clears the color and depth buffers.
         */
        void clear_frame() const;

        /**
         * @brief Prepares the frame for rendering.
         *
         * Clears the buffers and sets the background color.
         */
        void pre_frame() const;

        /**
         * @brief Finalizes the frame after rendering.
         *
         * Swaps buffers and polls for events.
         */
        void post_frame() const;

        /**
         * @brief Checks if the window should close.
         *
         * @return True if the window should close, false otherwise.
         */
        [[nodiscard]] bool should_close() const noexcept;

        /**
         * @brief Sets the window dimensions and title.
         *
         * @param width Window width in pixels.
         * @param height Window height in pixels.
         * @param title Window title.
         */
        void set_attributes(int width, int height, const char* title);

        /**
         * @brief Sets the window width.
         *
         * @param width Window width in pixels.
         */
        void set_width(int width) noexcept;

        /**
         * @brief Sets the window height.
         *
         * @param height Window height in pixels.
         */
        void set_height(int height) noexcept;

        /**
         * @brief Sets the window title.
         *
         * @param title Window title.
         */
        void set_title(const char* title) noexcept;

        /**
         * @brief Enables or disables VSync.
         *
         * @param vsync True to enable VSync, false to disable.
         */
        void set_vsync(bool vsync) noexcept;

        /**
         * @brief Sets the camera associated with the window.
         *
         * @param camera Pointer to the Camera object.
         */
        void set_camera(Camera* camera) noexcept;

        /**
         * @brief Gets the window width.
         *
         * @return Window width in pixels.
         */
        [[nodiscard]] int get_width() const noexcept;

        /**
         * @brief Gets the window height.
         *
         * @return Window height in pixels.
         */
        [[nodiscard]] int get_height() const noexcept;

        /**
         * @brief Gets the pointer to the GLFW window.
         *
         * @return Pointer to the GLFW window.
         */
        [[nodiscard]] GLFWwindow* get_window_ptr() const noexcept;

        /**
         * @brief Gets the pointer to the associated Camera object.
         *
         * @return Pointer to the Camera object.
         */
        [[nodiscard]] Camera* get_camera() const noexcept;

        /**
		 * @brief Gets the pointer to the Inputs object.
         * 
         * @return Pointer to the Inputs object.
         */
		[[nodiscard]] Inputs* get_inputs() const noexcept;

        /**
         * @brief Equality operator.
         *
         * Compares two Window objects based on their GLFW window pointers.
         *
         * @param other The other Window object to compare with.
         * @return True if both have the same GLFW window pointer, false otherwise.
         */
        bool operator==(const Window& other) const noexcept;

        /**
         * @brief Inequality operator.
         *
         * Compares two Window objects based on their GLFW window pointers.
         *
         * @param other The other Window object to compare with.
         * @return True if they have different GLFW window pointers, false otherwise.
         */
        bool operator!=(const Window& other) const noexcept;

    private:
        /**
         * @brief Creates the GLFW window.
         *
         * @throws std::runtime_error if window creation fails.
         */
        void create_window();

        /**
         * @brief Makes the window's context current.
         */
        void make_context_current() const;

        /**
         * @brief Sets the input callbacks for the window.
         */
        void set_callbacks();

        GLFWwindow* window_ = nullptr;                  ///< Pointer to the GLFW window.
        const char* title_ = "Default window name";     ///< Window title.
        int width_ = 800;                               ///< Window width in pixels.
        int height_ = 600;                              ///< Window height in pixels.
        bool vsync_ = true;                             ///< VSync enabled flag.
        Camera* camera_ = nullptr;                      ///< Pointer to the associated Camera object.
		Inputs* inputs_;    				            ///< Pointer to the Inputs object.      
    };

} // namespace Engine
