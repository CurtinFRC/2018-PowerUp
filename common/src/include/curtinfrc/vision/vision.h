#pragma once

#include "WPILib.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <chrono>

namespace curtinfrc {
	class VisionSystem {
	public:
		using clock = std::chrono::system_clock;
		using tp_millis = std::chrono::time_point<clock, std::chrono::milliseconds>;

		void start();

	private:
		std::thread *_vision_thread;
	};
} // namespace curtinfrc