all:
	g++ main.cpp instance.cpp debug_messenger.cpp phys_dev.cpp -g -std=c++17 -L/usr/lib/x86_64-linux-gnu -lvulkan -lglfw