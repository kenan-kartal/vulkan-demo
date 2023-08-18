#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "swap_chain.h"

#include <vulkan/vulkan.h>

class Render_pass {
public:
	Render_pass(VkDevice device, const Swap_chain &swap_chain) :
			_device{device}
		{
		VkAttachmentDescription color_attachment {
			.format = swap_chain.format(),
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		};
		VkAttachmentReference color_attachment_ref {
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		};
		VkSubpassDescription subpass{
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = 1,
			.pColorAttachments = &color_attachment_ref
		};
		VkSubpassDependency dependency {
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
		};
		VkRenderPassCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = 1,
			.pAttachments = &color_attachment,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &dependency
		};

		auto res = vkCreateRenderPass(_device, &create_info, nullptr, &_render_pass);
		_initialised = res == VK_SUCCESS;
	}

	~Render_pass() {
		if (!_initialised)
			return;
		vkDestroyRenderPass(_device, _render_pass, nullptr);
	}

	VkRenderPass get() const { return _render_pass; }

private:
	VkDevice _device;
	VkRenderPass _render_pass {};
	bool _initialised {};
};

#endif
