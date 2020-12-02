#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
static GLint const kWidth{800};
static GLint const kHeight{600};
static char const* const kAppTitle = "Triangle";
static int const kColorMin{0};
static int const kColorMax{255};

const GLchar* vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 pos;"
    "layout (location = 1) in vec3 color;"
    "out vec3 vert_color;"
    "void main() {"
    "  vert_color = color;"
    "  gl_Position = vec4(pos, 1.0f);"
    "}";

const GLchar* fragment_shader_src =
    "#version 330 core\n"
    "in vec3 vert_color;"
    "out vec4 frag_color;"
    "void main() {"
    "  frag_color = vec4(vert_color, 1.0f);"
    "}";

GLFWwindow* InitOpenGL();
void CleanOpenGL(std::string const& msg);
void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
float NormalizeColor(int value);

int main() {
  int ret{EXIT_SUCCESS};
  auto window = InitOpenGL();
  if (window) {
    int const kVerticesNumber{3};
    auto normalized{GL_FALSE};
    auto stride{sizeof(GLfloat) * 6};
    GLvoid* position_offset{nullptr};
    auto color_offset{sizeof(GLfloat) * 3};
    // Vertex Buffer Object Interleaved Layout
    // clang-format off
    GLfloat vertices[] = {
         0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  // Top, Red
         0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,  // Right, Green
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // Left, Blue
    };
    // clang-format on

    GLuint vbo;  // vertex buffer object
    // Generate 1 buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // STATIC - The data store contents will be modified once and used many times.
    // DRAW - The data store contents are modified by the application, and used as the source for GL drawing and image
    // specification commands.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vao;  // vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // position
    glVertexAttribPointer(0, kVerticesNumber, GL_FLOAT, normalized, stride, position_offset);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, kVerticesNumber, GL_FLOAT, normalized, stride, (GLvoid *)(color_offset));
    glEnableVertexAttribArray(1);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader_src, nullptr);
    glCompileShader(vs);
    GLint result;
    GLchar info_log[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
      glGetShaderInfoLog(vs, sizeof(info_log), nullptr, info_log);
      std::cerr << "Error: Vertex shader failed to compile: " << info_log << std::endl;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader_src, nullptr);
    glCompileShader(fs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
      glGetShaderInfoLog(vs, sizeof(info_log), nullptr, info_log);
      std::cerr << "Error: Fragment shader failed to compile: " << info_log << std::endl;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
    if (!result) {
      glGetProgramInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
      std::cerr << "Error: Shader Program linker failure: " << info_log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Loop until window closed
    while (!glfwWindowShouldClose(window)) {
      ShowFPS(window);

      // Get + Handle user input events
      glfwPollEvents();

      // Set color (R, G, B, alpha)
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shader_program);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      glfwSwapBuffers(window);
    }
    glDeleteProgram(shader_program);
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

        glClearColor(0, 0, 1, 1.0);
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