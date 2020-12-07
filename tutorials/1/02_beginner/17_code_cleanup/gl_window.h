//
// Created by pad on 12/7/20.
//
#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow {
 public:
  GLWindow();
  GLWindow(GLint width, GLint height, std::string title="", bool full_screen = false);
  virtual ~GLWindow();

  int Init();

  GLfloat GetBufferWidth() const { return buffer_width_; }

  GLfloat GetBufferHeight() const { return buffer_height_; }

  bool GetShouldClose() { return glfwWindowShouldClose(window_); }

  void SwapBuffers() { glfwSwapBuffers(window_); }

  static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);

  void ShowFPS();

 private:
  GLFWwindow* window_{nullptr};
  GLint width_{0};
  GLint height_{0};
  GLint buffer_width_{800};
  GLint buffer_height_{600};
  std::string title_{};
  bool full_screen_{false};
};
