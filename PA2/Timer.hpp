#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <iostream>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
    // 開始計時
    void start() {
        start_time = std::chrono::steady_clock::now();
    }

    // 結束計時並返回經過的時間（以毫秒為單位）
    long long stop() {
        auto end_time = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    }
};

#endif // TIMER_HPP
