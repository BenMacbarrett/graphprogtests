#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct shaderProgramSource
{
    const char* vertexSource;
    const char* fragmentSource;
    GLuint program;
};

enum x_direction
{
    X_NONE,
    X_LEFT,
    X_RIGHT
};

enum y_direction
{
    Y_NONE,
    Y_UP,
    Y_DOWN
};

class Window {
public:
    Window( const int width, const int height, const char* title);
    ~Window();

    void run( void );
private:
    // enum   _x_direction;
    enum y_direction _y_direction;
    struct GLFWwindow* _window;
    GLuint _shaderProgram;
    static void _framebuffer_size_callback( GLFWwindow* window, int width, int height );
    static void _keyboardCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
    static void _prepareShaderProgram( GLuint& shaderProgram );
};

#endif /* WINDOW_H */