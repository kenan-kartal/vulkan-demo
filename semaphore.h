#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <vulkan/vulkan.h>

class Semaphore {
public:
	Semaphore(VkDevice device) : _device{device} {
		VkSemaphoreCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};

		auto res = vkCreateSemaphore(_device, &create_info, nullptr, &_semaphore);
		_initialised = res == VK_SUCCESS;
	}

	~Semaphore() {
		if (!_initialised)
			return;
		vkDestroySemaphore(_device, _semaphore, nullptr);
	}

	VkSemaphore get() const { return _semaphore; }

private:
	VkSemaphore _semaphore;
	VkDevice _device;
	bool _initialised{};
};

#endif