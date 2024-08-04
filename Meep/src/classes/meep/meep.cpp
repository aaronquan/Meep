#include "meep.hpp"

unsigned int Meep::next_id = 0;
unsigned int Meep::sense_radius = 200;

float Meep::passive_energy_drain = 0.5f;
float Meep::movement_energy_drain = 0.2f;
Meep::Meep() : Meep(1.0) {};

Meep::Meep(float s) : m_id(next_id),
	m_state(Stable), m_speed(20.0f),
	m_vx(0), m_vy(0),
	m_size(4.0f), eats(0),
	m_max_energy(20.0f), m_energy(10.0f),
	m_energy_recovery_modifier(1.0f)
{
	next_id++;
};

void Meep::step(float dt) {
	float move_x = dt * m_vx * m_speed;
	float move_y = dt * m_vy * m_speed;
	m_x += move_x;
	m_y += move_y;

	float movement_sq = move_x * move_x + move_y * move_y;

	m_energy -= dt * passive_energy_drain + movement_sq * movement_energy_drain;
}

void Meep::setStableState() {
	m_state = Stable;
	m_vx = 0; m_vy = 0;
}
void Meep::updateState() {
	//std::cout << m_energy << std::endl;
	if (m_state == Stable) {
		if (m_energy < m_max_energy / 2) {
			m_state = LookingForFood;
		}
	}

	if (m_energy < 0) {
		m_state = Dead;
	}
}
MeepState Meep::getState() const {
	return m_state;
}
float Meep::getMaxEnergy() const {
	return m_max_energy;
}
float Meep::getEnergy() const {
	return m_energy;
}
float Meep::getEnergyRatio() const {
	return m_energy / m_max_energy;
}
float Meep::getSize() const {
	return m_size;
}
unsigned int Meep::getId() const {
	return m_id;
}
float Meep::getDistanceSquaredTo(float x, float y) {
	return Distances::pointToPointDistanceSquaredFloat(x, y, m_x, m_y);
	//float dx = x - m_x;
	//float dy = y - m_y;
	//return dx * dx + dy * dy;
}

std::optional<Food> Meep::closestFood(std::map<unsigned int, Food>& foods) {
	unsigned int closest = sense_radius * sense_radius;
	std::optional<Food> closest_food = std::nullopt;
	std::map<unsigned int, Food>::iterator it;
	for (it = foods.begin(); it != foods.end(); it++) {
		Food food = it->second;
		float distance = getDistanceSquaredTo(food.getX(), food.getY());
		if (distance < closest) {
			closest_food = food;
		}
	}
	return closest_food;
}

void Meep::moveToClosestFood(std::map<unsigned int, Food>& foods) {
	if (m_state == LookingForFood || m_state == MovingToFood) {
		std::optional<Food> closest = closestFood(foods);
		if (closest.has_value()) {
			float dx = closest.value().getX() - m_x;
			float dy = closest.value().getY() - m_y;
			float dist_sq = getDistanceSquaredTo(closest.value().getX(), closest.value().getY());
			float dist = sqrt(dist_sq);
			m_vx = dx / dist;
			m_vy = dy / dist;
			m_state = MovingToFood;
		}
	}
}

CollisionCircle Meep::getCollision() const {
	return CollisionCircle(m_x, m_y, getSize());
}
bool Meep::collideFood(Food const& food) const {
	return getCollision().collidesCircle(food.getCollision());
}
void Meep::eatFood(Food const& food) {
	eats++;
	m_energy += food.getEnergy();
	if (m_energy > m_max_energy / 2) {
		setStableState();
	}
	else {
		m_state = LookingForFood;
	}
}
std::vector<unsigned int> Meep::collideFood(std::map<unsigned int, Food>& foods) {
	std::vector<unsigned int> collision_food_ids;

	for (auto const& [food_id, food] : foods) {
		//Food food = it->second;
		if (collideFood(food)) {
			collision_food_ids.push_back(food_id);
			eatFood(food);
		}
	}

	return collision_food_ids;
}