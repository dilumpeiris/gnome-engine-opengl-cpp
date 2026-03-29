#pragma once

#include <cstddef>
#include "assets/MaterialAsset.h"
#include "assets/AnimationAsset.h"
#include "assets/MeshAsset.h"

class GPUMaterial {
  public:
	virtual void init(std::size_t entityID, MaterialAsset material, MeshAsset mesh) = 0;
	virtual void update(std::size_t entityID) = 0;
	virtual void draw(std::size_t entityID) = 0;
	virtual void addShader(std::size_t entityID, ShaderPass shader) = 0;
	virtual void addAnimation(std::size_t entityID, std::string name, AnimationAsset animation) = 0;
};
