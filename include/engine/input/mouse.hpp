#ifndef MOUSE_HPP
#define MOUSE_HPP

#pragma MOUSE_HPP

#include <GLFW/glfw3.h>

namespace engine {
    namespace input {
        class Mouse {
            private:
            static double m_positionX;
            static double m_positionY;
            static double m_lastPositionX;
            static double m_lastPositionY;
            static double m_positionDeltaX;
            static double m_positionDeltaY;
            static double m_scrollX;
            static double m_scrollY;
            static double m_scrollDeltaX;
            static double m_scrollDeltaY;
            static double m_scrollSpeed;
            static bool m_firstMove;

            public:
            Mouse() = delete;

            static void init(int widht, int height);

            static void cursorPositionCallback(GLFWwindow*, double xPos, double yPos);
            static void mouseButtonCallback(GLFWwindow*, int key, int action, int mods);
            static void scrollCallback(GLFWwindow*, double xOffset, double yOffset);

            static double positionX();
            static double positionY();
            static double positionDeltaX(); 
            static double positionDeltaY();
            static double scrollDeltaX();
            static double scrollDeltaY();

            static void update();
            static void reset(int width, int height);
        };
    }
}

#endif // MOUSE_HPP