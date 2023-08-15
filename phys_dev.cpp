#include "phys_dev.h"

#include <vector>

static bool is_device_suitable(const VkPhysicalDevice dev) {
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(dev, &prop);
	if (prop.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		return false;
	VkPhysicalDeviceFeatures feat;
	vkGetPhysicalDeviceFeatures(dev, &feat);
	if (!feat.geometryShader)
		return false;
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, queue_families.data());
	for (const auto &queue_family : queue_families) {
		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			return true;
	}
	return false;
}

VkPhysicalDevice pick_physical_device(const VkInstance instance) {
	uint32_t device_count{};
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
	if (device_count == 0)
		return nullptr;
	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());
	for (const auto device : devices) {
		if (is_device_suitable(device))
			return device;
	}
	return nullptr;
}
