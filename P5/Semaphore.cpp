//
// Created by tschn on 21.12.2022.
//

#include "Semaphore.h"

Semaphore::Semaphore(int count) : count(count) {}

void Semaphore::wait() {
	std::unique_lock<std::mutex> lock(mutex);
	if (--count < 0) {
		condition.wait(lock);
	}
}

void Semaphore::signal() {
	std::unique_lock<std::mutex> lock(mutex);
	if (++count <= 0) {
		condition.notify_one();
	}
}
