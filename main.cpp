#include "window.h"
#include "instance.h"
#include "logic_dev.h"
#include "swap_chain.h"
#include "pipeline.h"
#include "frame_buffers.h"
#include "command_pool.h"
#include "semaphore.h"
#include "fence.h"

#include "config.h"

#include <memory>
#include <stdexcept>

struct Drawer {
	VkQueue queue_graphics;
	VkQueue queue_present;
	VkDevice device;
	VkExtent2D extent;
	VkSwapchainKHR swap_chain;
	VkRenderPass render_pass;
	VkPipeline pipeline;
	VkFramebuffer *frame_buffers;
	VkCommandBuffer command_buffer;
	VkSemaphore sem_image_avail;
	VkSemaphore sem_render_finished;
	VkFence fence;

	void draw();
};

void Drawer::draw() {
	vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &fence);

	uint32_t image_index;
	vkAcquireNextImageKHR(device, swap_chain, UINT64_MAX, sem_image_avail, VK_NULL_HANDLE, &image_index);
	VkFramebuffer frame_buffer = frame_buffers[image_index];
	vkResetCommandBuffer(command_buffer, 0);
	record_command_buffer(command_buffer, render_pass, frame_buffer, extent, pipeline);

	VkSemaphore wait_semaphores[] = {sem_image_avail};
	VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkSemaphore signal_semaphores[] = {sem_render_finished};
	VkSubmitInfo submit_info {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = wait_semaphores,
		.pWaitDstStageMask = wait_stages,
		.commandBufferCount = 1,
		.pCommandBuffers = &command_buffer,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = signal_semaphores
	};
	auto res = vkQueueSubmit(queue_graphics, 1, &submit_info, fence);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit the graphics queue.");
	}

	VkSwapchainKHR swap_chains[] = {swap_chain};
	VkPresentInfoKHR present_info {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = signal_semaphores,
		.swapchainCount = 1,
		.pSwapchains = swap_chains,
		.pImageIndices = &image_index,
		.pResults = nullptr
	};
	vkQueuePresentKHR(queue_present, &present_info);
}

int main() {
	Window window{};
	Instance instance{};
	std::unique_ptr<Debug_messenger> messenger;
	if (enable_validation_layers)
		messenger = std::make_unique<Debug_messenger>(instance.get());
	Logical_device device{instance.get(), window.get()};
	Swap_chain swap_chain{device, window.get()};
	Pipeline pipeline{device.get(), swap_chain};
	Frame_buffers frame_buffers{device.get(), pipeline.render_pass().get(), swap_chain};
	Command_pool command_pool{device};
	Semaphore sem_image_avail{device.get()};
	Semaphore sem_render_finished{device.get()};
	Fence fence{device.get()};

	Drawer drawer {
		device.queue_graphics(),
		device.queue_present(),
		device.get(),
		swap_chain.extent(),
		swap_chain.get(),
		pipeline.render_pass().get(),
		pipeline.get(),
		const_cast<VkFramebuffer*>(frame_buffers.get()),
		command_pool.buffer(),
		sem_image_avail.get(),
		sem_render_finished.get(),
		fence.get()
	};

	while(!window.should_close()){
		window.poll();
		try {
			drawer.draw();
		} catch (...) {
			break;
		}
		vkDeviceWaitIdle(device.get());
	}

	return 0;
}
