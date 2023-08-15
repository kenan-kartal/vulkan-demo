#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
public:
	Window(int width = 800, int height = 600) {
		_initialised = glfwInit() == GLFW_TRUE;
		if (!_initialised)
			return;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		_window = glfwCreateWindow(width, height, "Vulkan Demo", nullptr, nullptr);
	}

	~Window() {
		if(!_initialised)
			return;
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	bool should_close() const {
		return !_initialised || glfwWindowShouldClose(_window);
	}

	void poll() const {
		if(!_initialised)
			return;
		glfwPollEvents();
	}

private:
	GLFWwindow* _window{};
	bool _initialised{};
};

#endif
