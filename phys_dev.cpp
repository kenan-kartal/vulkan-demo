#include "phys_dev.h"

#include <vector>
#include <iostream>

static std::pair<bool, Queue_family_indices> is_device_suitable(const VkPhysicalDevice dev, const VkSurfaceKHR surface) {
	Queue_family_indices indices;
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(dev, &prop);
	if (prop.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		return {false, indices};
	VkPhysicalDeviceFeatures feat;
	vkGetPhysicalDeviceFeatures(dev, &feat);
	if (!feat.geometryShader)
		return {false, indices};
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, queue_families.data());
	bool found_graphics {};
	bool found_present {};
	for (uint32_t i = 0; i < queue_families.size(); ++i) {
		const auto &queue_family = queue_families[i];
		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphics = i;
			found_graphics = true;
		}
		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &present_support);
		if (present_support) {
			indices.present = i;
			found_present = true;
		}
		if (found_graphics && found_present)
			return {true, indices};
	}
	return {false, indices};
}

std::pair<VkPhysicalDevice, Queue_family_indices> pick_physical_device(const VkInstance instance, const VkSurfaceKHR surface) {
	Queue_family_indices indices;
	uint32_t device_count{};
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
	if (device_count == 0)
		return {nullptr, indices};
	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());
	for (const auto device : devices) {
		auto res = is_device_suitable(device, surface);
		if (res.first) {
			indices = res.second;
			VkPhysicalDeviceProperties prop;
			vkGetPhysicalDeviceProperties(device, &prop);
			std::cout << "Picked device: " << prop.deviceName << '\n';
			return {device, indices};
		}
	}
	return {nullptr, indices};
}
