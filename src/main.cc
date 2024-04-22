#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <iostream>

// Vertex shader code
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 position;
    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

// Fragment shader code
const char *fragmentShaderSource = R"(
    #version 330 core
    uniform sampler2D image1;
    uniform sampler2D image2;
    uniform sampler2D mask;
    out vec4 FragColor;
    void main() {
        vec4 texColor1 = texture(image1, 1.0 - (gl_FragCoord.xy / textureSize(image1, 0)));
        vec4 texColor2 = texture(image2, 1.0 - (gl_FragCoord.xy / textureSize(image2, 0)));
        vec4 maskColor = texture(mask, 1.0 - (gl_FragCoord.xy / textureSize(mask, 0)));
        FragColor = mix(texColor1, texColor2, maskColor.r); // Use red channel of mask as blend factor
    }
)";

// Function to load an image into a texture
GLuint loadTexture(const char *filename) {
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  int width, height, channels;
  unsigned char *image =
      SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGBA);
  if (!image) {
    std::cerr << "Failed to load texture: " << filename << std::endl;
    return 0;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  return textureID;
}

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x anti-aliasing
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create GLFW window
  GLFWwindow *window =
      glfwCreateWindow(768, 1024, "OpenGL Masked Merge", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err)
              << std::endl;
    glfwTerminate();
    return -1;
  }

  // Compile vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  // Check for vertex shader compilation errors

  // Compile fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  // Check for fragment shader compilation errors

  // Link shaders into shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for shader program linking errors

  // Load textures
  GLuint texture1 = loadTexture("/home/jedi/Downloads/bg.png");
  GLuint texture2 = loadTexture("/home/jedi/Downloads/test_1.png");
  GLuint maskTexture = loadTexture("/home/jedi/Downloads/test_1_mask.png");

  // Create VBO and VAO for fullscreen quad
  float vertices[] = {-1.0f, -1.0f,  // bottom left
                      1.0f,  -1.0f,  // bottom right
                      1.0f,  1.0f,   // top right
                      1.0f,  1.0f,   // top right
                      -1.0f, 1.0f,   // top left
                      -1.0f, -1.0f}; // bottom left
  GLuint VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Process input
    glfwPollEvents();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader program
    glUseProgram(shaderProgram);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(shaderProgram, "image1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(shaderProgram, "image2"), 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, maskTexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "mask"), 2);

    // Draw fullscreen quad
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
  }

  // Cleanup resources
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glfwTerminate();

  return 0;
}
