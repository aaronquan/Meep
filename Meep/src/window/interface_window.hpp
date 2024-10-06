#pragma once

#include "window.hpp"
#include "./../utils/colour.hpp"

class InterfaceWindow {
public:
	InterfaceWindow(int x, int y, int width, int height);
	Dimensions getDimensions() const;
	Position getPosition() const;
	Colour getBackgroundColour() const;
	void setBackgroundColour(Colour c);

	bool isInside(Position p) const;
private:
	Position m_position;
	Dimensions m_dims;
	Colour m_background_colour;
};