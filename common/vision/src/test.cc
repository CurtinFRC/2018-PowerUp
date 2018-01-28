#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>

std::mutex lock;
std::condition_variable cv;

std::string shared;

void worker1() {
	std::cout << "Worker 1 (waiting)" << std::endl;
	{
		std::unique_lock<std::mutex> l(lock);
		cv.wait(l);
		// wait until ready
		
		std::cout << "Worker 1 (active)" << std::endl;
		std::cout << "W1 -> " << shared << std::endl;
		l.unlock();
	 }
}

void worker2() {
	std::cout << "Worker 2 (starting)" << std::endl;
	{
		std::lock_guard<std::mutex> l(lock);
		shared = "Hello World";
	}
	std::cout << "Worker 2 (Preparing to notify)" << std::endl;
	cv.notify_all();
	std::cout << "Worker 2 (done)" << std::endl;
}

int main() {
	std::thread w1(worker1);
	w1.detach();

	std::thread w2(worker2);
	w2.join();
	return 0;
}
