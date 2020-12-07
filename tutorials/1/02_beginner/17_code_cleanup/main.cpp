#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "shader.h"

static GLint const kWidth{800};
static GLint const kHeight{600};
static auto const* const kAppTitle = "TEO";
static auto const kColorMin{0};
static auto const kColorMax{255};

bool gFullScreen{false};
GLFWwindow* gWindow{nullptr};

std::vector<Mesh*> gMeshList;
std::vector<Shader*> gShaderList;

// Each iteration of the main loop will the `gTriOffset` will increment by `kTriIncrement` and when it will reach
// `kTriMaxOffset`, it will toggle the `gDirection`.
GLuint gUniformModel;  // How much to move
auto kTriIncrement{0.01f};
auto gDirection{true};  // true - To the right; false - To the left
auto gTriOffset{0.0f};
auto kTriMaxOffset{0.7f};

auto gSizeDirection{true};
auto gCurSize{0.4f};
auto kMaxSize{0.8f};
auto kMinSize{0.1f};

auto gCurAngle{0.0f};

auto kToRadians{3.14159265f / 180.f};

static char const* vShader = "../shaders/shader.vert";
static char const* fShader = "../shaders/shader.frag";

void CreateShaders();
void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
float NormalizeColor(int value);
bool InitOpenGL();
void CreateObjects();

void CreateShaders() {
  gShaderList.emplace_back(new Shader);
  gShaderList[0]->CreateFromFiles(vShader, fShader);
}

int main() {
  if (!InitOpenGL()) {
    std::cerr << "GLFW initialization failed!\n";
    return EXIT_FAILURE;
  }

  CreateObjects();
  CreateShaders();

  GLuint uniform_model{0};
  GLuint uniform_projection{0};

  glm::mat4 projection = glm::perspective(45.0f,
                                          static_cast<GLfloat>(kWidth) / static_cast<GLfloat>(kHeight),
                                          0.1f,
                                          100.0f);

  // Loop until window closed
  while (!glfwWindowShouldClose(gWindow)) {
    ShowFPS(gWindow);

    // Get + Handle user input events
    glfwPollEvents();

    if (gDirection) {
      gTriOffset += kTriIncrement;
    } else {
      gTriOffset -= kTriIncrement;
    }

    if (std::abs(gTriOffset) >= kTriMaxOffset) {
      gDirection = !gDirection;
    }

    gCurAngle += 0.5f;

    // Reset angle
    if (gCurAngle >= 360) {
      gCurAngle -= 360;
    }

    if (gSizeDirection) {
      gCurSize += 0.007f;
    } else {
      gCurSize -= 0.007f;
    }

    if (gCurSize >= kMaxSize || gCurSize < kMinSize) {
      gSizeDirection = !gSizeDirection;
    }

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gShaderList[0]->Use();
    uniform_model = gShaderList[0]->GetModelLocation();
    uniform_projection = gShaderList[0]->GetProjectionLocation();

    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(gTriOffset, 0.0f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    gMeshList[0]->RenderMesh();

    model = glm::mat4{1.0f};
    model = glm::translate(model, glm::vec3(-gTriOffset, 1.0f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(gUniformModel, 1, GL_FALSE, glm::value_ptr(model));
    gMeshList[1]->RenderMesh();

    glUseProgram(0);

    glfwSwapBuffers(gWindow);
  }

  glfwTerminate();
  return 0;
}

bool InitOpenGL() {
  if (!glfwInit()) {
    std::cerr << "GLFW initialization failed!\n";
    glfwTerminate();
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // No Backwards Compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  if (gFullScreen) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWvidmode const* vidmode = glfwGetVideoMode(monitor);
    gWindow = glfwCreateWindow(vidmode->width, vidmode->height, kAppTitle, monitor, nullptr);
  } else {
    gWindow = glfwCreateWindow(kWidth, kHeight, kAppTitle, nullptr, nullptr);
  }

  if (!gWindow) {
    std::cerr << "GLFW window creation failed!\n";
    glfwTerminate();
    return false;
  }

  // Get buffer size information
  int buffer_width;
  int buffer_height;
  glfwGetFramebufferSize(gWindow, &buffer_width, &buffer_height);

  // Set context for GLEW to use
  glfwMakeContextCurrent(gWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialization failed!\n";
    glfwDestroyWindow(gWindow);
    glfwTerminate();
    return false;
  }

  glEnable(GL_DEPTH_TEST);

  // Setup Viewport size
  glViewport(0, 0, buffer_width, buffer_height);

  glfwSetKeyCallback(gWindow, OnKey);

  return true;
}

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

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
  float ret;
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

void CreateObjects() {
  unsigned int indices[] = {
      0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      0, 1, 2
  };

  // clang-format off
  GLfloat vertices[] = {
      -1.0f, -1.0f,  0.0f,
       0.0f, -1.0f,  1.0f,  // The 4th point of the pyramid - on Z axis
       1.0f, -1.0f,  0.0f,
       0.0f,  1.0f,  0.0f
  };
  // clang-format on

  // Mesh* mesh1 = new Mesh();
  // mesh1->CreateMesh(vertices, indices, 12, 12);
  gMeshList.emplace_back(new Mesh);
  gMeshList.emplace_back(new Mesh);
  gMeshList[0]->CreateMesh(vertices, indices, 12, 12);
  gMeshList[1]->CreateMesh(vertices, indices, 12, 12);
}
