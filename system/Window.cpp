#include "Window.h"

#include "Events.h"
#include "Log.h"

namespace
{
  void ResizeCallback(GLFWwindow *window, int width, int height)
  {
    Events::Event e{};
    e.action = Events::Action::RESIZE;
    Events::AddEvent(e);
  }

  void MouseButtonCallback(GLFWwindow *window, const int button, const int action, int mods)
  {
    Events::Event e{};
    e.action = Events::Action::MOUSEBUTTON;
    e.press = action == GLFW_PRESS;
    e.key = button;

    glfwGetCursorPos(window, &e.x, &e.y);

    Events::AddEvent(e);
  }

  void MouseMoveCallback(GLFWwindow *window, const double xpos, const double ypos)
  {
    Events::Event e{};
    e.action = Events::Action::MOUSEMOVE;
    e.x = xpos;
    e.y = ypos;
    Events::AddEvent(e);
  }

  void KeyCallback(GLFWwindow * window, const int key, int scancode, const int action, int mods)
  {
    if(action != GLFW_PRESS && action != GLFW_RELEASE)
      return;
    Events::Event e{};
    e.action = Events::Action::KEY;
    e.press  = action == GLFW_PRESS;
    e.key    = key;

    glfwGetCursorPos(window, &e.x, &e.y);

    Events::AddEvent(e);
  }
} // namespace


Window::Window(const uint32_t width, const uint32_t height)
: Width(width), Height(height)
{
  if (glfwInit() != GLFW_TRUE)
  {
    Log::ExitError("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(Width, Height, "Graphics App", nullptr, nullptr);

  glfwSetWindowSizeCallback(window, ResizeCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glfwSetCursorPosCallback(window, MouseMoveCallback);
  glfwSetKeyCallback(window, KeyCallback);
}



Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}




bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}



void Window::Update()
{
    glfwSwapInterval(0);
    glfwPollEvents();
}


