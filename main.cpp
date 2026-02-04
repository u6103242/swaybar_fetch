#include <iostream>
#include <chrono>
#include <thread>

#include "Constants.h"
#include "SystemStatus.h"

[[noreturn]] int main() {
	ios_base::sync_with_stdio(false);

	SystemStatus sys(DEFAULT_MODE);
	auto next_tick = chrono::steady_clock::now();

	while (true) {
		next_tick += chrono::milliseconds(MSPT);

		sys.tick();
		cout << sys.get_status() << endl;

		this_thread::sleep_until(next_tick);
	}
}