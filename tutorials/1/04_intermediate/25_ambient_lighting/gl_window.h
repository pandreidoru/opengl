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
  GLWindow(GLint width, GLint height, std::string title = "", bool full_screen = false);
  virtual ~GLWindow();

  int Init();

  GLfloat GetBufferWidth() const { return buffer_width_; }

  GLfloat GetBufferHeight() const { return buffer_height_; }

  bool GetShouldClose() { return glfwWindowShouldClose(window_); }

  bool* GetKeys() { return keys_; }

  GLfloat GetXChange();
  GLfloat GetYChange();

  void SwapBuffers() { glfwSwapBuffers(window_); }

  void ShowFPS();

 private:
  void CreateCallbacks();
  static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
  static void HandleMouse(GLFWwindow* window, double x_pos, double y_pos);

  GLFWwindow* window_{nullptr};
  GLint width_{0};
  GLint height_{0};
  GLint buffer_width_{800};
  GLint buffer_height_{600};
  std::string title_{};
  bool full_screen_{false};

  bool keys_[1024]{false};

  GLfloat x_last_{0};
  GLfloat y_last_{0};
  GLfloat x_change_{0};
  GLfloat y_change_{0};
  bool mouse_first_move_{true};
};
