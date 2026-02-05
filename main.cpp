#include <iostream>
#include <chrono>
#include <thread>

#include "Constants.h"
#include "SystemStatus.h"

#include "Debug.h"

[[noreturn]] int main() {
	std::ios_base::sync_with_stdio(false);

	SystemStatus sys(DEFAULT_MODE);
	auto next_tick = std::chrono::steady_clock::now();

	while (true) {
		next_tick += std::chrono::milliseconds(MSPT);

		sys.tick();
		std::cout << sys.get_status() << std::endl;

		std::this_thread::sleep_until(next_tick);
	}
}