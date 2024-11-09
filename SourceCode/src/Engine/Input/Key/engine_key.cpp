#include "engine_key.h"

namespace Engine {
	namespace Input {

		// Constructs a Key object with default state
		Key::Key()
			: is_pressed_(false),
			  was_pressed_(false),
			  was_released_(false) {
			// Default initialization
		}

		// Updates the key's state based on the current pressed state
		void Key::update(bool pressed_now) {
			// Determine if the key was pressed or released in this frame
			was_pressed_ = !is_pressed_ && pressed_now;
			was_released_ = is_pressed_ && !pressed_now;

			// Update the current pressed state
			is_pressed_ = pressed_now;
		}

		// Resets the per-frame key states
		void Key::reset() {
			// Reset the temporary states
			was_pressed_ = false;
			was_released_ = false;
		}

		// Checks if the key is currently pressed
		bool Key::is_pressed() const noexcept {
			return is_pressed_;
		}

		// Checks if the key was pressed in this frame
		bool Key::was_pressed() const noexcept {
			return was_pressed_;
		}

		// Checks if the key was released in this frame
		bool Key::was_released() const noexcept {
			return was_released_;
		}

	} // namespace Input
} // namespace Engine
