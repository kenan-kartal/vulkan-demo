#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include <vulkan/vulkan.h>

#include <vector>

class Shader_module {
public:
	Shader_module(VkDevice device, const std::vector<char> &code) :
			_device{device}
		{
		VkShaderModuleCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = code.size(),
			.pCode = reinterpret_cast<const uint32_t*>(code.data())
		};
		auto res = vkCreateShaderModule(_device, &create_info, nullptr, &_module);
		_initialised = res == VK_SUCCESS;
	}

	~Shader_module() {
		if (!_initialised)
			return;
		vkDestroyShaderModule(_device, _module, nullptr);
	}

	VkShaderModule get() const { return _module; }

private:
	VkDevice _device;
	VkShaderModule _module{};
	bool _initialised{};
};

#endif
