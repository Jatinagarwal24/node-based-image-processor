#include "App.h"
#include <chrono>
#include <thread>

App::App() {
    std::cout << "App constructed." << std::endl;
}

App::~App() {
    std::cout << "App destructed." << std::endl;
}

// A dummy run loop to simulate a window/render loop.
void App::run() {
    std::cout << "Starting application loop. Press Ctrl+C to exit." << std::endl;
    
    // Run a short loop (here we simulate 5 iterations)
    for (int i = 0; i < 5; ++i) {
        std::cout << "Loop iteration: " << i + 1 << std::endl;
        // Sleep for 1 second (simulate frame delay)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Exiting application loop." << std::endl;
}
