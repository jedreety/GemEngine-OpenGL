#pragma once

#include <GlfwGlad.h>
#include <iostream>
#include <functional>
#include <Gem/Graphics/camera.h>
#include <Gem/Input/inputs.h>

namespace Gem {

    namespace Window {

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
             * 
			 * @param width Window width in pixels.
			 * @param height Window height in pixels.
			 * @param title Window title.
             * @param vsync True to enable VSync, false to disable.
             */
            Window(int width = 800, int height = 600, const char* title = "Default window name", bool vsync = true);

            /**
             * @brief Destructor that cleans up the GLFW window.
             */
            ~Window();

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
             * @brief Sets the camera associated with the window.
             *
             * @param camera Pointer to the Camera object.
			 * @param update_dimentions Tell if the camera's dimensions should be updated to match the window's viewport.
             */
            void set_camera(Graphics::Camera* camera, bool update_dimentions = true) noexcept;

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
            [[nodiscard]] Graphics::Camera* get_camera() const noexcept;

            /**
             * @brief Gets the pointer to the Inputs object.
             *
             * @return Pointer to the Inputs object.
             */
            [[nodiscard]] Input::Inputs* get_inputs() const noexcept;

            /**
             * @brief Sets a user-provided framebuffer size callback function.
             *
             * Allows the user to specify a custom callback function that will be called
             * whenever the window is resized. The user's callback will be invoked in addition
             * to the default framebuffer size callback.
             *
             * @param callback The user-provided callback function.
             */
            void set_framebuffer_size_callback(std::function<void(GLFWwindow*, int, int)> callback);

            /**
             * @brief Sets a user-provided key callback function.
             *
             * Allows the user to specify a custom callback function that will be called
             * whenever a key event occurs. The user's callback will be invoked in addition
             * to the default key callback.
             *
             * @param callback The user-provided callback function.
             */
            void set_key_callback(std::function<void(GLFWwindow*, int, int, int, int)> callback);

            /**
             * @brief Sets a user-provided mouse button callback function.
             *
             * Allows the user to specify a custom callback function that will be called
             * whenever a mouse button event occurs. The user's callback will be invoked in addition
             * to the default mouse button callback.
             *
             * @param callback The user-provided callback function.
             */
            void set_mouse_button_callback(std::function<void(GLFWwindow*, int, int, int)> callback);

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

        public:

			std::function<void(GLFWwindow*, int, int)> user_framebuffer_size_callback_;   ///< User-provided framebuffer size callback.
			std::function<void(GLFWwindow*, int, int, int, int)> user_key_callback_;      ///< User-provided key callback.
			std::function<void(GLFWwindow*, int, int, int)> user_mouse_button_callback_;  ///< User-provided mouse button callback.

        private:

            /**
             * @brief Initializes the window.
             *
             * Creates the GLFW window, sets up the context, callbacks, and VSync.
             *
             * @throws std::runtime_error if required attributes are not set or window creation fails.
             */
            void init();

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

        private:

            GLFWwindow* window_ = nullptr;         ///< Pointer to the GLFW window.
            const char* title_;                    ///< Window title.
            int width_;                            ///< Window width in pixels.
            int height_;                           ///< Window height in pixels.
            bool vsync_;                           ///< VSync enabled flag.
            Graphics::Camera* camera_ = nullptr;   ///< Pointer to the associated Camera object.
            Input::Inputs* inputs_;    		       ///< Pointer to the Inputs object.      

        };

	} // namespace Window

} // namespace Gem
