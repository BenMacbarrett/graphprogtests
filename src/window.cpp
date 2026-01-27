

#include <iostream>
#include "window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD Initialization Failed");
    }

    this->_prepareShaderProgram(this->_shaderProgram);
    glfwSetFramebufferSizeCallback(this->_window, this->_framebuffer_size_callback);

    glfwSetKeyCallback(this->_window, this->_keyboardCallback);
    // this->_x_direction = X_NONE;
    // this->_y_direction = Y_NONE;
}

Window::~Window()
{
    if (this->_window)
    {
        glfwDestroyWindow(this->_window);
    }
    glfwTerminate();
}

void Window::_prepareShaderProgram(GLuint& shaderProgram)
{
    // ========== SHADER SETUP ==========
    // Shaders are small programs that run on the GPU to process vertices and pixels.
    // The vertex shader processes each vertex (corner point) of our geometry.
    // The fragment shader determines the color of each pixel.
    
    // VERTEX SHADER: Runs once per vertex
    // - Takes in vertex position (aPos) and texture coordinates (aTexCoord)
    // - Outputs the transformed position (gl_Position) and passes texture coords to fragment shader
    // - Here we just pass positions through unchanged (orthographic 2D space from -1 to 1)
    // const char* vertexSource = R"(
    //     #version 330 core
    //     layout (location = 0) in vec2 aPos;        // Vertex position input
    //     layout (location = 1) in vec2 aTexCoord;   // Texture coordinate input
    //     out vec2 TexCoord;                         // Pass texture coords to fragment shader
    //     void main()
    //     {
    //         gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);  // Set vertex position
    //         TexCoord = aTexCoord;                           // Pass through texture coords
    //     }
    // )";
    
    // // FRAGMENT SHADER: Runs once per pixel
    // // - Takes in interpolated texture coordinates from the vertex shader
    // // - Samples the texture at those coordinates to get the pixel color
    // // - Outputs the final color (FragColor) for that pixel
    // const char* fragmentSource = R"(
    //     #version 330 core
    //     out vec4 FragColor;              // Output color for this pixel
    //     in vec2 TexCoord;                // Texture coordinates (interpolated from vertices)
    //     uniform sampler2D texture1;      // The texture to sample from
    //     void main()
    //     {
    //         FragColor = texture(texture1, TexCoord);  // Sample texture at coords
    //     }
    // )";
    
    // // ========== SHADER COMPILATION ==========
    // // Shaders need to be compiled from source code into GPU instructions
    
    // // Create and compile vertex shader
    // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);     // Create a vertex shader object
    // glShaderSource(vertexShader, 1, &vertexSource, NULL); // Attach source code
    // glCompileShader(vertexShader);                              // Compile it
    
    // // Check if vertex shader compiled successfully
    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //     std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    // }
    
    // // Create and compile fragment shader
    // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);   // Create a fragment shader object
    // glShaderSource(fragmentShader, 1, &fragmentSource, NULL); // Attach source code
    // glCompileShader(fragmentShader);                              // Compile it
    
    // // Check if fragment shader compiled successfully
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    // }
    
    // // ========== SHADER PROGRAM LINKING ==========
    // // A shader program combines vertex and fragment shaders into a complete pipeline
    // shaderProgram = glCreateProgram();          // Create program object
    // glAttachShader(shaderProgram, vertexShader);       // Attach vertex shader
    // glAttachShader(shaderProgram, fragmentShader);     // Attach fragment shader
    // glLinkProgram(shaderProgram);                      // Link them together
    
    // // Check if linking was successful
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if (!success)
    // {
    //     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //     std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    // }
    
    // // Clean up individual shaders (no longer needed after linking)
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
}

