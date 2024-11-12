#include <Gem/Core/scoped_timer.h>

namespace Gem {

	namespace Core {	

		std::mutex ScopedTimer::logMutex_;

		ScopedTimer::ScopedTimer(const std::string& name)
			: name_(name), timer_()
		{
			// Timer starts automatically upon construction
		}

		ScopedTimer::~ScopedTimer() {

			timer_.update();

			double elapsed = timer_.getElapsedMillis();

			std::lock_guard<std::mutex> lock(logMutex_);

			std::cout << "[TIMER] " << name_ << " ~ " << std::fixed << std::setprecision(3) << elapsed << " ms\n";
		}

	} // namespace Core

} // namespace Gem
