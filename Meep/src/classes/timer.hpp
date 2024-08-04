#pragma once

class Timer {
public:
	Timer(float mili) : m_timer(mili), m_current(0){};
	unsigned int add_time(float dt) {
		m_current += dt;
		if (m_current > m_timer) {
			unsigned int procs = static_cast<unsigned int>(m_current / m_timer);
			m_current -= procs * m_timer;
			return procs;
		}
		return 0;
	}
private:
	float m_timer;
	float m_current;
};