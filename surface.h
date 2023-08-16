#ifndef SURFACE_H
#define SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Surface {
public:
	Surface(VkInstance instance, GLFWwindow *window) :
		_instance{instance}
	{
		auto res = glfwCreateWindowSurface(_instance, window, nullptr, &_surface);
		_initialised = res == VK_SUCCESS;
	}

	~Surface() {
		if (!_initialised)
			return;
		vkDestroySurfaceKHR(_instance, _surface, nullptr);
	}

	VkSurfaceKHR get() const { return _surface; }

private:
	VkInstance _instance;
	VkSurfaceKHR _surface{};
	bool _initialised{};
};

#endif
