#include "phys_dev.h"

#include "window.h"

#include <iostream>
#include <cstring>
#include <limits>
#include <algorithm>

const std::vector<const char*> device_extensions {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &formats) {
	for (const auto& format : formats) {
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return format;
		}
	}
	return formats[0];
}

VkPresentModeKHR choose_swap_surface_present_mode(const std::vector<VkPresentModeKHR> &modes) {
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities, const GLFWwindow *window) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;
	int width, height;
	glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &width, &height);
	VkExtent2D extent {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	};
	extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	return extent;
}

Swap_chain_support_details get_swap_chain_support_details(const VkPhysicalDevice dev, const VkSurfaceKHR surface) {
	Swap_chain_support_details dets;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &dets.capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, nullptr);
	dets.formats.resize(format_count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, dets.formats.data());

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &present_mode_count, nullptr);
	dets.present_modes.resize(present_mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &present_mode_count, dets.present_modes.data());

	return dets;
}

std::pair<bool, Queue_family_indices> get_queue_family_indices(const VkPhysicalDevice device, const VkSurfaceKHR surface) {
	Queue_family_indices indices;
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());
	bool found_graphics {};
	bool found_present {};
	for (uint32_t i = 0; i < queue_families.size(); ++i) {
		const auto &queue_family = queue_families[i];
		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphics = i;
			found_graphics = true;
		}
		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
		if (present_support) {
			indices.present = i;
			found_present = true;
		}
		if (found_graphics && found_present)
			return {true, indices};
	}
	return {false, indices};
}

static bool has_extension (const std::vector<VkExtensionProperties> &ext_props, const char *ext_name) {
	for (const auto ext_prop : ext_props) {
		if (strcmp(ext_prop.extensionName, ext_name) == 0)
			return true;
	}
	return false;
}

static bool supports_extensions (const VkPhysicalDevice dev) {
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, available_extensions.data());
	for(const auto ext : device_extensions) {
		if (!has_extension(available_extensions, ext))
			return false;
	}
	return true;
}

static std::pair<bool, Queue_family_indices> is_device_suitable(const VkPhysicalDevice device, const VkSurfaceKHR surface) {
	Queue_family_indices indices;

	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(device, &prop);
	if (prop.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		return {false, indices};

	VkPhysicalDeviceFeatures feat;
	vkGetPhysicalDeviceFeatures(device, &feat);
	if (!feat.geometryShader)
		return {false, indices};

	if (!supports_extensions(device))
		return {false, indices};

	auto swap_chain_support = get_swap_chain_support_details(device, surface);
	if (swap_chain_support.formats.empty() || swap_chain_support.present_modes.empty())
		return {false, indices};

	auto queue_family_indices_result = get_queue_family_indices(device, surface);
	bool found_indices {queue_family_indices_result.first};
	if (found_indices) {
		indices = queue_family_indices_result.second;
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
		VkPhysicalDeviceProperties prop;
		vkGetPhysicalDeviceProperties(device, &prop);
		if (res.first) {
			indices = res.second;
			std::cout << "Picked device: " << prop.deviceName << '\n';
			return {device, indices};
		} else {
			std::cout << "Device not suitable: " << prop.deviceName << '\n';
		}
	}
	return {nullptr, indices};
}
