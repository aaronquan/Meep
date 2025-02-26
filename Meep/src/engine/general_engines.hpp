#pragma once

struct RectBound {
	float left;
	float right;
	float top;
	float bottom;
	RectBound(float l, float r, float t, float b) : left(l), right(r), top(t), bottom(b) {};
	void easyPrint() const {
		std::cout << left << ' ' << right << ' ' << top << ' ' << bottom << '\n';
	}
	float getWidth() const {
		return right - left;
	}
	float getHeight() const {
		return bottom - top;
	}
	bool isInBoundX(float x) const {
		return x >= left && x <= right;
	}
	bool isInBoundY(float y) const {
		return y >= top && y <= bottom;
	}
	bool isInBound(float x, float y) const {
		return isInBoundX(x) && isInBoundY(y);
	}
};

class Engine2D {
public:
	virtual float getWidth() const = 0;
	virtual float getHeight() const = 0;
};

class Engine2DWorldViewBound: public Engine2D {
public:
	//virtual float getViewWidth() const = 0;
	//virtual float getViewHeight() const = 0;
	virtual const RectBound& getWorldBound() const = 0;
	virtual const RectBound& getViewBound() const = 0;
};