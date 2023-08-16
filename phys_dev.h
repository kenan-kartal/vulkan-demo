#ifndef PHYS_DEV_H
#define PHYS_DEV_H

#include <vulkan/vulkan.h>
#include <utility>

struct Queue_family_indices {
	uint32_t graphics;
	uint32_t present;
};

std::pair<VkPhysicalDevice, Queue_family_indices> pick_physical_device(const VkInstance instance, const VkSurfaceKHR surface);

#endif
