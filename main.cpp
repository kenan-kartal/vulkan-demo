#include "window.h"
#include "instance.h"
#include "logic_dev.h"
#include "swap_chain.h"

#include "config.h"

#include <memory>

int main() {
	Window window{};
	Instance instance{};
	std::unique_ptr<Debug_messenger> messenger;
	if (enable_validation_layers)
		messenger = std::make_unique<Debug_messenger>(instance.get());
	Logical_device device{instance.get(), window.get()};
	Swap_chain swap_chain{device, window.get()};

	while(!window.should_close()){
		window.poll();
	}

	return 0;
}
