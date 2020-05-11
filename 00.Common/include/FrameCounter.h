//
// Created by Erwan on 10-May-20.
//

#ifndef GEOMETRY_FRAMECOUNTER_H
#define GEOMETRY_FRAMECOUNTER_H

#include <chrono>

class FrameCounter {
public:
    FrameCounter();

    float fps() const;
    float renderTime() const;
    void beginFrame();
    void endFrame();
private:
    std::chrono::time_point<std::chrono::system_clock>  _lastSecond;
    std::chrono::time_point<std::chrono::system_clock>  _lastFrame;
    float                                               _lastFps;
    float                                               _lastRenderTime;
    int                                                 _frameCount;
};


#endif //GEOMETRY_FRAMECOUNTER_H
