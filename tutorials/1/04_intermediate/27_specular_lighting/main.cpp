#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "gl_window.h"
#include "light.h"
#include "material.h"
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

  auto main_light = Light(1, 1, 1,
                          0.1,
                          2, -1, -2, 0.1);
  auto shiny_material = Material(1, 32);
  auto dull_material = Material(0.3, 4);

  auto uniform_model = gShaderList[0]->GetModelLocation();
  auto uniform_view = gShaderList[0]->GetViewLocation();
  auto uniform_projection = gShaderList[0]->GetProjectionLocation();
  auto uniform_ambient_colour = gShaderList[0]->GetAmbientColourLocation();
  auto uniform_ambient_intensity = gShaderList[0]->GetAmbientIntensityLocation();
  auto uniform_direction = gShaderList[0]->GetDirectionLocation();
  auto uniform_diffuse_intensity = gShaderList[0]->GetDiffuseIntensityLocation();
  auto uniform_eye_pos = gShaderList[0]->GetEyePosLocation();
  auto uniform_specular_intensity = gShaderList[0]->GetSpecularIntensityLocation();
  auto uniform_shininess = gShaderList[0]->GetShininessLocation();

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
    glUniform3f(uniform_eye_pos,
                camera.GetCameraPosition().x,
                camera.GetCameraPosition().y,
                camera.GetCameraPosition().z);

    main_light.Use(uniform_ambient_colour, uniform_ambient_intensity, uniform_direction, uniform_diffuse_intensity);

    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    brick_texture.Use();
    shiny_material.Use(uniform_specular_intensity, uniform_shininess);
    gMeshList[0]->RenderMesh();

    model = glm::mat4{1.0f};
    model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    dirt_texture.Use();
    dull_material.Use(uniform_specular_intensity, uniform_shininess);
    gMeshList[1]->RenderMesh();

    glUseProgram(0);

    main_window.SwapBuffers();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

void CalcAverageNormals(unsigned int const* indices, unsigned int indices_count,
                        GLfloat* vertices, unsigned int vertices_count, unsigned int vertex_length,
                        unsigned int normal_offset) {
  // Iterate through each triangle (row)
  for (auto i = 0; i < indices_count; i += 3) {
    // clang-format off
    unsigned int in0 = indices[i    ] * vertex_length;
    unsigned int in1 = indices[i + 1] * vertex_length;
    unsigned int in2 = indices[i + 2] * vertex_length;

    // Compute normals
    glm::vec3 v1{vertices[in1    ] - vertices[in0    ],   // X
                 vertices[in1 + 1] - vertices[in0 + 1],   // Y
                 vertices[in1 + 2] - vertices[in0 + 2]};  // Z
    glm::vec3 v2{vertices[in2    ] - vertices[in0    ],   // X
                 vertices[in2 + 1] - vertices[in0 + 1],   // Y
                 vertices[in2 + 2] - vertices[in0 + 2]};  // Z

    glm::vec3 normal = glm::cross(v1, v2);
    normal = glm::normalize(normal);

    // Update normals values inside indices for all the points of the current triangle
    in0 += normal_offset;
    in1 += normal_offset;
    in2 += normal_offset;
    vertices[in0] += normal.x;    vertices[in0 + 1] += normal.y;    vertices[in0 + 2] += normal.z;
    vertices[in1] += normal.x;    vertices[in1 + 1] += normal.y;    vertices[in1 + 2] += normal.z;
    vertices[in2] += normal.x;    vertices[in2 + 1] += normal.y;    vertices[in2 + 2] += normal.z;
    // clang-format on
  }

  // Normalize the normals
  for (auto i = 0; i < vertices_count / vertex_length; ++i) {
    unsigned int n_offset = i * vertex_length + normal_offset;
    glm::vec3 vec(vertices[n_offset], vertices[n_offset + 1], vertices[n_offset + 2]);
    vec = glm::normalize(vec);
    vertices[n_offset] = vec.x;
    vertices[n_offset + 1] = vec.y;
    vertices[n_offset + 2] = vec.z;
  }
}

void CreateShaders() {
  gShaderList.emplace_back(new Shader);
  gShaderList[0]->CreateFromFiles(vShader, fShader);
}

void CreateObjects() {
  // clang-format off
  GLfloat vertices[] = {
      //  x,     y,     z,     u,     v, normX, normY, normZ
      -1.0f, -1.0f, -0.6f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
       0.0f, -1.0f,  1.0f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,
       1.0f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
       0.0f,  1.0f,  0.0f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f
  };
  // clang-format on

  unsigned int indices[] = {
      0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      0, 1, 2
  };

  CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

  gMeshList.emplace_back(new Mesh);
  gMeshList.emplace_back(new Mesh);
  gMeshList[0]->CreateMesh(vertices, indices, 32, 12);
  gMeshList[1]->CreateMesh(vertices, indices, 32, 12);
}
