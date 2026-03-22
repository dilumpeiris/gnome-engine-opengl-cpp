#pragma once
#include "ecs/ECS.h"
#include <unordered_map>

struct AnimationFrame {
	int frameIndex;
	const char *filePath;
};

struct AnimationObject {
	std::string name;
	double frameDuration = 100;
	float timer = 0.0f;
	int currentFrameIndex = 0;
	std::vector<AnimationFrame> frames;
};

class Animation : public Component {

  public:
	std::vector<AnimationObject> animations;
	AnimationObject currentAnimation;

	bool isPlaying = false;

  public:
	void init() override {}

	void addAnimation(std::string name) {
		AnimationObject anim;
		anim.name = name;
		animations.push_back(anim);
	}

	void addFrame(std::string name, const char *filePath, int frameIndex) {
		for (auto &anim : animations) {
			AnimationFrame frame;
			frame.filePath = filePath;
			if (std::string(anim.name) == name) {
				frame.frameIndex = frameIndex;
				anim.frames.push_back(frame);
				break;
			}
		}
	}

	void playAnimation(std::string name) {
		isPlaying = true;
		for (auto &anim : animations) {
			if (anim.name == name) {
				currentAnimation = anim;
				break;
			}
		}
	}

	void update() override {}
};