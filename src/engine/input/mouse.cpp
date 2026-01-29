#include "engine/input/mouse.hpp"
#include "engine/core/timer.hpp"

using namespace engine::input;

double Mouse::m_positionX = 0.0;
double Mouse::m_positionY = 0.0;
double Mouse::m_lastPositionX = 0.0;
double Mouse::m_lastPositionY = 0.0;
double Mouse::m_positionDeltaX = 0.0;
double Mouse::m_positionDeltaY = 0.0;
double Mouse::m_scrollX = 0.0;
double Mouse::m_scrollY = 0.0;
double Mouse::m_scrollDeltaX = 0.0;
double Mouse::m_scrollDeltaY = 0.0;
double Mouse::m_scrollSpeed = 0.1;
bool Mouse::m_firstMove = true;

void Mouse::init(int width, int height)
{
    reset(width, height);
}

void Mouse::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (m_firstMove) {
        m_lastPositionX = xPos;
        m_lastPositionY = yPos;
        m_firstMove = false;
        return;
    }

    m_positionX = xPos;
    m_positionY = yPos;

    m_positionDeltaX = xPos - m_lastPositionX;
    m_positionDeltaY = m_lastPositionY - yPos;

    m_lastPositionX = xPos;
    m_lastPositionY = yPos;
}

void Mouse::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    m_scrollDeltaX = xOffset;
    m_scrollDeltaY = yOffset;
    m_scrollX += xOffset;
    m_scrollY += yOffset;
}

double Mouse::positionX()
{
    return m_positionX;
}

double Mouse::positionY()
{
    return m_positionY;
}

double Mouse::positionDeltaX()
{
    return m_positionDeltaX;
}

double Mouse::positionDeltaY()
{
    return m_positionDeltaY;
}

double Mouse::scrollDeltaX()
{
    return m_scrollDeltaX;
}

double Mouse::scrollDeltaY()
{
    return m_scrollDeltaY;
}

void Mouse::update()
{
    m_positionDeltaX = 0.0;
    m_positionDeltaY = 0.0;
    m_scrollDeltaX = 0.0;
    m_scrollDeltaY = 0.0;
}

void Mouse::reset(int width, int height)
{
    m_positionX = width / 2.0;
    m_positionY = height / 2.0;
    m_lastPositionX = m_positionX;
    m_lastPositionY = m_positionY;
    m_positionDeltaX = 0.0;
    m_positionDeltaY = 0.0;
    m_scrollX = 0.0;
    m_scrollY = 0.0;
    m_scrollDeltaX = 0.0;
    m_scrollDeltaY = 0.0;
    m_scrollSpeed = 0.1;
    m_firstMove = true;
}