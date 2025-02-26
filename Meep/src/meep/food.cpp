#include "food.hpp"

unsigned int Food::next_food_id = 0;

Food::Food() : Food(1) {};
Food::Food(float energy) : m_id(next_food_id), m_energy(energy) {
	next_food_id++;
};
unsigned int Food::getId() const {
	return m_id;
}
float Food::getEnergy() const {
	return m_energy;
}
float Food::getSize() const {
	return m_energy*0.5f;
}

float Food::getScale() const {
	return getSize()*2.0f;
}

float Food::eaten(float amount) {
	m_energy -= amount;
	if (m_energy < 0) {
		float eaten_amount = amount + m_energy;
		m_energy = 0;
		return eaten_amount;
	}
	return amount;
}

bool Food::isFinished() const {
	return m_energy <= 0;
}

CollisionCircle Food::getCollision() const {
	return CollisionCircle(getX(), getY(), getSize());
}