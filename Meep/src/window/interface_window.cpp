#include "interface_window.hpp"

InterfaceWindow::InterfaceWindow(int x, int y, int width, int height) :
    m_position(x, y), m_dims(width, height), m_background_colour(100, 0, 100)
{}

Dimensions InterfaceWindow::getDimensions() const {
    return m_dims;
}

Position InterfaceWindow::getPosition() const {
    return m_position;
}

Colour InterfaceWindow::getBackgroundColour() const {
    return m_background_colour;
}

void InterfaceWindow::setBackgroundColour(Colour c) {
    m_background_colour = c;
}

bool InterfaceWindow::isInside(Position p) const {
    return p.x > m_position.x && p.x < m_position.x + m_dims.width
        && p.y > m_position.y && p.y < m_position.y + m_dims.height;
}