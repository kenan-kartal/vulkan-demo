#ifndef PIPELINE_LAYOUT_H
#define PIPELINE_LAYOUT_H

#include <vulkan/vulkan.h>

class Pipeline_layout {
public:
	Pipeline_layout(VkDevice device) : _device{device} {
		VkPipelineLayoutCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};
		auto res = vkCreatePipelineLayout(_device, &create_info, nullptr, &_layout);
		_initialised = res == VK_SUCCESS;
	}

	~Pipeline_layout() {
		if (!_initialised)
			return;
		vkDestroyPipelineLayout(_device, _layout, nullptr);
	}

	VkPipelineLayout get() const { return _layout; }

private:
	VkDevice _device;
	VkPipelineLayout _layout{};
	bool _initialised{};
};

#endif
