#pragma once

#include <iostream>
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
    
    void Stop();
    
public:
    Timer();
    ~Timer();
};
