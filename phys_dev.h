#ifndef PHYS_DEV_H
#define PHYS_DEV_H

#include <vulkan/vulkan.h>
#include <utility>
#include <vector>

struct GLFWwindow;

struct Queue_family_indices {
	uint32_t graphics;
	uint32_t present;
};

struct Swap_chain_support_details {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> present_modes;
};

extern const std::vector<const char*> device_extensions;

std::pair<bool, Queue_family_indices> get_queue_family_indices(const VkPhysicalDevice device, const VkSurfaceKHR surface);

std::pair<VkPhysicalDevice, Queue_family_indices> pick_physical_device(const VkInstance instance, const VkSurfaceKHR surface);

VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &formats);

VkPresentModeKHR choose_swap_surface_present_mode(const std::vector<VkPresentModeKHR> &modes);

VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities, const GLFWwindow *window);

Swap_chain_support_details get_swap_chain_support_details(const VkPhysicalDevice device, const VkSurfaceKHR surface);

#endif
