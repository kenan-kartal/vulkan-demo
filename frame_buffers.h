#ifndef FRAME_BUFFERS_H
#define FRAME_BUFFERS_H

#include "swap_chain.h"

#include <vulkan/vulkan.h>

#include <vector>

class Frame_buffers {
public:
	Frame_buffers(VkDevice device, VkRenderPass render_pass, const Swap_chain &swap_chain) :
		_device{device}
	{
		auto& image_views = swap_chain.image_views();
		_buffers.resize(image_views.size());
		for (size_t i = 0; i < image_views.size(); ++i) {
			VkImageView image_view = image_views[i].get();
			VkFramebufferCreateInfo create_info {
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass = render_pass,
				.attachmentCount = 1,
				.pAttachments = &image_view,
				.width = swap_chain.extent().width,
				.height = swap_chain.extent().height,
				.layers = 1
			};
			auto res = vkCreateFramebuffer(_device, &create_info, nullptr, &_buffers[i]);
			if (res == VK_SUCCESS)
				++_initialised_count;
			else
				return;
		}
	}

	~Frame_buffers() {
		for (size_t i = 0; i < _initialised_count; ++i) {
			vkDestroyFramebuffer(_device, _buffers[i], nullptr);
		}
	}

	const std::vector<VkFramebuffer>& get() const { return _buffers; }

private:
	std::vector<VkFramebuffer> _buffers;
	VkDevice _device;
	size_t _initialised_count{};
};

#endif
