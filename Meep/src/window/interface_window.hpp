#pragma once

#include "window.hpp"
#include "./../utils/colour.hpp"

class InterfaceWindow {
public:
	InterfaceWindow();
	InterfaceWindow(int x, int y, int width, int height);
	const Dimensions& getDimensions() const;
	void setDimensions(int x, int y);
	void setDimensions(float x, float y);
	const WindowPosition& getPosition() const;
	void setPosition(float x, float y);
	const Colour& getBackgroundColour() const;
	void setBackgroundColour(Colour c);

	bool isInside(WindowPosition p) const;
	WindowPosition getRelativePosition(WindowPosition p) const;
private:
	WindowPosition m_position;
	Dimensions m_dims;
	Colour m_background_colour;
};