#ifndef TIMER_HPP
#define TIMER_HPP

#pragma once 
#include <chrono>

namespace engine {
    namespace core {
        class Timer {
        private:
            using Clock = std::chrono::steady_clock;
            using TimePoint = std::chrono::time_point<Clock>;
            using Duration = std::chrono::duration<double>;

            static TimePoint startTime;
            static TimePoint lastFrameTime;
            static TimePoint currentFrameTime;
            static double deltaTime;
            static bool paused;

        public:
            Timer() = delete;

            static void initialitation();                
            static void update();

            static double getDeltaTime();
            static double getTimeSinceStart();
            static float getFPS();
            static void pause();
            static void resume();
        };
    }
}

#endif // TIMER_HPP