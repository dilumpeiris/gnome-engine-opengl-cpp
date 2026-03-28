#pragma once
#include "Mesh.h"
#include "Physics.h"
#include "Transform.h"
#include "ecs/ECS.h"

class CollisionSystem : public System {
  public:
	void init() override {}

	void update() override {
		for (auto &entity : this->manager->entities) {
			Mesh *mesh = entity->getComponent<Mesh>();
			Transform *transform = entity->getComponent<Transform>();
			Physics *physics = entity->getComponent<Physics>();

			if (!mesh || !transform || !physics)
				continue;
		}
	}
};