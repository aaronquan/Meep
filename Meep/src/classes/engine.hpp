#pragma once
#include <vector>
#include <map>

#include "meep/meep.hpp"
//#include "meep/food.hpp"
#include "timer.hpp"
#include "random.hpp"
#include "shapes/shapes.hpp"

class Engine {
public:
	Engine();
	Engine(unsigned int width, unsigned int height);
	void setup();
	void clean();
	void spawn_food();
	void step(float dt);
	std::map<unsigned int, Meep> getMeeps() const;
	std::map<unsigned int, Food> getFoods() const;

	float getWidth() const;
	float getHeight() const;

private:
	std::map<unsigned int, Meep> m_meeps;
	std::map<unsigned int, Food> m_foods;

	float m_width;
	float m_height;

	Timer m_food_spawn_timer;
};