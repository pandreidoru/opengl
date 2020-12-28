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

  // Handle Keys + Mouse Input
  CreateCallbacks();
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  glfwSetWindowUserPointer(window_, this);

  return EXIT_SUCCESS;
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

void GLWindow::CreateCallbacks() {
  glfwSetKeyCallback(window_, HandleKeys);
  glfwSetCursorPosCallback(window_, HandleMouse);
}

GLfloat GLWindow::GetXChange() {
  GLfloat the_change = x_change_;
  x_change_ = 0;
  return the_change;
}

GLfloat GLWindow::GetYChange() {
  GLfloat the_change = y_change_;
  y_change_ = 0;
  return the_change;
}

void GLWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
  auto* the_window = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      the_window->keys_[key] = true;
    } else if (action == GLFW_RELEASE) {
      the_window->keys_[key] = false;
    }
  }
}

void GLWindow::HandleMouse(GLFWwindow* window, double x_pos, double y_pos) {
  auto* the_window = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

  if (the_window->mouse_first_move_) {
    the_window->x_last_ = x_pos;
    the_window->y_last_ = y_pos;
    the_window->mouse_first_move_ = false;
  }

  the_window->x_change_ = x_pos - the_window->x_last_;
  the_window->y_change_ = the_window->y_last_ - y_pos;  // Avoid inverted up/down controls

  the_window->x_last_ = x_pos;
  the_window->y_last_ = y_pos;
}
