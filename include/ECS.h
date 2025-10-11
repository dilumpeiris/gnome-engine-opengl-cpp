#pragma once
#include <algorithm>
#include <array>
#include <bitset>
#include <memory.h>
#include <vector>
class Manager;
class Entity;
class System;
class Component;

using componentID = std::size_t;
using systemID = std::size_t;

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxSystems = 32;

using componentBitSet = std::bitset<maxComponents>;
using systemBitSet = std::bitset<maxSystems>;

using componentArray = std::array<Component *, maxComponents>;
using systemArray = std::array<System *, maxSystems>;

inline componentID getComponentID() noexcept {
	static componentID lastID = 0;
	return lastID++;
}

inline systemID getSystemID() noexcept {
	static systemID lastID = 0;
	return lastID++;
}

template <typename T> inline componentID getComponentID() noexcept {
	static componentID componentID = getComponentID();
	return componentID;
}

template <typename T> inline systemID getSystemID() noexcept {
	static systemID systemID = getSystemID();
	return systemID;
}

class Component {
  public:
	Entity *entity;

	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	virtual ~Component() {}
};

class System {
  public:
	Manager *manager;

	virtual void init() {};
	virtual void update() {};
};

class Entity {
  public:
	Manager *manager;

	int tag;

	std::vector<std::unique_ptr<Component>> components;
	componentArray componentArray;
	componentBitSet componentBitSet;

	Entity(int entTag) { tag = entTag; }

	void init() {
		for (auto &c : components) {
			c->init();
		}
	};
	virtual void update() {
		for (auto &c : components) {
			c->update();
		}
	};
	virtual void draw() {
		for (auto &c : components) {
			c->draw();
		}
	};

	template <typename T, typename... TArgs> T &addComponent(TArgs &&...mArgs) {
		T *c = new T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		std::unique_ptr<T> uPtr{c};
		components.emplace_back(std::move(uPtr));
		componentArray[getComponentID<T>()] = c;
		componentBitSet[getComponentID<T>()] = true;
		return *c;
	}

	template <typename T> T *getComponent() {
		return static_cast<T *>(componentArray[getComponentID<T>()]);
	}
	template <typename T> bool hasComponent() { return componentBitSet[getComponentID<T>()]; }
};

class Manager {
  public:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<System>> systems;

	systemArray systemArray;
	systemBitSet systemBitSet;

	void init() {
		for (auto &e : entities) {
			e->init();
		}
	}
	void update() {
		for (auto &e : entities) {
			e->update();
		}
		// for (auto &s : systems) {
		//     s->update();
		// }
	}
	void draw() {
		for (auto &e : entities) {
			e->draw();
		}
	}
	Entity *addEntity(Entity *e) {
		std::unique_ptr<Entity> uPtr{e};
		entities.emplace_back(std::move(uPtr));
		return e;
	}
	Entity *addEntity(int entTag) {
		Entity *e = new Entity(entTag);
		std::unique_ptr<Entity> uPtr{e};
		entities.emplace_back(std::move(uPtr));
		return e;
	}
	template <typename T, typename... TArgs> T &addSystem(TArgs &&...mArgs) {
		T *s = new T(std::forward<TArgs>(mArgs)...);
		s->manager = this;
		std::unique_ptr<T> uPtr{s};
		systems.emplace_back(std::move(uPtr));
		systemArray[getSystemID<T>()] = s;
		systemBitSet[getSystemID<T>()] = true;
		return *s;
	}
};
