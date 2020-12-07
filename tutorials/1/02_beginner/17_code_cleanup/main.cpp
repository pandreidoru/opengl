#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "shader.h"
#include "gl_window.h"

static GLint const kWidth{800};
static GLint const kHeight{600};
static auto const* const kAppTitle = "TEO";

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

std::vector<Mesh*> gMeshList;
std::vector<Shader*> gShaderList;

void CreateShaders();
void CreateObjects();

int main() {
  GLWindow main_window{kWidth, kHeight, kAppTitle};
  main_window.Init();

  CreateObjects();
  CreateShaders();

  GLuint uniform_model{0};
  GLuint uniform_projection{0};

  glm::mat4 projection = glm::perspective(45.0f,
                                          main_window.GetBufferWidth() / main_window.GetBufferHeight(),
                                          0.1f,
                                          100.0f);

  // Loop until window closed
  while (!main_window.GetShouldClose()) {
    main_window.ShowFPS();

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

    main_window.SwapBuffers();
  }

  glfwTerminate();
  return 0;
}

void CreateShaders() {
  gShaderList.emplace_back(new Shader);
  gShaderList[0]->CreateFromFiles(vShader, fShader);
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
