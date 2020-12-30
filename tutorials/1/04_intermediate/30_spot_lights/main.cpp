#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "common_values.h"
#include "directional_light.h"
#include "gl_window.h"
#include "material.h"
#include "mesh.h"
#include "point_light.h"
#include "shader.h"
#include "spot_light.h"
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
  auto plain_texture = Texture("../textures/plain.png");
  brick_texture.Load();
  dirt_texture.Load();
  plain_texture.Load();

  auto main_light = DirectionalLight(1, 1, 1,
                                     0.1, 0.1,
                                     0, 0, -1);

  PointLight point_lights[kMaxPointLights];
  point_lights[0] = PointLight(0, 0, 1,
                               0, 0.1,
                               0, 0, 0,
                               0.3, 0.2, 0.1);
  point_lights[1] = PointLight(0, 1, 0,
                               0, 0.1,
                               -4, 2, 0,
                               0.3, 0.1, 0.1);
  unsigned int point_light_count = 0;

  SpotLight spot_lights[kMaxSpotLights];
  spot_lights[0] = SpotLight(1, 1, 1,
                             0, 2,
                             0, 0, 0,
                             0, -1, 0,
                             1, 0, 0,
                             20);
  spot_lights[1] = SpotLight(1, 1, 1,
                             0, 1,
                             0, -1.5, 0,
                             -100, -1, 0,
                             1, 0, 0,
                             20);
  unsigned int spot_light_count = 2;

  auto shiny_material = Material(4, 256);
  auto dull_material = Material(0.3, 4);

  auto uniform_model = gShaderList[0]->GetModelLocation();
  auto uniform_view = gShaderList[0]->GetViewLocation();
  auto uniform_projection = gShaderList[0]->GetProjectionLocation();
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

    // Place the light a little lower than the camera to mimic a flash-light held in the hand and not in the eye
    glm::vec3 lower_light = camera.GetCameraPosition();
    lower_light.y -= 0.3;

    // Move the source light together with the camera
    spot_lights[0].SetFlash(lower_light, camera.GetCameraDirection());

    gShaderList[0]->SetDirectionalLight(&main_light);
    gShaderList[0]->SetPointLights(point_lights, point_light_count);
    gShaderList[0]->SetSpotLights(spot_lights, spot_light_count);

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

    model = glm::mat4{1.0f};
    model = glm::translate(model, glm::vec3(0, -2, 0));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    dirt_texture.Use();
    shiny_material.Use(uniform_specular_intensity, uniform_shininess);
    gMeshList[2]->RenderMesh();

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

  unsigned int indices[] = {
      0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      0, 1, 2
  };

  unsigned int floor_indices[] = {
      0, 2, 1,
      1, 2, 3
  };

  GLfloat floor_vertices[] = {
      -10.0f,  0.0f, -10.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f,
       10.0f,  0.0f, -10.0f, 10.0f,  0.0f, 0.0f, -1.0f, 0.0f,
      -10.0f,  0.0f,  10.0f,  0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
       10.0f,  0.0f,  10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
  };
  // clang-format on

  CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

  gMeshList.emplace_back(new Mesh);
  gMeshList.emplace_back(new Mesh);
  gMeshList.emplace_back(new Mesh);
  gMeshList[0]->CreateMesh(vertices, indices, 32, 12);
  gMeshList[1]->CreateMesh(vertices, indices, 32, 12);
  gMeshList[2]->CreateMesh(floor_vertices, floor_indices, 32, 6);
}