void Window::run()
{
    // // ========== VERTEX DATA ==========
    // // Define a quad (rectangle) as two triangles to display our texture on
    // // Each vertex has: [x, y, texture_u, texture_v]
    // // Position coords (x,y) range from -0.5 to 0.5 (will be centered on screen)
    // // Texture coords (u,v) range from 0.0 to 1.0 (map to the image)
    // //   (0,0) = top-left of image, (1,1) = bottom-right of image
    // float vertices[] = {
    //     // positions      // texture coords
    //     -0.5f, -0.5f,     0.0f, 1.0f,   // Vertex 0: bottom-left position, bottom of texture
    //      0.5f, -0.5f,     1.0f, 1.0f,   // Vertex 1: bottom-right position, bottom of texture
    //      0.5f,  0.5f,     1.0f, 0.0f,   // Vertex 2: top-right position, top of texture
    //     -0.5f,  0.5f,     0.0f, 0.0f    // Vertex 3: top-left position, top of texture
    // };
    
    // // Indices define which vertices form each triangle (counter-clockwise winding)
    // // Triangle 1: vertices 0, 1, 2 (bottom-left, bottom-right, top-right)
    // // Triangle 2: vertices 2, 3, 0 (top-right, top-left, bottom-left)
    // unsigned int indices[] = {
    //     0, 1, 2,   // First triangle
    //     2, 3, 0    // Second triangle
    // };
    
    // // ========== VERTEX BUFFER SETUP ==========
    // // VAO (Vertex Array Object): Stores the configuration of how to read vertex data
    // // VBO (Vertex Buffer Object): Stores the actual vertex data in GPU memory
    // // EBO (Element Buffer Object): Stores indices to avoid duplicate vertices
    // GLuint VAO, VBO, EBO;
    // glGenVertexArrays(1, &VAO);  // Create VAO
    // glGenBuffers(1, &VBO);       // Create VBO
    // glGenBuffers(1, &EBO);       // Create EBO
    
    // // Bind VAO first - it will remember all subsequent vertex attribute configurations
    // glBindVertexArray(VAO);
    
    // // Upload vertex data to GPU
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);                                      // Bind VBO
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Upload data
    
    // // Upload index data to GPU
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                // Bind EBO
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Upload data
    
    // // Configure how OpenGL should interpret the vertex data
    // // Position attribute (location = 0 in vertex shader)
    // // - 2 components (x, y)
    // // - Each vertex has 4 floats total (x, y, u, v)
    // // - Position starts at offset 0
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    
    // // Texture coordinate attribute (location = 1 in vertex shader)
    // // - 2 components (u, v)
    // // - Each vertex has 4 floats total (x, y, u, v)
    // // - Texture coords start at offset 2 (after x and y)
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    
    // // ========== TEXTURE LOADING ==========
    // // Load the PNG image from disk using stb_image library
    // int width, height, channels;
    // // stbi_load returns pixel data, last parameter (4) forces RGBA format
    // unsigned char* data = stbi_load("../images/pixelart/pixil-frame-0.png", &width, &height, &channels, 4);
    // if (!data)
    // {
    //     std::cerr << "Failed to load texture: " << stbi_failure_reason() << std::endl;
    //     return;
    // }
    
    // // Create and configure OpenGL texture
    // GLuint texture;
    // glGenTextures(1, &texture);              // Generate texture ID
    // glBindTexture(GL_TEXTURE_2D, texture);   // Bind it for configuration

    // // TEXTURE FILTERING: How to sample when texture is scaled
    // // GL_NEAREST = sharp pixels (best for pixel art, no blurring)
    // // GL_LINEAR = smooth interpolation (would blur pixel art)
    // // MIN_FILTER = when texture is smaller than screen space
    // // MAG_FILTER = when texture is larger than screen space
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // // TEXTURE WRAPPING: What happens at texture edges
    // // GL_CLAMP_TO_EDGE = don't repeat, clamp to edge pixels
    // // S = horizontal axis, T = vertical axis (like X and Y for textures)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // // Upload texture data to GPU memory
    // // Parameters: target, mipmap level, internal format, width, height, border, format, type, data
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // stbi_image_free(data);  // Free CPU memory (GPU has a copy now)
    
    // // Enable alpha blending for transparency
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Standard alpha blending
    
    // ========== RENDER LOOP ==========
    // This loop runs every frame until the window is closed
    while (!glfwWindowShouldClose(this->_window))
    {
        // Clear the screen with a background color (dark teal: R=0.2, G=0.3, B=0.3, A=1.0)
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);  // Actually clear the color buffer

        // // Draw the textured quad
        // glUseProgram(this->_shaderProgram);          // Activate our shader program
        // glBindTexture(GL_TEXTURE_2D, texture); // Bind our texture (sent to fragment shader)
        // glBindVertexArray(VAO);               // Bind our vertex configuration
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // Draw 6 vertices as triangles
        //             ^^^^^^^^^^^   ^  ^^^^^^^^^^^^^^^^^^^
        //             draw mode     |  index buffer type & offset
        //                           number of indices to draw

        // Swap front and back buffers (double buffering prevents flickering)
        glfwSwapBuffers(this->_window);
        
        // Process window events (keyboard, mouse, window resize, etc.)
        glfwPollEvents();
    }
    
    // // ========== CLEANUP ==========
    // // Free OpenGL resources (good practice to prevent memory leaks)
    // glDeleteVertexArrays(1, &VAO);      // Delete vertex array object
    // glDeleteBuffers(1, &VBO);           // Delete vertex buffer
    // glDeleteBuffers(1, &EBO);           // Delete element buffer
    // glDeleteProgram(this->_shaderProgram);     // Delete shader program
    // glDeleteTextures(1, &texture);      // Delete texture
}

