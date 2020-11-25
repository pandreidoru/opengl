#include <iostream>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std::chrono_literals;

// Window dimensions
static GLint const kWidth{800};
static GLint const kHeight{600};
static char const *const kAppTitle = "TEO";
static int const kColorMin{0};
static int const kColorMax{255};

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

  GLFWwindow *main_window = glfwCreateWindow(kWidth, kHeight, kAppTitle, nullptr, nullptr);
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

  // Loop until window closed
  while (!glfwWindowShouldClose(main_window)) {
    static int iter{0};
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    // Set color (R, G, B, alpha)
    glClearColor(0, 0, NormalizeColor(iter % (kColorMax + 1)), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(main_window);
    std::this_thread::sleep_for(1ms);
    iter++;
  }

  glfwTerminate();
  return 0;
}
