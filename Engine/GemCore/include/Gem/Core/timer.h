#pragma once

#include <chrono>
#include <mutex>

/**
 * @file Timer.h
 * @brief Declaration of the Timer class for time tracking.
 */

namespace Gem {
    namespace Core {

        /**
         * @class Timer
         * @brief Utility class for managing and tracking elapsed time and delta time.
         *
         * Provides functionality to track the total elapsed time since the timer was started,
         * as well as the time elapsed between consecutive updates (delta time).
         */
        class Timer {

        public:
            
            using Clock = std::chrono::steady_clock;
            using TimePoint = std::chrono::time_point<Clock>;

            /**
             * @brief Constructs a new Timer object and starts the timer.
             */
            Timer();

            /**
             * @brief Starts or restarts the timer.
             *
             * Resets the start and last time points to the current time and resets elapsed and delta times.
             */
            void start();

            /**
             * @brief Updates the timer by calculating the elapsed and delta times, and optionally caps the FPS.
             *
             * Should be called once per frame or at regular intervals to keep track of time progression.
             *
             * @param maxFps Optional maximum FPS to cap the frame rate. If set to 0, no FPS cap is applied.
             */
            void update(double maxFps = 0.0);

            /**
             * @brief Retrieves the total elapsed time since the timer was started.
             * @return Total elapsed time in seconds.
             */
            double getElapsedTime() const;

            /**
             * @brief Retrieves the time elapsed since the last update.
             * @return Delta time in seconds.
             */
            double getDeltaTime() const;

            /**
             * @brief Retrieves the total elapsed time in milliseconds.
             * @return Total elapsed time in milliseconds.
             */
            double getElapsedMillis() const;

            /**
             * @brief Retrieves the delta time in milliseconds.
             * @return Delta time in milliseconds.
             */
            double getDeltaMillis() const;

            /**
             * @brief Sets the interval in seconds at which the FPS is logged.
             *
             * If set to a value greater than zero, the Timer will print the current FPS
             * to the console every `debugFps` seconds during each update.
             *
             * @param debugFps The interval in seconds for FPS logging.
             */
            void set_debugFps(unsigned int debugFps);

			// Delete copy constructor and copy assignment operator
			Timer(const Timer&) = delete;
			Timer& operator=(const Timer&) = delete;

			// Delete move constructor and move assignment operator
			Timer(Timer&&) = delete;
			Timer& operator=(Timer&&) = delete;


        private:

			mutable std::mutex mutex_;    ///< Mutex to protect shared data.

            TimePoint startTime_;         ///< The time point when the timer was started.
            TimePoint lastTime_;          ///< The time point at the last update.

            double elapsedTime_;          ///< Total elapsed time in seconds.
            double deltaTime_;            ///< Delta time in seconds.

			float fpsAccumulator_;        ///< Accumulator for tracking elapsed time for FPS calculation.
			int frameCount_;              ///< Counts the number of frames within the FPS interval.
			double debugFps_;             ///< Interval in seconds to print the FPS. Set to 0 to disable FPS logging.

        };

    } // namespace Core
} // namespace Gem
