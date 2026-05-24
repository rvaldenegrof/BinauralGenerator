#include "AndroidTimerController.h"

AndroidTimerController::AndroidTimerController() = default;
AndroidTimerController::~AndroidTimerController() = default;

void AndroidTimerController::start (int durationSeconds)
{
    stop();

    remainingSeconds = durationSeconds;
    running = true;
    startTimer (1000);

    if (onTick)
        onTick (remainingSeconds);
}

void AndroidTimerController::stop()
{
    if (running)
    {
        stopTimer();
        running = false;
    }
}

bool AndroidTimerController::isRunning() const noexcept
{
    return running;
}

void AndroidTimerController::timerCallback()
{
    if (--remainingSeconds <= 0)
    {
        stop();
        running = false;

        if (onTick)
            onTick (0);

        if (onFinish)
            onFinish();
    }
    else if (onTick)
    {
        onTick (remainingSeconds);
    }
}
