# Vulkan Demo

![A triangle with interpolated colours](/images/first_triangle.png)

Contains code of my first exposure to Vulkan API.

## Build

This code is for my own use, I have not intended it to be cross-platform. That is why I have kept the build simple with GNU Make.

Library dependencies are Vulkan SDK & GLFW. Also I use `glslc` to compile shaders into `SPIR-V` format. On Ubuntu 22.04 this file comes from `shaderc` package which is a dependency of `vulkan-sdk`.

Please modify `Makefile` according to your system.

## References

- [Vulkan Tutorial](https://vulkan-tutorial.com)
- [Vulkan Specification](https://registry.khronos.org/vulkan/specs/1.3-extensions/html/)

