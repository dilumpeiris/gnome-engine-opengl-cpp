#pragma once

#include <cstddef>

class GPUTexture {
  public:
	virtual void init(std::size_t textureID) = 0;
	virtual void use(std::size_t textureID) = 0;
	virtual void addTexture(std::size_t textureID, const char *filePath) = 0;
	virtual ~GPUTexture() = default;
};