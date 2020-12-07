//
// Created by pad on 12/7/20.
//

#include "gl_window.h"

#include <sstream>

GLWindow::GLWindow() = default;

GLWindow::~GLWindow() {
  glfwDestroyWindow(window_);
  glfwTerminate();
}

GLWindow::GLWindow(GLint width, GLint height, std::string title, bool full_screen) :
    width_{width},
    height_{height},
    title_(std::move(title)),
    full_screen_{full_screen} {}

int GLWindow::Init() {
  if (!glfwInit()) {
    std::cerr << "GLFW initialization failed!\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  if (full_screen_) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWvidmode const* vidmode = glfwGetVideoMode(monitor);
    window_ = glfwCreateWindow(vidmode->width, vidmode->height, title_.c_str(), monitor, nullptr);
  } else {
    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
  }

  if (!window_) {
    std::cerr << "GLFW window creation failed!\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwGetFramebufferSize(window_, &buffer_width_, &buffer_height_);

  // Set context for GLEW to use
  glfwMakeContextCurrent(window_);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialization failed!\n";
    glfwDestroyWindow(window_);
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glEnable(GL_DEPTH_TEST);

  // Setup Viewport size
  glViewport(0, 0, buffer_width_, buffer_height_);

  glfwSetKeyCallback(window_, GLWindow::OnKey);

  return EXIT_SUCCESS;
}

void GLWindow::OnKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void GLWindow::ShowFPS() {
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
        << title_ << " "
        << "FPS: " << fps << " "
        << " Frame Time: " << ms_per_frame << " (ms)";
    glfwSetWindowTitle(window_, out.str().c_str());

    frame_count = 0;
  }

  frame_count++;
}
