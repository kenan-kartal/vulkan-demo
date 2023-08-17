SOURCES = main.cpp instance.cpp debug_messenger.cpp phys_dev.cpp swap_chain.cpp pipeline.cpp

.PHONY: source shaders

all: shaders source

shaders:
	glslc shaders/shader.vert -o build/vert.spv
	glslc shaders/shader.frag -o build/frag.spv

source: shaders
	g++ $(SOURCES) -g -std=c++17 -L/usr/lib/x86_64-linux-gnu -lvulkan -lglfw
