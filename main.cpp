#include "window.h"
#include "instance.h"
#include "logic_dev.h"
#include "swap_chain.h"
#include "pipeline.h"
#include "frame_buffers.h"

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
	Pipeline pipeline{device.get(), swap_chain};
	Frame_buffers frame_buffers{device.get(), pipeline.render_pass().get(), swap_chain};

	while(!window.should_close()){
		window.poll();
	}

	return 0;
}
