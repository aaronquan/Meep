#pragma once

#include "window.hpp"
#include "./../utils/colour.hpp"

class InterfaceWindow {
public:
	InterfaceWindow();
	InterfaceWindow(int x, int y, int width, int height);
	Dimensions getDimensions() const;
	void setDimensions(float x, float y);
	Position getPosition() const;
	void setPosition(float x, float y);
	Colour getBackgroundColour() const;
	void setBackgroundColour(Colour c);

	bool isInside(Position p) const;
	Position getRelativePosition(Position p) const;
private:
	Position m_position;
	Dimensions m_dims;
	Colour m_background_colour;
};