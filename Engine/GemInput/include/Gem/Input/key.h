#pragma once

#include <GlfwGlad.h>

namespace Gem {

    namespace Input {

        /**
         * @brief Represents the state of a single key or mouse button.
         *
         * The Key class tracks whether a key is currently pressed, was pressed in this frame,
         * or was released in this frame.
         */
        class Key {

        public:

            /**
             * @brief Constructs a Key object with default state.
             */
            Key();

            /**
             * @brief Updates the key's state based on the current pressed state.
             *
             * @param pressed_now True if the key is currently pressed, false otherwise.
             */
            void update(bool pressed_now);

            /**
             * @brief Resets the per-frame key states.
             *
             * Should be called at the beginning or end of each frame to reset temporary states.
             */
            void reset();

            /**
             * @brief Checks if the key is currently pressed.
             *
             * @return True if the key is pressed, false otherwise.
             */
            [[nodiscard]] bool is_pressed() const noexcept;

            /**
             * @brief Checks if the key was pressed in this frame.
             *
             * @return True if the key was pressed this frame, false otherwise.
             */
            [[nodiscard]] bool was_pressed() const noexcept;

            /**
             * @brief Checks if the key was released in this frame.
             *
             * @return True if the key was released this frame, false otherwise.
             */
            [[nodiscard]] bool was_released() const noexcept;

        private:

            bool is_pressed_ = false;   ///< Indicates if the key is currently pressed.
            bool was_pressed_ = false;  ///< Indicates if the key was pressed in this frame.
            bool was_released_ = false; ///< Indicates if the key was released in this frame.

        };

    } // namespace Input

} // namespace Gem
