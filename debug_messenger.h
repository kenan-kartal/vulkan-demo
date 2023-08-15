#ifndef DEBUG_MESSENGER_H
#define DEBUG_MESSENGER_H

#include <vulkan/vulkan.h>

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT *data,
	void *user_data
);

class Debug_messenger {
public:
	Debug_messenger(VkInstance instance) : _instance{instance} {
		VkDebugUtilsMessengerCreateInfoEXT create_info {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = debug_callback,
			.pUserData = nullptr
		};
		create_func = (PFN_vkCreateDebugUtilsMessengerEXT) (vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
		destroy_func = (PFN_vkDestroyDebugUtilsMessengerEXT) (vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
		if (!create_func || !destroy_func)
			return;
		VkResult res = create_func(_instance, &create_info, nullptr, &_messenger);
		_initialised = res == VK_SUCCESS;
	}

	~Debug_messenger() {
		if (!_initialised)
			return;
		destroy_func(_instance, _messenger, nullptr);
	}

private:
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _messenger;
	bool _initialised{};
	PFN_vkCreateDebugUtilsMessengerEXT create_func{};
	PFN_vkDestroyDebugUtilsMessengerEXT destroy_func{};
};

#endif
