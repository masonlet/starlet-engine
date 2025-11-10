#pragma once

namespace Starlet::Engine {
	class Timer {
	public:
		Timer();

		float tick();

	private:
		float lastTime;
	};
}