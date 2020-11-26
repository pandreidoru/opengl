#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
static int const kWidth{800};
static int const kHeight{600};
static char const* const kAppTitle = "TEO";
static int const kColorMin{0};
static int const kColorMax{255};

bool g_full_screen{false};

GLFWwindow* InitOpenGL();
void CleanOpenGL(std::string const& text);
void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void ShowFPS(GLFWwindow* window);
float NormalizeColor(int value);

int main() {
  int ret{EXIT_SUCCESS};
  auto window = InitOpenGL();
  if (window) {
    // Loop until window closed
    while (!glfwWindowShouldClose(window)) {
      static int iter{0};

      ShowFPS(window);

      // Get + Handle user input events
      glfwPollEvents();

      // Set color (R, G, B, alpha)
      glClearColor(0, 0, NormalizeColor(iter % (kColorMax + 1)), 1.0);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);

      iter++;
    }
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

    if (g_full_screen) {
      GLFWmonitor* monitor = glfwGetPrimaryMonitor();
      GLFWvidmode const* vidmode = glfwGetVideoMode(monitor);
      window = glfwCreateWindow(vidmode->width, vidmode->height, kAppTitle, monitor, nullptr);
    } else {
      window = glfwCreateWindow(kWidth, kHeight, kAppTitle, nullptr, nullptr);
    }

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