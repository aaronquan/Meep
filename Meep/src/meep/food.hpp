#pragma once

#include "entity.hpp"
#include "./../shapes/shapes.hpp"

class Food : public Entity2D {
public:
	Food();
	Food(float energy);
	unsigned int getId() const;
	float getEnergy() const;
	float getSize() const;
	float getScale() const;
	float eaten(float amount); // returns energy

	bool isFinished() const;

	CollisionCircle getCollision() const;
private:
	unsigned int m_id;
	float m_energy;
	static unsigned int next_food_id;
};