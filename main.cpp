#include "window.h"
#include "instance.h"
#include "config.h"

int main() {
	Window window{};
	Instance instance{};
	std::unique_ptr<Debug_messenger> messenger;
	if (enable_validation_layers)
		messenger = std::make_unique<Debug_messenger>(instance.get());

	while(!window.should_close()){
		window.poll();
	}

	return 0;
}
