#pragma once
#include "Animation.h"
#include "ecs/ECS.h"
#include "GnomeTime.h"

using namespace Gnome;

class AnimationSystem : public System {
  public:
	void init() override {}

	void update() override {

		for (auto &entity : this->manager->entities) {
			Animation *animation = entity->getComponent<Animation>();

			if (!animation)
				continue;

			if (animation->isPlaying && !animation->animations.empty() &&
			    !animation->currentAnimation.frames.empty()) {

				// Accumulate delta time
				animation->currentAnimation.timer += GnomeTime::getDeltaTime();

				// Convert frameDuration from milliseconds to seconds
				float frameDurationInSeconds = animation->currentAnimation.frameDuration / 1000.0f;

				// Advance frame if enough time has passed
				while (animation->currentAnimation.timer >= frameDurationInSeconds) {
					animation->currentAnimation.currentFrameIndex =
					    (animation->currentAnimation.currentFrameIndex + 1) %
					    animation->currentAnimation.frames.size();
					animation->currentAnimation.timer -= frameDurationInSeconds;
				}
			}
		}
	}
};