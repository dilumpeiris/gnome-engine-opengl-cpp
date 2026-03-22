#pragma once
#include <chrono>

namespace Gnome {

class GnomeTime {
public:
    static float deltaTime;
    static float totalTime;

    static void init();
    static void update();

    // Utility for convenience
    static float getDeltaTime() { return deltaTime; }
    static float getTotalTime() { return totalTime; }

private:
    static std::chrono::high_resolution_clock::time_point startTime;
    static std::chrono::high_resolution_clock::time_point lastFrameTime;
};

} // namespace Gnome
