#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "gl_window.h"
#include "light.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

static GLint const kWidth{800};
static GLint const kHeight{600};
static auto const* const kAppTitle = "Title";

auto kToRadians{3.14159265f / 180.f};

static char const* vShader = "../shaders/shader.vert";
static char const* fShader = "../shaders/shader.frag";

std::vector<Mesh*> gMeshList;
std::vector<Shader*> gShaderList;

GLfloat gDeltaTime{0};
GLfloat gLastTime{0};

void CreateShaders();
void CreateObjects();

int main() {
  GLWindow main_window{kWidth, kHeight, kAppTitle};
  main_window.Init();

  CreateObjects();
  CreateShaders();

  Camera camera{glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                -90.0f, 0.0f, 5.0f, 0.1f};

  auto brick_texture = Texture("../textures/brick.png");
  auto dirt_texture = Texture("../textures/dirt.png");
  brick_texture.Load();
  dirt_texture.Load();

  auto main_light = Light(1, 1, 1, 0.2);

  auto uniform_model = gShaderList[0]->GetModelLocation();
  auto uniform_view = gShaderList[0]->GetViewLocation();
  auto uniform_projection = gShaderList[0]->GetProjectionLocation();
  auto uniform_ambient_colour = gShaderList[0]->GetAmbientColourLocation();
  auto uniform_ambient_intensity = gShaderList[0]->GetAmbientIntensityLocation();

  auto projection = glm::perspective(45.0f,
                                     main_window.GetBufferWidth() / main_window.GetBufferHeight(),
                                     0.1f,
                                     100.0f);

  while (!main_window.GetShouldClose()) {
    GLfloat now = glfwGetTime();
    gDeltaTime = now - gLastTime;
    gLastTime = now;

    main_window.ShowFPS();

    // Get + Handle user input events
    glfwPollEvents();

    camera.KeyControl(main_window.GetKeys(), gDeltaTime);
    camera.MouseControl(main_window.GetXChange(), main_window.GetYChange());

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gShaderList[0]->Use();
    glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));

    main_light.Use(uniform_ambient_colour, uniform_ambient_intensity);

    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    brick_texture.Use();
    gMeshList[0]->RenderMesh();

    model = glm::mat4{1.0f};
    model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    dirt_texture.Use();
    gMeshList[1]->RenderMesh();

    glUseProgram(0);

    main_window.SwapBuffers();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

void CreateShaders() {
  gShaderList.emplace_back(new Shader);
  gShaderList[0]->CreateFromFiles(vShader, fShader);
}

void CreateObjects() {
  // clang-format off
  GLfloat vertices[] = {
      //  x,     y,     z,     u,     v
      -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
       0.0f, -1.0f,  1.0f,  0.5f,  0.0f,
       1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
       0.0f,  1.0f,  0.0f,  0.5f,  1.0f
  };
  // clang-format on

  unsigned int indices[] = {
      0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      0, 1, 2
  };

  gMeshList.emplace_back(new Mesh);
  gMeshList.emplace_back(new Mesh);
  gMeshList[0]->CreateMesh(vertices, indices, 20, 12);
  gMeshList[1]->CreateMesh(vertices, indices, 20, 12);
}
