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

bool full_screen{false};

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
float NormalizeColor(int value);

int main() {
  if (!glfwInit()) {
    std::cerr << "GLFW initialization failed!\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // No Backwards Compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* main_window = nullptr;
  if (full_screen) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWvidmode const* vidmode = glfwGetVideoMode(monitor);
    main_window = glfwCreateWindow(vidmode->width, vidmode->height, kAppTitle, monitor, nullptr);
  } else {
    main_window = glfwCreateWindow(kWidth, kHeight, kAppTitle, nullptr, nullptr);
  }

  if (!main_window) {
    std::cerr << "GLFW window creation failed!\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Get buffer size information
  int buffer_width;
  int buffer_height;
  glfwGetFramebufferSize(main_window, &buffer_width, &buffer_height);

  // Set context for GLEW to use
  glfwMakeContextCurrent(main_window);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialization failed!\n";
    glfwDestroyWindow(main_window);
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Setup Viewport size
  glViewport(0, 0, buffer_width, buffer_height);

  glfwSetKeyCallback(main_window, OnKey);

  // Loop until window closed
  while (!glfwWindowShouldClose(main_window)) {
    ShowFPS(main_window);

    static int iter{0};
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    // Set color (R, G, B, alpha)
    glClearColor(0, 0, NormalizeColor(iter % (kColorMax + 1)), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(main_window);

    iter++;
  }

  glfwTerminate();
  return 0;
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