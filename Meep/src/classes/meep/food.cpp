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
	return m_energy*2.0f;
}

CollisionCircle Food::getCollision() const {
	return CollisionCircle(m_x, m_y, 1);
}