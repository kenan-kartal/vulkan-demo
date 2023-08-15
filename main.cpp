#include "window.h"
#include "instance.h"

int main() {
	Window window{};
	Instance instance{};

	while(!window.should_close()){
		window.poll();
	}

	return 0;
}