// ========== HOW TO USE THIS CODE ==========
// 1. Change the image path in stbi_load() to load different images
// 2. Adjust vertex positions (vertices array) to change quad size/position
//    - Values range from -1.0 to 1.0 in OpenGL's normalized device coordinates
//    - (0,0) is screen center, (-1,-1) is bottom-left, (1,1) is top-right
// 3. Modify texture filtering (GL_NEAREST vs GL_LINEAR) for different visual styles
// 4. Change glClearColor() for different background colors
// 5. Add uniforms to shaders to pass data like position, scale, rotation, etc.

void Window::_keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ( void )window;
    ( void )action;
    ( void )mods;
    

    // switch( key )
    // {
    //     case GLFW_KEY_W:
    //     case GLFW_KEY_S:
    //         this->_y_direction = ( action == GLFW_PRESS ) ? Y_UP : ( action == GLFW_PRESS ) ? Y_NONE;
    //         break;
    //     case GLFW_KEY_DOWN:
    //         this->_y_direction = ( action == GLFW_PRESS ) ? Y_DOWN : Y_NONE;
    //         break;
    //     case GLFW_KEY_A:
    //         this->_x_direction = ( action == GLFW_PRESS ) ? X_LEFT : X_NONE;
    //         break;
    //     case GLFW_KEY_RIGHT:
    //         this->_x_direction = ( action == GLFW_PRESS ) ? X_RIGHT : X_NONE;
    //         break;
    //     default:
    //         break;
    // }

    // if( ( GLFW_REPEAT == action ) || ( action == GLFW_PRESS ) )
    // {
    //     switch
    //     if( ( key >= GLFW_KEY_A ) && ( key <= GLFW_KEY_Z ) )
    //     {
    //         std::cout << (( mods == GLFW_MOD_SHIFT ) ? static_cast<char>(key) : static_cast<char>(tolower(key))) << std::flush;

    //         if( key == GLFW_KEY_W )
    //         {
    //             std::cout << " Up arrow pressed" << std::endl;
    //             this->_direction = UP;
    //         }
    //         else if( key == GLFW_KEY_S )
    //         {
    //             std::cout << " Down arrow pressed" << std::endl;
    //             this->_direction = DOWN;
    //         }
    //         else if( key == GLFW_KEY_A )
    //         {
    //             std::cout << " Left arrow pressed" << std::endl;
    //             this->_direction = LEFT;
    //         }
    //         else if( key == GLFW_KEY_D )
    //         {
    //             std::cout << " Right arrow pressed" << std::endl;
    //             this->_direction = RIGHT;
    //         }
    //     }
    //     else if( key == GLFW_KEY_SPACE )
    //     {
    //         std::cout << ' ' << std::flush;
    //     }
    //     else if( key == GLFW_KEY_ENTER )
    //     {
    //         std::cout << std::endl;
    //     }
    // }
}

void Window::_framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
    glViewport(0, 0, width, height);
}
