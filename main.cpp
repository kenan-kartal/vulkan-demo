#include "window.h"

int main() {
	Window window{};

	while(!window.should_close()){
		window.poll();
	}

	return 0;
}
