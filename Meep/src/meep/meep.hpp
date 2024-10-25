#pragma once
#include <iostream>
#include <optional>
#include <map>
#include <unordered_set>

#include "entity.hpp"
#include "food.hpp"

#include "./../math/map.hpp"
#include "./../shapes/shapes.hpp"
#include "./../math/distances.hpp"

enum class MeepStage {
	Cloning, Child, Adult
};

enum class MeepState {
	Stable, Rotating, Eating, LookingForFood, MovingToFood, Dead, Cloning
};

class Meep;

struct MeepStateChangeData {
	Meep* clone_meep;
	MeepStateChangeData(): clone_meep(nullptr){};
};

class Meep : public Entity2D {
public:
	Meep();
	Meep(float speed);
	Meep(float size, float speed, MeepStage stage);
	void step(float dt);

	void growUp();
	bool isCloningStage();

	void setStableState();

	MeepStateChangeData updateState();

	MeepState getState() const;
	float getMaxEnergy() const;
	float getEnergy() const;
	float getEnergyRatio() const;
	float getSize() const;
	float getScale() const;
	glm::vec2 getDirection() const;

	float getRotation() const;

	unsigned int getId() const;

	float getDistanceSquaredTo(float x, float y);

	void moveToClosestFood(std::map<unsigned int, Food>& foods);

	CollisionCircle getCollision() const;
	bool collideFood(Food const &food) const;
	bool collidePoint(float x, float y) const;
	bool eatFood(Food &food, float dt);
	std::vector<unsigned int> collideFood(std::map<unsigned int, Food> &foods, float dt);

private:
	unsigned int m_id;

	MeepStage m_stage;
	MeepState m_state;
	CollisionCircle m_collision;

	MapPosition* m_target_food;

	float m_speed;
	float m_vx;
	float m_vy;

	float m_size;
	float m_age;

	float m_rotation;

	float m_energy;
	float m_max_energy;

	unsigned int m_n_eats;
	float m_eat_speed;

	float m_energy_recovery_modifier;

	std::vector<Meep*> m_children;

	std::optional<Food> closestFood(std::map<unsigned int, Food>& foods);

	void move(float x, float y);
	void setSize(float size);
	
	static unsigned int next_id;
	static float sense_radius;
	static float passive_energy_drain;
	static float movement_energy_drain;

	static float child_size;
	static float spawn_size;
	static std::unordered_set<unsigned int> reserved_food;
};

class RotationMeep : public Meep {
public:
	RotationMeep();
private:
	float m_rotation_energy_drain;
};