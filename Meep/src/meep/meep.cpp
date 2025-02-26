#include "meep.hpp"

unsigned int Meep::next_id = 0;
float Meep::sense_radius = 200.0f;

float Meep::passive_energy_drain = 0.1f;
float Meep::movement_energy_drain = 0.01f;

float Meep::spawn_size = 0.5f;
float Meep::child_size = 2.0f;
std::unordered_set<unsigned int> Meep::reserved_food;

Meep::Meep() : Meep(4.0f, 20.0f, MeepStage::Cloning) {};
Meep::Meep(Meep* parent) : Meep() {};
Meep::Meep(float speed): Meep(1.0f, speed, MeepStage::Cloning) {};
Meep::Meep(float size, float speed, MeepStage stage)
	: Entity2D(0, 0),
	m_id(next_id),
	m_stage(stage),
	m_state(MeepState::Stable), m_speed(speed),
	m_vx(0), m_vy(0),
	m_rotation(0),
	m_size(size), m_n_eats(0),
	m_eat_speed(2.0),
	m_max_energy(20.0f), m_energy(10.0f),
	m_energy_recovery_modifier(1.0f),
	m_age(0),
	m_collision(0, 0, size),
	m_children(),
	m_target(nullptr),
	m_parent(nullptr)
{
	next_id++;
};

void Meep::step(float dt) {
	float move_x = 0;
	float move_y = 0;
	//std::cout << getX() << " " << getY() << std::endl;
	if(!(m_stage == MeepStage::Cloning || m_state == MeepState::Cloning)){
		move_x = dt * m_vx * m_speed;
		move_y = dt * m_vy * m_speed;
		move(move_x, move_y);
	}
	float movement_sq = move_x * move_x + move_y * move_y;

	m_energy -= dt * passive_energy_drain + movement_sq * movement_energy_drain;

	//meep growth
	if (m_energy > m_max_energy * 0.6) {
		m_energy -= 0.1 * dt;
		m_size += 0.1 * dt;
	}

	if (m_state == MeepState::Cloning) {
		for (Meep* m : m_children) {
			if(m->isCloningStage()){
				float change = 0.1*dt;
				setSize(getSize()-change);
				//std::cout << m->getSize() + change << std::endl;
				m->setSize(m->getSize()+change);
				if (m->getSize() > child_size) {
					m->growUp();
					m_state = MeepState::Stable;
				}
			}
		}
	}

}

void Meep::growUp() {
	if (isCloningStage()) {
		m_stage = MeepStage::Child;
		m_state = MeepState::Stable;
	}
}

bool Meep::isCloningStage() {
	return m_stage == MeepStage::Cloning;
}

void Meep::setStableState() {
	m_state = MeepState::Stable;
	m_vx = 0; m_vy = 0;
}

