#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#ifdef __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#include <AppKit/AppKit.hpp>
#include <GLFW/glfw3native.h>
#endif

class Window
{
  GLFWwindow* window;

  uint32_t Width = 800;
  uint32_t Height = 600;

public:
  Window(uint32_t width, uint32_t height);
  ~Window();

  bool ShouldClose() const;
  void Update();

  GLFWwindow *GetWindow() { return window; }
  void GetSize(int &width, int &height) { glfwGetFramebufferSize(window, &width, &height); }


#ifdef __APPLE__
  NS::Window *GetCocoaWindow() { return reinterpret_cast<NS::Window *>(glfwGetCocoaWindow(window)); }
#endif
};



#endif //WINDOW_H
