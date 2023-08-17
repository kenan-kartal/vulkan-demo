#include "swap_chain.h"

void Swap_chain::make_images() {
	uint32_t image_count;
	vkGetSwapchainImagesKHR(_device, _swap_chain, &image_count, nullptr);
	_images.resize(image_count);
	vkGetSwapchainImagesKHR(_device, _swap_chain, &image_count, _images.data());
}

void Swap_chain::make_image_views() {
	_image_views.reserve(_images.size());
	for (const auto image : _images) {
		_image_views.emplace_back(_device, image, _format);
	}
}
