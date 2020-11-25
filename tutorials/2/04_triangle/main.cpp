#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
static GLint const kWidth{800};
static GLint const kHeight{600};
static char const* const kAppTitle = "TEO";
static int const kColorMin{0};
static int const kColorMax{255};

bool g_full_screen{false};
GLFWwindow *g_window{nullptr};

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
float NormalizeColor(int value);
bool InitOpenGL();

int main() {
  if (!InitOpenGL()) {
    std::cerr << "GLFW initialization failed!\n";
    return EXIT_FAILURE;
  }

  // Loop until window closed
  while (!glfwWindowShouldClose(g_window)) {
    ShowFPS(g_window);

    static int iter{0};
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    // Set color (R, G, B, alpha)
    glClearColor(0, 0, NormalizeColor(iter % (kColorMax + 1)), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(g_window);

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

  if (g_full_screen) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWvidmode const* vidmode = glfwGetVideoMode(monitor);
    g_window = glfwCreateWindow(vidmode->width, vidmode->height, kAppTitle, monitor, nullptr);
  } else {
    g_window = glfwCreateWindow(kWidth, kHeight, kAppTitle, nullptr, nullptr);
  }

  if (!g_window) {
    std::cerr << "GLFW window creation failed!\n";
    glfwTerminate();
    return false;
  }

  // Get buffer size information
  int buffer_width;
  int buffer_height;
  glfwGetFramebufferSize(g_window, &buffer_width, &buffer_height);

  // Set context for GLEW to use
  glfwMakeContextCurrent(g_window);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialization failed!\n";
    glfwDestroyWindow(g_window);
    glfwTerminate();
    return false;
  }

  // Setup Viewport size
  glViewport(0, 0, buffer_width, buffer_height);

  glfwSetKeyCallback(g_window, OnKey);

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