#include <iostream>
#include "window.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

int main( void ) 
{
    int return_code = 0;

    Window* window_instance = new Window( 800, 600, "OpenGL Window" );
    if( window_instance == nullptr ) 
    {
        std::cerr << "Failed to create Window instance" << std::endl;
        return_code = -1;
    }
    else
    {
        window_instance->run();
        delete window_instance;
    }

    // glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    // if (window == NULL)
    // {
    //     std::cout << "Failed to create GLFW window" << std::endl;
    //     glfwTerminate();
    //     return -1;
    // }

    // glfwMakeContextCurrent(window);
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    // {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }   

    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // while(!glfwWindowShouldClose(window))
    // {
    //     glfwSwapBuffers(window);
    //     glfwPollEvents();    
    // }


    // glfwTerminate();

    return return_code;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  
