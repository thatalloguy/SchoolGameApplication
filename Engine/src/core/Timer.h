//
// Created by allos on 12/22/2024.
//
#pragma once

#include <thread>

namespace Tyche {


    class Timer {

    public:
        Timer() = default;
        ~Timer() = default;
        Timer(int milliseconds);

        void setWaitTime(int milliseconds);

        template<typename F>
        void start(F callback) {
            _clear = false;

            std::thread t([=, this]() {
                //First check if the timer is still active, then we can start the sleep
                if (this->_clear)
                    return;

                std::this_thread::sleep_for(std::chrono::milliseconds(_milliseconds));

                // Check again if we may have been stopped during our sleep
                if (this->_clear)
                    return;

                // Run the callback;
                callback();
            });

            t.detach();
        }

        void stop();

    private:
        bool _clear = false;
        long _milliseconds = 0.0;
    };


}