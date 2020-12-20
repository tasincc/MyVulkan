#include "FrameRate.h"

void FrameRate::Update() {
	static auto last = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	delta_ = now - last;
	last = now;
}
float FrameRate::GetFps() {
	static auto	last = std::chrono::steady_clock::now();
	static float delta_sum = 0;
	static unsigned times = 0;

	delta_sum += delta_.count();
	times++;

	auto now = std::chrono::steady_clock::now();
	if (now > last + std::chrono::seconds(1) && times) {
		fps_ = (float)times / delta_sum;
		delta_sum = 0;
		times = 0;
		last = now;
	}
	return fps_;
}