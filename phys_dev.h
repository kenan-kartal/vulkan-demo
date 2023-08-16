#ifndef PHYS_DEV_H
#define PHYS_DEV_H

#include <vulkan/vulkan.h>
#include <utility>

/*
 * Takes a vulkan instance, device features reference and required queue family flags.
 * Returns a pair of physical device pointer and queue family index.
 */
std::pair<VkPhysicalDevice, uint32_t> pick_physical_device(
	const VkInstance instance,
	const VkPhysicalDeviceFeatures &requested_features,
	const VkQueueFlags queue_flags
);

#endif
