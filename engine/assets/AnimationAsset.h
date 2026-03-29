#pragma once

#include <vector>
#include "TextureAsset.h"

struct AnimationAsset {
	std::string name;
	std::vector<TextureAsset> frames;

	float frameDuration;
	bool loop;

	int height, width, depth;

	int currentFrameIndex = 0;
};
