#include <iostream>
#include <thread>
#include <mutex>

void thread1();
void thread2();
void thread3();

std::mutex cout_mutex;

int main() {
	std::thread t1(thread1);
	std::thread t2(thread2);
	std::thread t3(thread3);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}

void thread1() {
	cout_mutex.lock();
	for(char c = 'a'; c <= 'z'; c++)
		std::cout << c << ' ';
	cout_mutex.unlock();
}

void thread2() {
	cout_mutex.lock();
	for(int i = 0; i <= 32; i++)
		std::cout << i << ' ';
	cout_mutex.unlock();
}

void thread3() {
	cout_mutex.lock();
	for(char c = 'A'; c <= 'Z'; c++)
		std::cout << c << ' ';
	cout_mutex.unlock();
}