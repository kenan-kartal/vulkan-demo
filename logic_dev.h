#ifndef LOGIC_DEV_H
#define LOGIC_DEV_H

#include "phys_dev.h"

#include "config.h"

#include <vulkan/vulkan.h>

class Logical_device {
public:
	Logical_device(VkInstance instance) {
		VkPhysicalDeviceFeatures features {};
		VkQueueFlags queue_flags = VK_QUEUE_GRAPHICS_BIT;
		auto phy_ind_pair = pick_physical_device(instance, features, queue_flags);
		if (!phy_ind_pair.first)
			return;
		float priority = 1.0f;
		VkDeviceQueueCreateInfo queue_create_info {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = phy_ind_pair.second,
			.queueCount = 1,
			.pQueuePriorities = &priority
		};
		VkDeviceCreateInfo device_create_info {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queue_create_info,
			.pEnabledFeatures = &features
		};
		auto res = vkCreateDevice(phy_ind_pair.first, &device_create_info, nullptr, &_device);
		_initialised = res == VK_SUCCESS;
		if (!_initialised)
			return;
		vkGetDeviceQueue(_device, phy_ind_pair.second, 0, &_queue);
	}

	~Logical_device() {
		if (!_initialised)
			return;
		vkDestroyDevice(_device, nullptr);
	}

	VkQueue queue() const { return _queue; }

private:
	VkDevice _device{};
	VkQueue _queue{};
	bool _initialised{};
};

#endif
