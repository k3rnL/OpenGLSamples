//
// Created by Erwan on 10-May-20.
//

#include "FrameCounter.h"

FrameCounter::FrameCounter() {
    _frameCount = 0;
    _lastFps = 0;
    _lastRenderTime = 0;
    _lastSecond = std::chrono::system_clock::now();
}

float FrameCounter::fps() const {
    return _lastFps;
}

float FrameCounter::renderTime() const {
    return _lastRenderTime;
}

void FrameCounter::beginFrame() {
    _lastFrame = std::chrono::system_clock::now();
}

void FrameCounter::endFrame() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> diff = now - _lastSecond;
    if (diff.count() >= 1.f) {
        _lastSecond = now;
        _lastFps = _frameCount;
        _frameCount = 0;
    }
    else
        _frameCount++;
    _lastRenderTime = (now - _lastFrame).count() / 1000.f;
}
