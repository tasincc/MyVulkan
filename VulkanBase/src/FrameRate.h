#ifndef FRAMERATE_HPP
#define FRAMERATE_HPP

#include<chrono>

class FrameRate {
public:
	FrameRate() :fps_(0), delta_(0) {}
	float GetDelta() const { return delta_.count(); }
	void Update();
	float GetFps();
private:
	float fps_;
	std::chrono::duration<float> delta_;
};

#endif 
