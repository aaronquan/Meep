#include "interface_window.hpp"

InterfaceWindow::InterfaceWindow(): InterfaceWindow(0, 0, 0, 0){};

InterfaceWindow::InterfaceWindow(int x, int y, int width, int height) :
    m_position(x, y), m_dims(width, height), m_background_colour(0, 0, 0)
{}

const Dimensions& InterfaceWindow::getDimensions() const {
    return m_dims;
}

void InterfaceWindow::setDimensions(int x, int y) {
    m_dims.width = x; m_dims.height = y;
}

void InterfaceWindow::setDimensions(float x, float y) {
    m_dims.width = static_cast<int>(x); m_dims.height = static_cast<int>(y);
}

const WindowPosition& InterfaceWindow::getPosition() const {
    return m_position;
}

void InterfaceWindow::setPosition(float x, float y) {
    m_position.x = x; m_position.y = y;
}

const Colour& InterfaceWindow::getBackgroundColour() const {
    return m_background_colour;
}

void InterfaceWindow::setBackgroundColour(Colour c) {
    m_background_colour = c;
}

bool InterfaceWindow::isInside(WindowPosition p) const {
    return p.x > m_position.x && p.x < m_position.x + m_dims.width
        && p.y > m_position.y && p.y < m_position.y + m_dims.height;
}

WindowPosition InterfaceWindow::getRelativePosition(WindowPosition p) const {
    return WindowPosition(p.x - m_position.x, p.y - m_position.y);
}