#ifndef LOGIC_DEV_H
#define LOGIC_DEV_H

#include "phys_dev.h"
#include "surface.h"

#include "config.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Logical_device {
public:
	Logical_device(VkInstance instance, GLFWwindow *window)
		: _surface{instance, window} {
		if (!_surface.get())
			return;
		auto phy_ind_pair = pick_physical_device(instance, _surface.get());
		VkPhysicalDevice phys_device = phy_ind_pair.first;
		Queue_family_indices indices = phy_ind_pair.second;
		if (!phys_device)
			return;
		std::vector<VkDeviceQueueCreateInfo> queue_create_infos{};
		float priority = 1.0f;
		std::vector<uint32_t> unique_indices {};
		unique_indices.push_back(indices.graphics);
		if (indices.present != indices.graphics)
			unique_indices.push_back(indices.present);
		for (uint32_t index : unique_indices) {
			VkDeviceQueueCreateInfo queue_create_info {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = index,
				.queueCount = 1,
				.pQueuePriorities = &priority
			};
			queue_create_infos.push_back(queue_create_info);
		}
		VkPhysicalDeviceFeatures features {};
		VkDeviceCreateInfo device_create_info {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
			.pQueueCreateInfos = queue_create_infos.data(),
			.pEnabledFeatures = &features
		};
		auto res = vkCreateDevice(phys_device, &device_create_info, nullptr, &_device);
		_initialised = res == VK_SUCCESS;
		if (!_initialised)
			return;
		vkGetDeviceQueue(_device, indices.graphics, 0, &_queue_graphics);
		vkGetDeviceQueue(_device, indices.present, 0, &_queue_present);
	}

	~Logical_device() {
		if (!_initialised)
			return;
		vkDestroyDevice(_device, nullptr);
	}

	VkQueue queue_graphics() const { return _queue_graphics; }
	VkQueue queue_present() const { return _queue_present; }

private:
	Surface _surface;
	VkDevice _device{};
	VkQueue _queue_graphics{};
	VkQueue _queue_present{};
	bool _initialised{};
};

#endif
