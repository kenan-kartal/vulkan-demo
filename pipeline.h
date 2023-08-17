#ifndef PIPELINE_H
#define PIPELINE_H

#include "pipeline_layout.h"
#include "render_pass.h"

#include <vulkan/vulkan.h>

class Swap_chain;

class Pipeline {
public:
	Pipeline(VkDevice device, const Swap_chain &swap_chain);
	~Pipeline();

private:
	VkPipeline _graphics_pipeline{};
	Pipeline_layout _layout;
	Render_pass _render_pass;
	VkDevice _device;
	bool _initialised{};
};

#endif
