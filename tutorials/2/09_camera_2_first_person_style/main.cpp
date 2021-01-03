#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
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

std::string const kWoodenCrateTex{"../textures/wooden_crate.jpg"};
std::string const kFloorTex{"../textures/grid.jpg"};

FPSCamera gFPSCamera(glm::vec3(0.0f, 0.0f, 5.0f));
double const kZoomSensitivity{-3};
float const kMoveSpeed{5};  // units per second
float const kMouseSensitivity{0.1};

GLFWwindow* InitOpenGL();
void CleanOpenGL(std::string const& msg);
void GLFWOnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void GLFWOnFrameBufferSize(GLFWwindow* window, int width, int height);
void GLFWOnMouseMove(GLFWwindow* window, double pos_x, double pos_y);
void GLFWOnMouseScroll(GLFWwindow* window, double delta_x, double delta_y);
void Update(GLFWwindow* window, double elapsed_time);
void ShowFPS(GLFWwindow* window);

int main() {
  auto ret{EXIT_SUCCESS};
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

    auto cube_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    auto floor_pos = glm::vec3(0.0f, -1.0f, 0.0f);

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

    Texture2D wooden_crate;
    wooden_crate.Load(kWoodenCrateTex, true);

    Texture2D floor_tex;
    floor_tex.Load(kFloorTex, true);

    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
      ShowFPS(window);

      double delta_time = glfwGetTime() - last_time;

      // Poll for and process events
      glfwPollEvents();
      Update(window, delta_time);

      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      wooden_crate.Bind(0);

      glm::mat4 model{1.0};
      glm::mat4 view{1.0};
      glm::mat4 projection{1.0};

      model = glm::translate(model, cube_pos);

      view = gFPSCamera.GetViewMatrix();

      projection = glm::perspective(glm::radians(gFPSCamera.GetFOV()),
                                    (float) gWindowWidth / (float) gWindowHeight,
                                    0.1f, 100.0f);

      shader_program.Use();

      shader_program.SetUniform("model", model);
      shader_program.SetUniform("view", view);
      shader_program.SetUniform("projection", projection);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      floor_tex.Bind(0);
      // The floor will be a cube of thickness 0.01
      model = glm::translate(model, floor_pos) * glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));
      shader_program.SetUniform("model", model);
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

      glfwSetKeyCallback(window, GLFWOnKey);
      glfwSetCursorPosCallback(window, GLFWOnMouseMove);
      glfwSetScrollCallback(window, GLFWOnMouseScroll);

      // Hides and grabs cursor, unlimited movement
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPos(window, gWindowWidth / 2.0, gWindowHeight / 2.0);

      // Allow modern extension features
      glewExperimental = GL_TRUE;

      if (glewInit() == GLEW_OK) {
        // Setup Viewport size
        glViewport(0, 0, buffer_width, buffer_height);

        glViewport(0, 0, gWindowWidth, gWindowHeight);
        glEnable(GL_DEPTH_TEST);
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

void GLFWOnKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void GLFWOnFrameBufferSize(GLFWwindow* window, int width, int height) {
  gWindowWidth = width;
  gWindowHeight = height;
  glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void GLFWOnMouseMove(GLFWwindow* window, double pos_x, double pos_y) {
  // static glm::vec2 last_mouse_pos{0, 0};
  //
  // // Update angles based on Left Mouse Button input to orbit around the cube.
  // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
  //   // Each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
  //   gYaw -= ((float)pos_x - last_mouse_pos.x) * kMouseSensitivity;
  //   gPitch += ((float)pos_y - last_mouse_pos.y) * kMouseSensitivity;
  // }
  //
  // // Change orbit camera radius with the Right Mouse Button
  // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
  //   float dx = 0.01f *((float)pos_x - last_mouse_pos.x);
  //   float dy = 0.01f *((float)pos_y - last_mouse_pos.y);
  //   gRadius += dx - dy;
  // }
  //
  // last_mouse_pos.x = (float)pos_x;
  // last_mouse_pos.y = (float)pos_y;
}

void GLFWOnMouseScroll(GLFWwindow* window, double delta_x, double delta_y) {
  double fov = gFPSCamera.GetFOV() + delta_y * kZoomSensitivity;
  fov = glm::clamp(fov, 1.0, 120.0);
  gFPSCamera.SetFOV((float)fov);
}

void Update(GLFWwindow* window, double elapsed_time) {
  // Camera orientation
  double mouse_x, mouse_y;

  // Get the current mouse cursor position delta
  glfwGetCursorPos(window, &mouse_x, &mouse_y);

  // Rotate the camera the difference in mouse distance from the center screen.
  // Multiply this delta by a speed scaler.
  gFPSCamera.Rotate((float)(gWindowWidth / 2.0 - mouse_x) * kMouseSensitivity,
                    (float)(gWindowHeight / 2.0 - mouse_y) * kMouseSensitivity);

  // Set mouse cursor to center of screen
  glfwSetCursorPos(window, gWindowWidth / 2.0, gWindowHeight / 2.0);

  // Forward/Backward
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float)elapsed_time * gFPSCamera.GetLook());
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float)elapsed_time * -gFPSCamera.GetLook());
  }

  // Left/Right
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float)elapsed_time * -gFPSCamera.GetRight());
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float)elapsed_time * gFPSCamera.GetRight());
  }

  // Up/Down
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float)elapsed_time * gFPSCamera.GetUp());
  } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float)elapsed_time * -gFPSCamera.GetUp());
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
