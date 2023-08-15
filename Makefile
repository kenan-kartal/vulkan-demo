all:
	g++ main.cpp instance.cpp debug_messenger.cpp -std=c++17 -g -L/usr/lib/x86_64-linux-gnu -lvulkan -lglfw