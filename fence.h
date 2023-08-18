#ifndef FENCE_H
#define FENCE_H

#include <vulkan/vulkan.h>

class Fence {
public:
	Fence(VkDevice device) : _device{device} {
		VkFenceCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT
		};

		auto res = vkCreateFence(_device, &create_info, nullptr, &_fence);
		_initialised = res == VK_SUCCESS;
	}

	~Fence() {
		if (!_initialised)
			return;
		vkDestroyFence(_device, _fence, nullptr);
	}

	VkFence get() const { return _fence; }

private:
	VkFence _fence;
	VkDevice _device;
	bool _initialised{};
};

#endif