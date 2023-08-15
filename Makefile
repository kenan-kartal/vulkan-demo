all:
	g++ main.cpp instance.cpp -std=c++17 -g -L/usr/lib/x86_64-linux-gnu -lvulkan -lglfw