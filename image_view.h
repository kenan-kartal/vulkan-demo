#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <vulkan/vulkan.h>

class Image_view {
public:
	Image_view (VkDevice device, VkImage image, VkFormat format) :
			_device{device}
		{
		VkImageViewCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.components = VkComponentMapping {
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY
			},
			.subresourceRange = VkImageSubresourceRange {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};
		auto res = vkCreateImageView(_device, &create_info, nullptr, &_view);
		_initialised = res == VK_SUCCESS;
	}

	~Image_view() {
		if (!_initialised)
			return;
		vkDestroyImageView(_device, _view, nullptr);
	}

	VkImageView get() const { return _view; }

private:
	VkDevice _device;
	VkImageView _view{};
	bool _initialised{};
};

#endif
