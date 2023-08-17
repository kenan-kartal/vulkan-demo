#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <vector>

inline std::vector<char> read_file(const std::string &filename) {
	std::ifstream file{filename, std::ios::ate | std::ios::binary};
	if (!file.is_open())
		throw std::runtime_error("Failed to open file.");
	size_t size{ static_cast<size_t>(file.tellg()) };
	std::vector<char> buffer(size);
	file.seekg(0);
	file.read(buffer.data(), size);
	file.close();
	return buffer;
}

#endif
