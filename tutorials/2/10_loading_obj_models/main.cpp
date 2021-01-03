#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "mesh.h"
#include "shader_program.h"
#include "texture_2d.h"

// Window dimensions
static GLint gWindowWidth{1024};
static GLint gWindowHeight{768};
bool gWireframe{false};
static char const* const kAppTitle = "Loading OBJ Models";
int const kVerticesNumber{3};
int const kTexturesNumber{2};
auto const kNormalized{GL_FALSE};
int const kStride{(kVerticesNumber + kTexturesNumber) * sizeof(float)};
GLvoid const* const kOffset{nullptr};

FPSCamera gFPSCamera(glm::vec3(0.0f, 3.0f, 10.0f));
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
    ShaderProgram shader_program;
    shader_program.LoadShader("../basic.vert", "../basic.frag");

    // Model positions
    glm::vec3 model_pos[] = {
        glm::vec3(-2.5, 1, 0),  // crate
        glm::vec3(2.5, 1, 0),  // woodcrate
        glm::vec3(0, 0, -2),  // robot
        glm::vec3(0, 0, 0)  // floor
    };

    // Model scale
    glm::vec3 model_scale[] = {
        glm::vec3(1, 1, 1),  // crate
        glm::vec3(1, 1, 1),  // woodcrate
        glm::vec3(1, 1, 1),  // robot
        glm::vec3(10, 0, 10)  // floor
    };

    // Load meshes and textures
    int const num_models{4};
    Mesh mesh[num_models];
    Texture2D texture[num_models];

    mesh[0].LoadOBJ("../models/crate.obj");
    mesh[1].LoadOBJ("../models/woodcrate.obj");
    mesh[2].LoadOBJ("../models/robot.obj");
    mesh[3].LoadOBJ("../models/floor.obj");

    texture[0].Load("../textures/crate.jpg", true);
    texture[1].Load("../textures/woodcrate_diffuse.jpg", true);
    texture[2].Load("../textures/robot_diffuse.jpg", true);
    texture[3].Load("../textures/tile_floor.jpg", true);

    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
      ShowFPS(window);

      double delta_time = glfwGetTime() - last_time;

      // Poll for and process events
      glfwPollEvents();
      Update(window, delta_time);

      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glm::mat4 model{1.0};
      glm::mat4 view{1.0};
      glm::mat4 projection{1.0};

      view = gFPSCamera.GetViewMatrix();

      projection = glm::perspective(glm::radians(gFPSCamera.GetFOV()),
                                    (float) gWindowWidth / (float) gWindowHeight,
                                    0.1f, 100.0f);

      shader_program.Use();

      shader_program.SetUniform("view", view);
      shader_program.SetUniform("projection", projection);

      for (int i = 0; i < num_models; ++i) {
        model = glm::translate(glm::mat4{1}, model_pos[i]) * glm::scale(glm::mat4{1}, model_scale[i]);
        shader_program.SetUniform("model", model);

        texture[i].Bind(0);
        mesh[i].Draw();
        texture[i].Unbind(0);
      }

      glfwSwapBuffers(window);

      last_time += delta_time;
    }
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
  if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
  {
    gWireframe = !gWireframe;
    if (gWireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
  gFPSCamera.SetFOV((float) fov);
}

void Update(GLFWwindow* window, double elapsed_time) {
  // Camera orientation
  double mouse_x, mouse_y;

  // Get the current mouse cursor position delta
  glfwGetCursorPos(window, &mouse_x, &mouse_y);

  // Rotate the camera the difference in mouse distance from the center screen.
  // Multiply this delta by a speed scaler.
  gFPSCamera.Rotate((float) (gWindowWidth / 2.0 - mouse_x) * kMouseSensitivity,
                    (float) (gWindowHeight / 2.0 - mouse_y) * kMouseSensitivity);

  // Set mouse cursor to center of screen
  glfwSetCursorPos(window, gWindowWidth / 2.0, gWindowHeight / 2.0);

  // Forward/Backward
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float) elapsed_time * gFPSCamera.GetLook());
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float) elapsed_time * -gFPSCamera.GetLook());
  }

  // Left/Right
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float) elapsed_time * -gFPSCamera.GetRight());
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float) elapsed_time * gFPSCamera.GetRight());
  }

  // Up/Down
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float) elapsed_time * gFPSCamera.GetUp());
  } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    gFPSCamera.Move(kMoveSpeed * (float) elapsed_time * -gFPSCamera.GetUp());
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

