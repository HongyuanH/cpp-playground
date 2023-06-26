#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
 
std::condition_variable cv;
std::mutex mtx; 
int i = 0;
 
void waits()
{
    std::unique_lock<std::mutex> lk(mtx); // condition_variable::wait() only accept unique_lock
    std::cerr << "Waiting... \n";
    cv.wait(lk, []{return i == 1;}); // release lock when waiting
    std::cerr << "...First pass. i == 1\n"; // acquire lock again when awaken

    std::cerr << "Waiting again... \n";
    cv.wait(lk, []{return i == 2;});
    std::cerr << "...Second pass. i == 2\n";    
}
 
void signals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lk(mtx); // lock for writing to i, to avoid deadlock
        i = 0;
        std::cerr << "Notifying all, i == 0...\n";
    }
    cv.notify_all(); // notification can be done outside of the lock
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lk(mtx);
        i = 1;
        std::cerr << "Notifying all, i == 1...\n";
    }
    cv.notify_all();
 
    for (int k=0; k<4; k++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> lk(mtx);
            i = 2;
            std::cerr << "Notifying one, i == 2...\n";
        }
        cv.notify_one();
    }
}
 
int main()
{
    std::thread t0(signals);
    std::thread t1(waits), t2(waits), t3(waits);
    t0.join();
    t1.join(); 
    t2.join(); 
    t3.join();
}