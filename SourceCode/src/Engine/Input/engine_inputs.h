#pragma once

#include <array>
#include <GlfwGlad.h>
#include "Key/engine_key.h"

namespace Engine {

	/**
	* @brief Manages input states for keyboard keys and mouse buttons.
	*
	* The Inputs class tracks the state of keyboard keys and mouse buttons,
	* including whether they are currently pressed, were pressed in the current frame,
	* or were released in the current frame.
	*/
	class Inputs {

	public:
		/**
		* @brief Maximum number of keys and mouse buttons to handle.
		*
		* Calculated as the highest key code for keyboard keys and mouse buttons plus one.
		*/
		static constexpr int MAX_KEYS = GLFW_KEY_LAST + 1 + GLFW_MOUSE_BUTTON_LAST + 1;

		/**
		* @brief Offset value used to map mouse button codes into the key array.
		*
		* Mouse buttons are mapped into the key array by adding this offset to their codes.
		*/
		static constexpr int MOUSE_BUTTON_OFFSET = GLFW_KEY_LAST + 1;

		/**
		* @brief Constructs an Inputs object and initializes key states.
		*/
		Inputs();

		/**
		* @brief Processes a key event and updates the corresponding key state.
		*
		* @param keyCode The key code of the key event.
		* @param action The action associated with the key event (GLFW_PRESS, GLFW_RELEASE, etc.).
		*/
		void key_callback(int keyCode, int action);

		/**
		* @brief Processes a mouse button event and updates the corresponding key state.
		*
		* @param button The mouse button code received from GLFW (e.g., GLFW_MOUSE_BUTTON_LEFT).
		* @param action The action associated with the mouse button event (GLFW_PRESS, GLFW_RELEASE, etc.).
		*/
		void mouse_button_callback(int button, int action);

		/**
		* @brief Resets per-frame key and mouse button states.
		*
		* Should be called at the beginning or end of each frame to reset the temporary key states.
		*/
		void update();

		/**
		* @brief Checks if a key is currently pressed.
		*
		* @param keyCode The key code to check.
		* @return True if the key is pressed, false otherwise.
		*/
		[[nodiscard]] bool is_key_pressed(int keyCode) const noexcept;

		/**
		* @brief Checks if a key was pressed in this frame.
		*
		* @param keyCode The key code to check.
		* @return True if the key was pressed this frame, false otherwise.
		*/
		[[nodiscard]] bool was_key_pressed(int keyCode) const noexcept;

		/**
		* @brief Checks if a key was released in this frame.
		*
		* @param keyCode The key code to check.
		* @return True if the key was released this frame, false otherwise.
		*/
		[[nodiscard]] bool was_key_released(int keyCode) const noexcept;

		/**
		* @brief Checks if a mouse button is currently pressed.
		*
		* @param button The mouse button code to check (e.g., GLFW_MOUSE_BUTTON_LEFT).
		* @return True if the mouse button is pressed, false otherwise.
		*/
		[[nodiscard]] bool is_mouse_button_pressed(int button) const noexcept;

		/**
		* @brief Checks if a mouse button was pressed in this frame.
		*
		* @param button The mouse button code to check.
		* @return True if the mouse button was pressed this frame, false otherwise.
		*/
		[[nodiscard]] bool was_mouse_button_pressed(int button) const noexcept;

		/**
		* @brief Checks if a mouse button was released in this frame.
		*
		* @param button The mouse button code to check.
		* @return True if the mouse button was released this frame, false otherwise.
		*/
		[[nodiscard]] bool was_mouse_button_released(int button) const noexcept;

	private:
		/**
		* @brief Array of Key objects representing the state of each key and mouse button.
		*
		* Keyboard keys and mouse buttons are stored in this array. Mouse buttons are mapped
		* into this array using the MOUSE_BUTTON_OFFSET.
		*/
		std::array<Input::Key, MAX_KEYS> keys_;

	};

} // namespace Engine