MeepStateChangeData Meep::updateState() {
	MeepStateChangeData data = MeepStateChangeData();
	//std::cout << m_energy << std::endl;
	if (m_state == MeepState::Stable) {
		if (m_stage == MeepStage::Adult && m_energy > m_max_energy*0.6 && m_size > child_size*2) {
			m_state = MeepState::Cloning;
			m_energy -= m_max_energy*0.25f;
			data.clone_meep = new Meep(spawn_size, m_speed, MeepStage::Cloning);
			m_children.push_back(data.clone_meep);
		}
		else if (m_energy < m_max_energy*0.8) {
			m_state = MeepState::LookingForFood;

		}
	}
	//std::cout << meepStateToString(m_state) << std::endl;

	if (m_energy < 0) {
		m_state = MeepState::Dead;
	}

	if (m_stage == MeepStage::Cloning) {
		m_state = MeepState::Cloning;
	}
	return data;
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

float Meep::getScale() const {
	return m_size*2;
}

float Meep::getSpeed() const {
	return m_speed;
}

float Meep::getRotation() const {
	return m_rotation;
}

glm::vec2 Meep::getDirection() const {
	return glm::vec2(m_vx, m_vy);
}

MapPosition* Meep::getTarget() const {
	return m_target;
}


unsigned int Meep::getId() const {
	return m_id;
}

void Meep::setVelocity(float x, float y) {
	m_vx = x; m_vy = y;
}

void Meep::setEating() {
	m_state = MeepState::Eating;
}

float Meep::getDistanceSquaredTo(float x, float y) {
	return Distances::pointToPointDistanceSquaredFloat(x, y, getX(), getY());
}

//to be unused
/*
void Meep::moveToClosestFood(std::map<unsigned int, Food*>& foods) {
	if (m_state == MeepState::LookingForFood) {
		std::optional<Food> closest = getClosestFood(foods);
		if (closest.has_value()) {
			Food closest_food = closest.value();
			float dx = closest_food.getX() - getX();
			float dy = closest_food.getY() - getY();
			float dist_sq = getDistanceSquaredTo(closest_food.getX(), closest_food.getY());
			float dist = sqrt(dist_sq);
			if (dist_sq > m_size*m_size) {
				float dist = sqrt(dist_sq);
				m_vx = dx / dist;
				m_vy = dy / dist;
				m_rotation = atan2(m_vy, m_vx);
				m_state = MeepState::MovingToFood;
				setTarget(closest_food.getX(), closest_food.getY());
				reserved_food.insert(closest_food.getId());
			}
			else {
				m_vx = 0;
				m_vy = 0;
				m_state = MeepState::Eating;
			}
			//m_state = MovingToFood;
		}
	}
}*/

CollisionCircle Meep::getCollision() const {
	return m_collision;
}
bool Meep::isCollisionFood(Food const& food) const {
	return m_collision.collidesCircle(food.getCollision());
}
bool Meep::isCollisionPoint(float x, float y) const {
	return m_collision.collidesPoint(x, y);
}
bool Meep::eatFood(Food& food, float dt) {
	//if (m_state == MeepState::LookingForFood || m_state == MeepState::MovingToFood) {
	//	m_state = MeepState::Eating;
	//	m_vx = 0;
	//	m_vy = 0;
	//}
	if(m_state == MeepState::Eating){
		//std::cout << "eating" << std::endl;
		float energy_gain = food.eaten(dt*m_eat_speed);
		m_energy += energy_gain;
		if (m_energy >= m_max_energy) {
			setStableState();
		}
		else {
			//m_state = MeepState::Stable;
		}
		return true;
	}
	return false;
}
std::vector<unsigned int> Meep::collideFood(std::map<unsigned int, Food*>& foods, float dt) {
	std::vector<unsigned int> collision_food_ids;
	bool eating = false;
	for (auto& [food_id, food] : foods) {
		if (isCollisionFood(*food)) {
			//collision_food_ids.push_back(food_id);
			if(eatFood(*food, dt)){
				eating = true;
			}
			if (food->isFinished()) {
				setNoTarget();
				collision_food_ids.push_back(food_id);
			}
		}
	}
	if (!eating && m_state == MeepState::Eating) {
		m_state = MeepState::Stable;
	}

	return collision_food_ids;
}

void Meep::setTargetFood(Meep& meep, std::map<unsigned int, Food*>& foods) {
	if (meep.getState() == MeepState::LookingForFood) {
		std::optional<Food> closest = meep.getClosestFood(foods);
		if (closest.has_value()) {
			Food closest_food = closest.value();
			float dx = closest_food.getX() - meep.m_position.x;
			float dy = closest_food.getY() - meep.m_position.y;
			float dist_sq = meep.getDistanceSquaredTo(closest_food.getX(), closest_food.getY());
			float size = meep.getSize();
			if (dist_sq > size*size) {
				meep.setTarget(closest_food.getX(), closest_food.getY());
				reserved_food.insert(closest_food.getId());
				meep.m_state = MeepState::MovingToFood;
			}
		}
	}
}

std::optional<Food> Meep::getClosestFood(std::map<unsigned int, Food*>& foods) {
	float closest = sense_radius * sense_radius;
	std::optional<Food> closest_food = std::nullopt;
	std::map<unsigned int, Food*>::iterator it;
	for (it = foods.begin(); it != foods.end(); it++) {
		Food* food = it->second;
		if (reserved_food.find(food->getId()) == reserved_food.end()) {
			float distance = getDistanceSquaredTo(food->getX(), food->getY());
			if (distance < closest) {
				closest = distance;
				closest_food = *food;
			}
		}
	}
	return closest_food;
}

void Meep::move(float mx, float my) {
	Entity2D::move(mx, my);
	m_collision.moveTo(getX(), getY());
}

void Meep::setSize(float size) {
	m_size = size;
	m_collision.setRadius(m_size);
}

void Meep::setTarget(float x, float y) {
	if(m_target != nullptr) delete m_target;
	m_target = new MapPosition(x, y);
}

void Meep::setNoTarget() {
	delete m_target;
	m_target = nullptr;
}

std::string meepStateToString(MeepState state) {
	switch (state) {
	case MeepState::MovingToFood:
		return "Moving to food";
	}
	return "";
}