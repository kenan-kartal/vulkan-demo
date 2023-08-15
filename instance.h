#ifndef INSTANCE_H
#define INSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

extern std::vector<const char*> required_layers;
extern std::vector<const char*> required_extensions;

bool has_required_layers();
bool has_required_extensions();

class Instance {
public:
	Instance() {
		bool has_support = has_required_layers() && has_required_extensions();
		if (!has_support)
			return;
		VkApplicationInfo app_info {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Vulkan Demo",
			.applicationVersion = VK_API_VERSION_1_0,
			.pEngineName = "No Engine",
			.engineVersion = VK_API_VERSION_1_0,
			.apiVersion = VK_API_VERSION_1_0
		};
		VkInstanceCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = static_cast<uint32_t>(required_layers.size()),
			.ppEnabledLayerNames = required_layers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
			.ppEnabledExtensionNames = required_extensions.data(),
		};
		VkResult res = vkCreateInstance(&create_info, nullptr, &_instance);
		_initialised = res == VK_SUCCESS;
	}

	~Instance() {
		if (!_initialised)
			return;
		vkDestroyInstance(_instance, nullptr);
	}

private:
	VkInstance _instance;
	bool _initialised{};
};

#endif
