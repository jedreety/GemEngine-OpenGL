#include <Gem/Input/inputs.h>
#include <iostream> // For error logging

namespace Gem {

	namespace Input {

		// Constructor
		Inputs::Inputs() {
			// Initialize all key states
			keys_.fill(Input::Key());
		}

		// Processes a key event and updates the corresponding key state
		void Inputs::key_callback(int keyCode, int action) {
			// Ensure keyCode is within valid bounds for keyboard keys
			if (keyCode >= 0 && keyCode <= GLFW_KEY_LAST) {
				// Determine if the key is pressed or released
				bool pressed = action != GLFW_RELEASE;

				// Update the key state in the keys_ array
				keys_[keyCode].update(pressed);
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::key_callback: Invalid keyCode (" << keyCode << ")." << std::endl;
			}
		}

		// Processes a mouse button event and updates the corresponding key state
		void Inputs::mouse_button_callback(int button, int action) {
			// Map the mouse button code into the key array using the offset
			int keyCode = button + MOUSE_BUTTON_OFFSET;

			// Ensure keyCode is within valid bounds for mouse buttons
			if (keyCode >= MOUSE_BUTTON_OFFSET && keyCode < MAX_KEYS) {
				// Determine if the button is pressed or released
				bool pressed = action != GLFW_RELEASE;

				// Update the key state in the keys_ array
				keys_[keyCode].update(pressed);
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::mouse_button_callback: Invalid keyCode (" << keyCode
					<< ") for button (" << button << ")." << std::endl;
			}
		}

		// Resets per-frame key and mouse button states
		void Inputs::update() {
			// Reset the temporary states for all keys and mouse buttons
			for (auto& key : keys_) {
				key.reset();
			}
		}

		// Checks if a key is currently pressed
		bool Inputs::is_key_pressed(int keyCode) const noexcept {
			// Ensure keyCode is within valid bounds for keyboard keys
			if (keyCode >= 0 && keyCode <= GLFW_KEY_LAST) {
				// Return the current pressed state of the key
				return keys_[keyCode].is_pressed();
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::is_key_pressed: Invalid keyCode (" << keyCode << ")." << std::endl;
				return false;
			}
		}

		// Checks if a key was pressed in this frame
		bool Inputs::was_key_pressed(int keyCode) const noexcept {
			// Ensure keyCode is within valid bounds for keyboard keys
			if (keyCode >= 0 && keyCode <= GLFW_KEY_LAST) {
				// Return true if the key was pressed this frame
				return keys_[keyCode].was_pressed();
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::was_key_pressed: Invalid keyCode (" << keyCode << ")." << std::endl;
				return false;
			}
		}

		// Checks if a key was released in this frame
		bool Inputs::was_key_released(int keyCode) const noexcept {
			// Ensure keyCode is within valid bounds for keyboard keys
			if (keyCode >= 0 && keyCode <= GLFW_KEY_LAST) {
				// Return true if the key was released this frame
				return keys_[keyCode].was_released();
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::was_key_released: Invalid keyCode (" << keyCode << ")." << std::endl;
				return false;
			}
		}

		// Checks if a mouse button is currently pressed
		bool Inputs::is_mouse_button_pressed(int button) const noexcept {
			// Map the mouse button code into the key array using the offset
			int keyCode = button + MOUSE_BUTTON_OFFSET;

			// Ensure keyCode is within valid bounds for mouse buttons
			if (keyCode >= MOUSE_BUTTON_OFFSET && keyCode < MAX_KEYS) {
				// Return the current pressed state of the mouse button
				return keys_[keyCode].is_pressed();
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::is_mouse_button_pressed: Invalid keyCode (" << keyCode
					<< ") for button (" << button << ")." << std::endl;
				return false;
			}
		}

		// Checks if a mouse button was pressed in this frame
		bool Inputs::was_mouse_button_pressed(int button) const noexcept {
			// Map the mouse button code into the key array using the offset
			int keyCode = button + MOUSE_BUTTON_OFFSET;

			// Ensure keyCode is within valid bounds for mouse buttons
			if (keyCode >= MOUSE_BUTTON_OFFSET && keyCode < MAX_KEYS) {
				// Return true if the mouse button was pressed this frame
				return keys_[keyCode].was_pressed();
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::was_mouse_button_pressed: Invalid keyCode (" << keyCode
					<< ") for button (" << button << ")." << std::endl;
				return false;
			}
		}

		// Checks if a mouse button was released in this frame
		bool Inputs::was_mouse_button_released(int button) const noexcept {
			// Map the mouse button code into the key array using the offset
			int keyCode = button + MOUSE_BUTTON_OFFSET;

			// Ensure keyCode is within valid bounds for mouse buttons
			if (keyCode >= MOUSE_BUTTON_OFFSET && keyCode < MAX_KEYS) {
				// Return true if the mouse button was released this frame
				return keys_[keyCode].was_released();
			}
			else {
				// Log an error if the keyCode is invalid
				std::cerr << "ERROR::Inputs::was_mouse_button_released: Invalid keyCode (" << keyCode
					<< ") for button (" << button << ")." << std::endl;
				return false;
			}
		}

	} // namespace Input

} // namespace Gem
