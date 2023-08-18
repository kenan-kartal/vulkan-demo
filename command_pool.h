#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include "logic_dev.h"
#include "render_pass.h"
#include "swap_chain.h"
#include "frame_buffers.h"
#include "pipeline.h"

#include <vulkan/vulkan.h>

#include <stdexcept>

class Command_pool {
public:
	Command_pool(const Logical_device& device) : _device{device.get()} {
		auto indices_res = get_queue_family_indices(device.physical_device(), device.surface().get());
		if (!indices_res.first)
			return;
		Queue_family_indices indices { indices_res.second };
		VkCommandPoolCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = indices.graphics
		};
		auto res = vkCreateCommandPool(_device, &create_info, nullptr, &_pool);
		if (res != VK_SUCCESS)
			return;

		VkCommandBufferAllocateInfo alloc_info{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = _pool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};
		res = vkAllocateCommandBuffers(_device, &alloc_info, &_buffer);
		_initialised = res == VK_SUCCESS;
	}

	~Command_pool() {
		if (!_initialised)
			return;
		vkDestroyCommandPool(_device, _pool, nullptr);
	}

private:
	VkDevice _device;
	VkCommandPool _pool;
	VkCommandBuffer _buffer;
	bool _initialised{};
};

inline void record(
		VkCommandBuffer buffer,
		uint32_t image_index,
		const Render_pass &render_pass,
		const Frame_buffers &frame_buffers,
		const Swap_chain &swap_chain,
		const Pipeline &pipeline
		) {
	VkCommandBufferBeginInfo begin_info {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0,
		.pInheritanceInfo = nullptr
	};
	auto res = vkBeginCommandBuffer(buffer, &begin_info);
	if (res != VK_SUCCESS)
		throw std::runtime_error("Failed to begin recording command buffer.");

	VkClearValue clear_value {
		.color = {0.f, 0.f, 0.f, 1.f}
	};
	VkRenderPassBeginInfo render_pass_info {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = render_pass.get(),
		.framebuffer = frame_buffers.get()[image_index],
		.renderArea = VkRect2D {
			.offset = {0, 0},
			.extent = swap_chain.extent()
		},
		.clearValueCount = 1,
		.pClearValues = &clear_value
	};
	vkCmdBeginRenderPass(buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());

	VkViewport viewport {
		.x = 0.f,
		.y = 0.f,
		.width = static_cast<float>(swap_chain.extent().width),
		.height = static_cast<float>(swap_chain.extent().height),
		.minDepth = 0.f,
		.maxDepth = 1.f
	};
	vkCmdSetViewport(buffer, 0, 1, &viewport);

	VkRect2D scissor {
		.offset = {0, 0},
		.extent = swap_chain.extent()
	};
	vkCmdSetScissor(buffer, 0, 1, &scissor);

	// Hardcode vertex info for now.
	vkCmdDraw(buffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(buffer);

	res = vkEndCommandBuffer(buffer);
	if (res != VK_SUCCESS)
		throw std::runtime_error("Failed to end recording comand buffer.");
}

#endif
