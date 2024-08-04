#pragma once
#include <iostream>
#include <optional>
#include <map>

#include "./../entity.hpp"
#include "./../shapes/shapes.hpp"
#include "food.hpp"
#include "./../math/distances.hpp"

enum MeepState {
	Stable, LookingForFood, MovingToFood, Dead
};

class Meep : public Entity2D {
public:
	Meep();
	Meep(float sp);
	void step(float dt);
	void setStableState();

	void updateState();

	MeepState getState() const;
	float getMaxEnergy() const;
	float getEnergy() const;
	float getEnergyRatio() const;
	float getSize() const;

	unsigned int getId() const;

	float getDistanceSquaredTo(float x, float y);

	void moveToClosestFood(std::map<unsigned int, Food>& foods);

	CollisionCircle getCollision() const;
	bool collideFood(Food const &food) const;
	void eatFood(Food const &food);
	std::vector<unsigned int> collideFood(std::map<unsigned int, Food> &foods);

private:
	unsigned int m_id;

	MeepState m_state;

	float m_speed;
	float m_vx;
	float m_vy;

	float m_size;

	float m_energy;
	float m_max_energy;
	unsigned int eats;

	float m_energy_recovery_modifier; //use this todo

	std::optional<Food> closestFood(std::map<unsigned int, Food>& foods);

	
	static unsigned int next_id;
	static unsigned int sense_radius;
	static float passive_energy_drain;
	static float movement_energy_drain;

};