#ifndef WINDOW_H
#define WINDOW_H
#include <GLFW/glfw3.h>

class Window {
public:
    Window( const int width, const int height, const char* title);
    ~Window();

    void run( void );
private:
    struct GLFWwindow* _window;
    static void _keyboardCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
};

#endif /* WINDOW_H */