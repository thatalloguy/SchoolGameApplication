//
// Created by allos on 12/28/2024.
//
#include "Timer.h"


Tyche::Timer::Timer(int milliseconds) {
    _milliseconds = milliseconds;
}

void Tyche::Timer::setWaitTime(int milliseconds) {
    _milliseconds = milliseconds;
}


void Tyche::Timer::stop() {
    _clear = true;
}