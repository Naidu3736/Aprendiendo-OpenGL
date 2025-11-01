#include "engine/core/timer.hpp"

using namespace engine::core;

Timer::TimePoint Timer::startTime;
Timer::TimePoint Timer::lastFrameTime;
Timer::TimePoint Timer::currentFrameTime;
double Timer::deltaTime = 0.0;
bool Timer::paused = false;

void Timer::initialitation() 
{
    startTime = Clock::now();
    lastFrameTime = startTime;
    currentFrameTime = startTime;
    deltaTime = 0.0;
    paused = false;
}

void Timer::update() 
{
    if (!paused) {
        currentFrameTime = Clock::now();
        Duration duration = currentFrameTime - lastFrameTime;
        deltaTime = duration.count();
        lastFrameTime = currentFrameTime;
    }
}

double Timer::getDeltaTime() 
{
    return !paused ? deltaTime : 0.0;
}

double Timer::getTimeSinceStart() 
{
    auto now = Clock::now();
    Duration duration = now - startTime;
    return duration.count();
}

float Timer::getFPS() 
{
    return deltaTime > 0 ? 1.0f / static_cast<float>(deltaTime) : 0.0f;
}

void Timer::pause() 
{ 
    paused = true; 
}
void Timer::resume() 
{ 
    paused = false; 
}       
