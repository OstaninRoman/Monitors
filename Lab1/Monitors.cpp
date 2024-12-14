#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstdlib>
#include <ctime>

int message;
bool messageReady = false;
std::mutex mutex;
std::condition_variable condition;

void producer() {
    while (true) {
        int newMessage = rand() % 100 + 1;
        
        {
            std::unique_lock<std::mutex> lock(mutex);
            message = newMessage;
            messageReady = true;
            std::cout << "Produser " << newMessage << std::endl;
            condition.notify_one();
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void consumer() {
    while (true) {
        int receivedMessage;
        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [] { return messageReady; });
            receivedMessage = message;
            messageReady = false;
        }
        std::cout << "Consumer " << receivedMessage << std::endl;
    }
}

int main() {
    srand(time(0));
    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();

    return 0;
}
