#include "debug_messenger.h"

#include <iostream>

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT *data,
	void *user_data
) {
	std::cerr << "Validation layer: " << data->pMessage << '\n';
	return VK_FALSE;
}
