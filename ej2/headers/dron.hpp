#pragma once

#include <thread>
#include <mutex>
#include <string>

class Dron {
    private:
        int id;
        std::mutex& leftZone;
        std::mutex& rightZone;
        std::thread thread;
        void takeoff();
    public:
        Dron(int id, std::mutex& leftZone, std::mutex& rightZone);
        void start();
        void join();
        ~Dron();
};