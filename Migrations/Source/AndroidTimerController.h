#pragma once

#include <juce_core/juce_core.h>
#include <functional>

class AndroidTimerController : private juce::Timer
{
public:
    AndroidTimerController();
    ~AndroidTimerController() override;

    void start (int durationSeconds);
    void stop();
    bool isRunning() const noexcept;

    std::function<void(int)> onTick;
    std::function<void()> onFinish;

private:
    void timerCallback() override;

    int remainingSeconds = 0;
    bool running = false;
};
