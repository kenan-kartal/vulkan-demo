#include "phys_dev.h"

#include <vector>
#include <iostream>

static bool supports_requested_features(const VkPhysicalDeviceFeatures &features, const VkPhysicalDeviceFeatures &requested_features) {
	if (!requested_features.geometryShader || features.geometryShader && requested_features.geometryShader)
		return true;
	return false;
}

static std::pair<bool, uint32_t> is_device_suitable(
	const VkPhysicalDevice dev,
	const VkPhysicalDeviceFeatures &requested_features,
	const VkQueueFlags queue_flags
	) {
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(dev, &prop);
	if (prop.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		return {false, 0};
	VkPhysicalDeviceFeatures feat;
	vkGetPhysicalDeviceFeatures(dev, &feat);
	if (!supports_requested_features(feat, requested_features))
		return {false, 0};
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, queue_families.data());
	for (uint32_t i = 0; i < queue_families.size(); ++i) {
		const auto &queue_family = queue_families[i];
		if ((queue_family.queueFlags & queue_flags) == queue_flags)
			return {true, i};
	}
	return {false, 0};
}

std::pair<VkPhysicalDevice, uint32_t> pick_physical_device(
	const VkInstance instance,
	const VkPhysicalDeviceFeatures &requested_features,
	const VkQueueFlags queue_flags
	) {
	uint32_t device_count{};
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
	if (device_count == 0)
		return {nullptr, 0};
	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());
	for (const auto device : devices) {
		auto res = is_device_suitable(device, requested_features, queue_flags);
		if (res.first) {
			VkPhysicalDeviceProperties prop;
			vkGetPhysicalDeviceProperties(device, &prop);
			std::cout << "Picked device: " << prop.deviceName << '\n';
			return {device, res.second};
		}
	}
	return {nullptr, 0};
}
