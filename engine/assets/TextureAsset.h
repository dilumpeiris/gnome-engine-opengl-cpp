#pragma once

#include <vector>

// Duplication of data must be avoided.
struct TextureAsset {
	const char *filePath;
	const char *name;
	std::vector<unsigned char> data;
	int width;
	int height;
	int channels;
};