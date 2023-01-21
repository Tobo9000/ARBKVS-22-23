#include <iostream>
#include <thread>
#include "Semaphore.h"

void thread1();
void thread2();
void thread3();

Semaphore semaphore(1);

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
	semaphore.wait();
	for(char c = 'a'; c <= 'z'; c++)
		std::cout << c << ' ';
	semaphore.signal();
}

void thread2() {
	semaphore.wait();
	for(int i = 0; i <= 32; i++)
		std::cout << i << ' ';
	semaphore.signal();
}

void thread3() {
	semaphore.wait();
	for(char c = 'A'; c <= 'Z'; c++)
		std::cout << c << ' ';
	semaphore.signal();
}