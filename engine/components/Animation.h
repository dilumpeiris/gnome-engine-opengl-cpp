#pragma once
#include "ecs/ECS.h"
#include "assets/AnimationAsset.h"
#include <unordered_map>
// =====================================================================================================
// Animation Component
// =====================================================================================================
class Animation : public Component {

  public:
	std::unordered_map<std::string, AnimationAsset> animationAssets;
	std::string currentAnimation;
	bool isPlaying = false;

  public:
	void init() override {}

	void addAnimation(std::string name, AnimationAsset asset) { animationAssets[name] = asset; }

	std::string getCurrentAnimation() { return currentAnimation; }
};
