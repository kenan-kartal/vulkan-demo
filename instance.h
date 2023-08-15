#ifndef INSTANCE_H
#define INSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class Instance {
public:
	Instance() {
		print_available_extensions();
		VkApplicationInfo app_info {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Vulkan Demo",
			.applicationVersion = VK_API_VERSION_1_0,
			.pEngineName = "No Engine",
			.engineVersion = VK_API_VERSION_1_0,
			.apiVersion = VK_API_VERSION_1_0
		};
		uint32_t glfw_extension_count{};
		const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
		VkInstanceCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = 0,
			.enabledExtensionCount = glfw_extension_count,
			.ppEnabledExtensionNames = glfw_extensions,
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
	void print_available_extensions() const {
		uint32_t extension_count{};
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
		std::cout << "Available extensions:\n";
		for (const auto& extension: extensions) {
			std::cout << '\t' << extension.extensionName << '\n';
		}
	}

private:
	VkInstance _instance;
	bool _initialised{};
};

#endif
