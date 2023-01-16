#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
  )";

const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
       FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
  )";;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void check_shader_compile_status(unsigned int shader)
{
}

int main()
{
    // Initialize GLFW
    glfwInit();

    // Set the version of OpenGL to use 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window with 800x800 dimensions, named "Test"
    GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);

    // Check if the window was created successfully
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the window as the current OpenGL context
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    gladLoadGL();

    // Set the viewport to the size of the window
    glViewport(0, 0, 800, 600);

    // Resize buffer on window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Vertex Shader
    float vertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
    };  

    // Init logging variables
    int success;
    char infoLog[512];

    // Init shaders and VBO / VBA
    unsigned int vertexShader, fragmentShader, shaderProgram, VBO, VAO;

    // Create vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    };


    // Use program ( every shader and rendering call will now use this program object and its' shaders )
    glUseProgram(shaderProgram);

    // Delete shader objects ( no longer needed since already linked )
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create vertex object buffer + bind VBO + copies vertex data into buffer's memory
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate vertex arrays + bind VAO + copy verticies array into buffer
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set vertex attributes pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // The main loop of the program
    while(!glfwWindowShouldClose(window)) {
      // Process user input
      processInput(window);

      // Clear screen with color
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      
      // Render
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      // Swap buffer
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    // End
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
