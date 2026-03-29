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
		}
	}
};
