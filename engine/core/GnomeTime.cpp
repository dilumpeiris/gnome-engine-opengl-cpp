#include "GnomeTime.h"

namespace Gnome {

float GnomeTime::deltaTime = 0.0f;
float GnomeTime::totalTime = 0.0f;
std::chrono::high_resolution_clock::time_point GnomeTime::startTime;
std::chrono::high_resolution_clock::time_point GnomeTime::lastFrameTime;

void GnomeTime::init() {
	startTime = std::chrono::high_resolution_clock::now();
	lastFrameTime = startTime;
	deltaTime = 0.0f;
	totalTime = 0.0f;
}

void GnomeTime::update() {
	auto currentTime = std::chrono::high_resolution_clock::now();

	// Calculate deltaTime in seconds
	std::chrono::duration<float> elapsed = currentTime - lastFrameTime;
	deltaTime = elapsed.count();

	// Calculate totalTime in seconds
	std::chrono::duration<float> total = currentTime - startTime;
	totalTime = total.count();

	lastFrameTime = currentTime;
}

} // namespace Gnome
