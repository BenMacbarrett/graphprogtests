
#include <iostream>
#include "window.h"


Window::Window(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        throw std::runtime_error("GLFW Initialization Failed");
    }

    this->_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!this->_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("GLFW Window Creation Failed");
    }

    glfwSwapInterval(1); 


    glfwMakeContextCurrent(this->_window);

    glfwSetKeyCallback(this->_window, this->_keyboardCallback);
}

Window::~Window()
{
    if (this->_window)
    {
        glfwDestroyWindow(this->_window);
    }
    glfwTerminate();
}

void Window::run()
{
    std::cout << std::endl;
    while (!glfwWindowShouldClose(this->_window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render OpenGL content here

        glfwSwapBuffers(this->_window);
        glfwWaitEvents();
        double x = 0.0;
        double y = 0.0;
        // glfwGetCursorPos(this->_window, &x, &y);
        // std::cout << "Cursor Position: (" << x << ", " << y << ")" << std::endl;
    }
}

void Window::_keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ( void )window;
    ( void )action;
    ( void )mods;
    
    if( ( GLFW_REPEAT == action ) || ( action == GLFW_PRESS ) )
    {
        if( ( key >= GLFW_KEY_A ) && ( key <= GLFW_KEY_Z ) )
        {
            std::cout << (( mods == GLFW_MOD_SHIFT ) ? static_cast<char>(key) : static_cast<char>(tolower(key))) << std::flush;
        }
        else if( key == GLFW_KEY_SPACE )
        {
            std::cout << ' ' << std::flush;
        }
        else if( key == GLFW_KEY_ENTER )
        {
            std::cout << std::endl;
        }
    }    
}
