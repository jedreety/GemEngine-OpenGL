#include <Gem/Core/timer.h>
#include <iostream>
#include <thread>

namespace Gem {

	namespace Core {

		Timer::Timer(): debugFps_(0.0) {

			start();
		}

		void Timer::start() {

			std::lock_guard<std::mutex> lock(mutex_);

			startTime_ = Clock::now();
			lastTime_ = startTime_;
			
			elapsedTime_ = 0.0;
			deltaTime_ = 0.0;

			fpsAccumulator_ = 0.0f;
			frameCount_ = 0;
		}

		void Timer::update(double maxFps) {
			double currentDeltaTime = 0.0;

			//Calculate Delta Time
			{
				std::lock_guard<std::mutex> lock(mutex_);

				TimePoint currentTime = Clock::now();

				// Calculate Elapsed Time and Delta Time
				elapsedTime_ = std::chrono::duration<double>(currentTime - startTime_).count();
				deltaTime_ = std::chrono::duration<double>(currentTime - lastTime_).count();

				lastTime_ = currentTime;
				currentDeltaTime = deltaTime_;
			}

			//FPS Capping (Performed Outside Mutex)
			if (maxFps > 0.0) {
				double desiredFrameTime = 1.0 / maxFps;

				if (currentDeltaTime < desiredFrameTime) {
					double sleepTime = desiredFrameTime - currentDeltaTime;
					std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));

					// After Sleep: Update Timing Without Mutex
					TimePoint afterSleep = Clock::now();
					double actualSleepTime = std::chrono::duration<double>(afterSleep - (lastTime_)).count();

					{
						std::lock_guard<std::mutex> lock(mutex_);

						// Update Elapsed Time and Delta Time
						elapsedTime_ += actualSleepTime;
						deltaTime_ = desiredFrameTime;
						lastTime_ = afterSleep;
					}
				}
			}

			//FPS Logging
			if (debugFps_ > 0.0) {
				std::lock_guard<std::mutex> lock(mutex_);

				frameCount_++;
				fpsAccumulator_ += deltaTime_;

				if (fpsAccumulator_ >= debugFps_) {
					double fps = static_cast<double>(frameCount_) / fpsAccumulator_;
					std::cout << "FPS: " << fps << std::endl;

					frameCount_ = 0;
					fpsAccumulator_ = 0.0;
				}
			}
		}

		double Timer::getElapsedTime() const {

			std::lock_guard<std::mutex> lock(mutex_);
			return elapsedTime_;
		}

		double Timer::getDeltaTime() const {

			std::lock_guard<std::mutex> lock(mutex_);
			return deltaTime_;
		}

		double Timer::getElapsedMillis() const {

			std::lock_guard<std::mutex> lock(mutex_);
			return elapsedTime_ * 1000.0;
		}

		double Timer::getDeltaMillis() const {

			std::lock_guard<std::mutex> lock(mutex_);
			return deltaTime_ * 1000.0;
		}

		void Timer::set_debugFps(unsigned int debugFps) {

			std::lock_guard<std::mutex> lock(mutex_);
			debugFps_ = debugFps;       
		}

	} // namespace Core

} // namespace Gem
