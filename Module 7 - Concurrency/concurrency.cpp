/*
This program is designed to create two threads to act as counters.
One thread should count up to 20. Once thread one reaches 20,
A second thread should be used to count down to 0.
*/


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Shared resources
std::mutex mtx; // Protects the shared counter and boolean flag
std::condition_variable cv; // Used to block/notify threads
int counter = 0; // The shared counter variable
bool countUpComplete = false; // Flag to indicate completion of the first task

// Thread 1 function: Counts up to 20
void countUp() {
    std::unique_lock<std::mutex> lck(mtx); // Acquire lock

    while (counter < 20) {
        counter++;
        std::cout << "Count Up From 0: " << counter << std::endl;
    }
    
    countUpComplete = true; // Set the condition flag
    std::cout << "Count Up thread finished. Notifying Count Down thread." << std::endl;
    lck.unlock(); // Unlock before notification
    cv.notify_one(); // Notify the waiting thread
}

// Thread 2 function: Counts down from 20 to 0
void countDown() {
    std::unique_lock<std::mutex> lck(mtx); // Acquire lock

    // Wait until the countUpComplete condition is true
    cv.wait(lck, []{ return countUpComplete; }); // The wait atomically unlocks the mutex and blocks, then reacquires the lock upon notification

    std::cout << "Count Down start." << std::endl;
    while (counter > 0) {
        counter--;
        std::cout << "Count Down From 20: " << counter << std::endl;
    }
    std::cout << "Count Down finished." << std::endl;
}

int main() {
    std::cout << "-----Thread Counting-----" << std::endl;

    // Create the two threads
    std::thread thread1(countUp);
    std::thread thread2(countDown);

    // Wait for both threads to complete
    thread1.join();
    thread2.join();

    std::cout << "Both threads finished. Exit main thread." << std::endl;

    return 0;
}