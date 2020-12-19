#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_program.h"
#include "texture_2d.h"

// Window dimensions
static GLint gWindowWidth{1024};
static GLint gWindowHeight{768};
bool gWireframe{false};
static char const* const kAppTitle = "Hello Shader";
int const kVerticesNumber{3};
int const kTexturesNumber{2};
auto const kNormalized{GL_FALSE};
int const kStride{(kVerticesNumber + kTexturesNumber) * sizeof(float)};
GLvoid const* const kOffset{nullptr};

GLFWwindow* InitOpenGL();
void CleanOpenGL(std::string const& msg);
void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
void OnFrameBufferSize(GLFWwindow* window, int width, int height);

std::string const kTexture1{"../textures/wooden_crate.jpg"};
std::string const kTexture2{"../textures/crate.jpg"};

int main() {
  int ret{EXIT_SUCCESS};
  auto window = InitOpenGL();
  if (window) {
    // Vertex Buffer Object Position Layout
    // clang-format off
    GLfloat vertices[] = {
        // position		 // tex coords

        // front face
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

        // back face
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        // left face
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

        // right face
         1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        // top face
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

        // bottom face
        -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
    };
    // clang-format on

    glm::vec3 cube_pos = glm::vec3(0.0f, 0.0f, -5.0f);

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
    glVertexAttribPointer(1, kTexturesNumber, GL_FLOAT, kNormalized, kStride, (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    ShaderProgram shader_program;
    shader_program.LoadShader("../basic.vert", "../basic.frag");

    Texture2D texture1;
    texture1.Load(kTexture1, true);

    // Texture2D texture2;
    // texture2.Load(kTexture2, true);

    float cube_angle{0.0f};
    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
      ShowFPS(window);

      double delta_time = glfwGetTime() - last_time;

      // Poll for and process events
      glfwPollEvents();
      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      texture1.Bind(0);
      // texture2.Bind(1);

      cube_angle += (float)(delta_time * 50.0f);
      if (cube_angle >= 360.0f) {
        cube_angle = 0.0f;
      }

      glm::mat4 model{1.0};
      glm::mat4 view{1.0};
      glm::mat4 projection{1.0};

      model = glm::translate(model, cube_pos) * glm::rotate(model, glm::radians(cube_angle),
                                                            glm::vec3(0.0f, 1.0f, 0.0f));
      // Location of the virtual camera in the world
      glm::vec3 cam_pos(0.0f, 0.0f, 0.0f);
      // The aim point where the camera is looking
      glm::vec3 target_pos(0.0f, 0.0f, -1.0f);
      // Up direction unit vector of the camera
      glm::vec3 up(0.0f, 1.0f, 0.0f);
      view = glm::lookAt(cam_pos, target_pos, up);

      projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

      shader_program.Use();

      shader_program.SetUniform("model", model);
      shader_program.SetUniform("view", view);
      shader_program.SetUniform("projection", projection);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);

      glfwSwapBuffers(window);

      last_time += delta_time;
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
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

    window = glfwCreateWindow(gWindowWidth, gWindowHeight, kAppTitle, nullptr, nullptr);

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
        glViewport(0, 0, gWindowWidth, gWindowHeight);
        glEnable(GL_DEPTH_TEST);
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

void OnFrameBufferSize(GLFWwindow* window, int width, int height) {
  gWindowWidth = width;
  gWindowHeight = height;
  glViewport(0, 0, gWindowWidth, gWindowHeight);
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

