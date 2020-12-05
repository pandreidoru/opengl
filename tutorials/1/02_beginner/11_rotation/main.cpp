#include <cstring>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLint const kWidth{800};
static GLint const kHeight{600};
static auto const* const kAppTitle = "TEO";
static auto const kColorMin{0};
static auto const kColorMax{255};

bool gFullScreen{false};
GLFWwindow* gWindow{nullptr};

GLuint VAO, VBO, gShader;

// Each iteration of the main loop will the `gTriOffset` will increment by `kTriIncrement` and when it will reach
// `kTriMaxOffset`, it will toggle the `gDirection`.
GLuint gUniformModel;  // How much to move
auto kTriIncrement{0.01f};
auto gDirection{true};  // true - To the right; false - To the left
auto gTriOffset{0.0f};
auto kTriMaxOffset{0.7f};

auto kToRadians{3.14159265f / 180.f};

// Vertex Shader
static char const* vShader =
    "#version 330 \n"
    "layout (location = 0) in vec3 pos;"
    ""
    "uniform mat4 model;"
    ""
    "void main() {"
    "  gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0f);"
    "}";

static char const* fShader =
    "#version 330 \n"
    "out vec4 color;"
    ""
    "void main() {"
    "  color = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
    "}";

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
float NormalizeColor(int value);
bool InitOpenGL();
void CreateTriangle();
void AddShader(GLuint program, char const* code, GLenum type);
void CompileShaders();

int main() {
  if (!InitOpenGL()) {
    std::cerr << "GLFW initialization failed!\n";
    return EXIT_FAILURE;
  }

  CreateTriangle();
  CompileShaders();

  // Loop until window closed
  while (!glfwWindowShouldClose(gWindow)) {
    ShowFPS(gWindow);

    static int iter{0};
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

    // Clear window
    // Set color (R, G, B, alpha)
    glClearColor(0, 0, NormalizeColor(iter % (kColorMax + 1)), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gShader);

    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(gTriOffset, 0.0f, 0.0f));
    // Rotate by 10 degree
    model = glm::rotate(model, 10 * kToRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(gUniformModel, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(gWindow);

    iter++;
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

void CreateTriangle() {
  // clang-format off
  GLfloat vertices[] = {
      -1.0f, -1.0f,  0.0f,
       1.0f, -1.0f,  0.0f,
       0.0f,  1.0f,  0.0f
  };
  // clang-format on

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void AddShader(GLuint program, char const* shader_code, GLenum type) {
  GLuint shader = glCreateShader(type);
  GLchar const* code[1];
  code[0] = shader_code;

  GLint code_length[1];
  code_length[0] = strlen(shader_code);
  glShaderSource(shader, 1, code, code_length);

  GLint result = 0;
  GLchar result_log[1024] = {0};
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error compiling the " << type << " shader: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  glAttachShader(program, shader);
}

void CompileShaders() {
  gShader = glCreateProgram();
  if (!gShader) {
    std::cerr << "Error creating shader program\n";
    exit(EXIT_FAILURE);
  }

  AddShader(gShader, vShader, GL_VERTEX_SHADER);
  AddShader(gShader, fShader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar result_log[1024] = {0};
  glLinkProgram(gShader);
  glGetProgramiv(gShader, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(gShader, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error linking program: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  glValidateProgram(gShader);
  glGetProgramiv(gShader, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(gShader, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error validating program: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  gUniformModel = glGetUniformLocation(gShader, "model");
}