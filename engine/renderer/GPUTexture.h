#pragma once

#include <cstddef>

class GPUTexture {
  public:
	virtual void init(std::size_t entityID) = 0;
	virtual void bind(std::size_t textureID) = 0;
	virtual void use(std::size_t entityID, const char *name) = 0;
	virtual void addTexture(std::size_t entityID, const char *name, const char *filePath) = 0;
	virtual void addTexture3D(std::size_t entityID, const char *name, const char *filePath,
	                          int maxLayers = 16) = 0;
	virtual int getTextureLocation(std::size_t entityID, const char *name) = 0;
	virtual const char *getTextureName(std::size_t entityID, int textureLocation) = 0;
	virtual void printTextureNames(std::size_t entityID) = 0;
	virtual ~GPUTexture() = default;
};