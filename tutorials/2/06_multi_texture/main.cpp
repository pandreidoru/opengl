#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader_program.h"
#include "texture_2d.h"

// Window dimensions
static GLint const kWidth{800};
static GLint const kHeight{600};
static char const* const kAppTitle = "Hello Shader";
static int const kColorMin{0};
static int const kColorMax{255};
int const kVerticesNumber{3};
int const kTexturesNumber{2};
auto const kNormalized{GL_FALSE};
int const kStride{(kVerticesNumber + kTexturesNumber) * sizeof(float)};
GLvoid const* const kOffset{nullptr};
bool gWireframe{false};

GLFWwindow* InitOpenGL();
void CleanOpenGL(std::string const& msg);
void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
float NormalizeColor(int value);

std::string const kTexture1{"../textures/airplane.PNG"};
std::string const kTexture2{"../textures/crate.jpg"};

int main() {
  int ret{EXIT_SUCCESS};
  auto window = InitOpenGL();
  if (window) {
    // Vertex Buffer Object Position Layout
    // clang-format off
    GLfloat vertices[] = {
        // position          // texture1_filename coords
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // top left
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f   // bottom left
    };

    GLuint indices[] = {
         0, 1, 2,  // tri 0
         0, 2, 3   // tri 1
    };
    // clang-format on

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Position
    glVertexAttribPointer(0, kVerticesNumber, GL_FLOAT, kNormalized, kStride, kOffset);
    glEnableVertexAttribArray(0);
    // Texture coords
    glVertexAttribPointer(1, kTexturesNumber, GL_FLOAT, kNormalized, kStride, (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);  // unbind to make sure other code doesn't change it

    ShaderProgram shader_program;
    shader_program.LoadShader("../basic.vert", "../basic.frag");

    Texture2D texture1;
    texture1.Load(kTexture1, true);

    Texture2D texture2;
    texture2.Load(kTexture2, true);

    while (!glfwWindowShouldClose(window)) {
      ShowFPS(window);
      // Poll for and process events
      glfwPollEvents();
      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT);

      texture1.Bind(0);
      texture2.Bind(1);

      shader_program.Use();
      glUniform1i(glGetUniformLocation(shader_program.GetProgram(), "my_texture1"), 0);
      glUniform1i(glGetUniformLocation(shader_program.GetProgram(), "my_texture2"), 1);

      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      glBindVertexArray(0);

      glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
  } else {
    ret = EXIT_FAILURE;
  }

  glfwTerminate();
  return ret;
}

GLFWwindow* InitOpenGL() {
  GLFWwindow* window{nullptr};
  if (glfwInit()) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(kWidth, kHeight, kAppTitle, nullptr, nullptr);

    if (window) {
      int buffer_width;
      int buffer_height;
      glfwGetFramebufferSize(window, &buffer_width, &buffer_height);

      // Set context for GLEW to use
      glfwMakeContextCurrent(window);

      // Allow modern extension features
      glewExperimental = GL_TRUE;

      if (glewInit() == GLEW_OK) {
        // Setup Viewport size
        glViewport(0, 0, buffer_width, buffer_height);
        glfwSetKeyCallback(window, OnKey);

        glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
      } else {
        CleanOpenGL("GLEW initialization failed!\n");
        glfwDestroyWindow(window);
        window = nullptr;
      }
    } else {
      CleanOpenGL("GLFW window creation failed!\n");
    }
  } else {
    CleanOpenGL("GLFW initialization failed!\n");
  }

  return window;
}

void CleanOpenGL(std::string const& msg) {
  std::cerr << msg;
  glfwTerminate();
}

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    gWireframe = !gWireframe;
    glPolygonMode(GL_FRONT_AND_BACK, (gWireframe ? GL_LINE : GL_FILL));
  }
}

/**
 * @brief Computes the average frames per second and the average time it takes to render one frame.
 * These stats are appended to the window caption bar.
 * @param window
 */
void ShowFPS(GLFWwindow* window) {
  static double prev_time{0};
  static int frame_count{0};
  double cur_time = glfwGetTime();
  double elapsed_time = cur_time - prev_time;

  // Limit text update - 4 times per second
  if (elapsed_time > 0.25) {
    prev_time = cur_time;
    double fps = frame_count / elapsed_time;
    double ms_per_frame = 1000 / fps;

    std::ostringstream out;
    out.precision(3);
    out << std::fixed
        << kAppTitle << " "
        << "FPS: " << fps << " "
        << " Frame Time: " << ms_per_frame << " (ms)";
    glfwSetWindowTitle(window, out.str().c_str());

    frame_count = 0;
  }

  frame_count++;
}

float NormalizeColor(int value) {
  float ret{0};
  if (value < kColorMin) {
    std::cerr << "Color value " << value << " less than " << kColorMin
              << ". It is converted to " << kColorMin << "\n";
    ret = kColorMin;
  } else if (value > kColorMax) {
    std::cerr << "Color value " << value << " grater than " << kColorMax
              << ". It is converted to " << kColorMax << "\n";
    ret = kColorMax;
  } else {
    ret = static_cast<float>(value % (kColorMax + 1)) / kColorMax;
  }
  return ret;
}