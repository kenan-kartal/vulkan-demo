#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

#include "phys_dev.h"
#include "logic_dev.h"
#include "window.h"

#include <vector>

class Swap_chain {
public:
	Swap_chain(const Logical_device& logical_device, GLFWwindow *window) :
			_device{logical_device.get()}
		{
		const VkPhysicalDevice physical_device = logical_device.physical_device();
		VkSurfaceKHR surface = logical_device.surface().get();

		auto support = get_swap_chain_support_details(physical_device, surface);
		VkSurfaceFormatKHR surface_format = choose_swap_surface_format(support.formats);
		_format = surface_format.format;
		VkPresentModeKHR present_mode = choose_swap_surface_present_mode(support.present_modes);
		_extent = choose_swap_extent(support.capabilities, window);
		uint32_t image_count = support.capabilities.minImageCount;
		if (image_count == 1 && (support.capabilities.maxImageCount == 0 || support.capabilities.maxImageCount >= 2))
			image_count = 2;

		VkSwapchainCreateInfoKHR create_info {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = surface,
			.minImageCount = image_count,
			.imageFormat = _format,
			.imageColorSpace = surface_format.colorSpace,
			.imageExtent = _extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.preTransform = support.capabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = present_mode,
			.clipped = VK_TRUE,
			.oldSwapchain = VK_NULL_HANDLE
		};
		auto found_indices_pair = get_queue_family_indices(physical_device, surface);
		if (!found_indices_pair.first)
			return;
		Queue_family_indices indices {found_indices_pair.second};
		std::vector<uint32_t> indices_vec { indices.graphics, indices.present };
		if (indices.graphics != indices.present) {
			create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			create_info.queueFamilyIndexCount = 2;
			create_info.pQueueFamilyIndices = indices_vec.data();
		}
		auto res = vkCreateSwapchainKHR(_device, &create_info, nullptr, &_swap_chain);
		_initialised = res == VK_SUCCESS;
		if (!_initialised)
			return;
		vkGetSwapchainImagesKHR(_device, _swap_chain, &image_count, nullptr);
		_images.resize(image_count);
		vkGetSwapchainImagesKHR(_device, _swap_chain, &image_count, _images.data());
	}

	~Swap_chain() {
		if (!_initialised)
			return;
		vkDestroySwapchainKHR(_device, _swap_chain, nullptr);
	}

	VkSwapchainKHR get() const { return _swap_chain; }

private:
	std::vector<VkImage> _images;
	VkSwapchainKHR _swap_chain{};
	VkDevice _device{};
	VkFormat _format;
	VkExtent2D _extent;
	bool _initialised{};
};

#endif
