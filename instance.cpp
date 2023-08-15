#include "instance.h"

#include <iostream>
#include <vector>
#include <cstring>

#ifdef NDEBUG
static const bool enable_validation_layers = false;
#else
static const bool enable_validation_layers = true;
#endif

static const char *validation_layer_name = "VK_LAYER_KHRONOS_validation";

std::vector<const char*> required_layers {};
std::vector<const char*> required_extensions {};

template<typename T, typename F>
static void print_list(const std::vector<T> items, const char *header, F func) {
	std::cout << header << '\n';
	for (const auto& item: items) {
		std::cout << '\t' << func(item) << '\n';
	}
}

static bool has_required_layer(const char* name, const std::vector<VkLayerProperties> &layers) {
	for (const auto &property : layers) {
		if (strcmp(name, property.layerName) == 0)
			return true;
	}
	return false;
}

static bool has_required_extension(const char* name, const std::vector<VkExtensionProperties> &extensions) {
	for (const auto &property : extensions) {
		if (strcmp(name, property.extensionName) == 0)
			return true;
	}
	return false;
}

bool has_required_layers() {
	uint32_t layer_count{};
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
	std::vector<VkLayerProperties> layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, layers.data());
	auto get_layer_name = [](const VkLayerProperties& p) {return p.layerName;};
	print_list(layers, "Available layers:", get_layer_name);
	if (enable_validation_layers) {
		required_layers.push_back(validation_layer_name);
	}
	auto get_req_layer_name = [](const char* ln) {return ln;};
	print_list(required_layers, "Required layers:", get_req_layer_name);
	for (const char *layer : required_layers)
		if (!has_required_layer(layer, layers))
			return false;
	return true;
}

bool has_required_extensions() {
	uint32_t extension_count{};
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
	auto get_extension_name = [](const VkExtensionProperties& e) {return e.extensionName;};
	print_list(extensions, "Available extensions", get_extension_name);
	uint32_t glfw_extension_count{};
	const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
	for (int i = 0; i < glfw_extension_count; ++i) {
		required_extensions.push_back(glfw_extensions[i]);
	}
	auto get_req_extension_name = [](const char* en) {return en;};
	print_list(required_extensions, "Required extensions:", get_req_extension_name);
	for (const char *extension : required_extensions)
		if (!has_required_extension(extension, extensions))
			return false;
	return true;
}
