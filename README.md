# Simple Service Locator
A simple C++ static Service Locator class.

## Purpose
Providing a global point of access to a service (Window Manager, Renderer) without coupling users to the concrete class that implements it.

## Example use case
### A simple window manager using GLFW or SDL

</br>

#### *Window Interface*
```cpp

// include/service/window_service.h

#ifndef _IWINDOW_H_
#define _IWINDOW_H_

// Window Interface
// Avoid defining virtual functions as = 0 in order to be used as the default NullService
class IWindow {
private:

protected:
    bool _opened = false;

public:
    inline bool isOpened() { return _opened; }
    virtual void Create(const int p_width = 720,
                        const int p_height = 480,
                        const char* p_title = "Untitled Window") { };
    virtual void Close() { };

    virtual void RequestRenderContext() { };

    virtual void SetTitle(const char* p_title = "Untitled Window") { };
    virtual void Resize(const int p_width, const int p_height) { };

    virtual void Update() { };
};

#endif // _IWINDOW_H_

```

</br>

### *GLFW Implementation*
```cpp

// src/service/window_glfw.h
#ifndef _WINDOW_GLFW_H_
#define _WINDOW_GLFW_H_

#include <service/window_service.h>
#include <GLFW/glfw3.h>

class Window_GLFW : public IWindow {
private:
    GLFWwindow* _window = nullptr;
public:
    Window_GLFW() = default;
    void Create(const int p_width, const int p_height, const char* p_title) override;
    void Close() override;
    
    void SetTitle(const char* p_title) override;
    void Resize(const int p_width, const int p_height) override;

    void Update() override;
};

#endif // _WINDOW_GLFW_H_

```

```cpp

// src/service/window_glfw.cpp
#include "window_glfw.h"

void Window_GLFW::Create(const int p_width, const int p_height, const char* p_title) {
    if(_window != nullptr) {
        return;
    }

    glfwInit();
    _window = glfwCreateWindow(p_width, p_height, p_title, nullptr, nullptr);
    _opened = true;
}

void Window_GLFW::Close() {
    if(_window == nullptr) {
        return;
    }
    
    glfwDestroyWindow(_window);
    _window = nullptr;
    _opened = false;
}

void Window_GLFW::SetTitle(const char* p_title) {
    if(_window == nullptr) {
        return;
    }

    glfwSetWindowTitle(_window, p_title);
}

void Window_GLFW::Resize(const int p_width, const int p_height) {
    if(_window == nullptr) {
        return;
    }

    glfwSetWindowSize(_window, p_width, p_height);
}

void Window_GLFW::Update() {
    if(_window == nullptr) {
        return;
    }

    glfwPollEvents();
    if(glfwWindowShouldClose(_window)) {
        Close();
    }
}

```

### *SDL Implementation*
```cpp

// src/service/window_sdl.h
#ifndef _WINDOW_SDL_H_
#define _WINDOW_SDL_H_

#include <service/window_service.h>
#include <SDL.h>

class Window_SDL : public IWindow {
private:
    SDL_Window* _window;
public:
    Window_SDL() = default;
    void Create(const int p_width, const int p_height, const char* p_title) override;
    void Close() override;
    
    void SetTitle(const char* p_title) override;
    void Resize(const int p_width, const int p_height) override;

    void Update() override;
};

#endif // _WINDOW_SDL_H_

```

```cpp

// src/service/window_sdl.cpp
#include "window_sdl.h"

void Window_SDL::Create(const int p_width, const int p_height, const char* p_title) {
    SDL_Init(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow(p_title,
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                p_width, p_height,
                                SDL_WINDOW_RESIZABLE);

    if(_window == nullptr) {
        return;
    }
    _opened = true;
}

void Window_SDL::SetTitle(const char* p_title) {
    if(_window == nullptr) {
        return;
    }

    SDL_SetWindowTitle(_window, p_title);
}

void Window_SDL::Resize(const int p_width, const int p_height) {
    if(_window == nullptr) {
        return;
    }

    SDL_SetWindowSize(_window, p_width, p_height);
    SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Window_SDL::Close() {
    if(_window == nullptr) {
        return;
    }

    SDL_DestroyWindow(_window);
    _window = nullptr;
    _opened = false;
}

void Window_SDL::Update() {
    if(_window == nullptr) {
        return;
    }

    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        Close();
    }
}

```

## Providing access

```cpp

// Use GLFW
ServiceLocator::Provide<IWindow>(new Window_GLFW());

// Or use SDL instead
// ServiceLocator::Provide<IWindow>(new Window_SDL());

```

## Resolving access

```cpp

// Resizing the window
ServiceLocator::Resolve<IWindow>()->Resize(1600, 900);

// If no implementation for IWindow was provided the NullService will be returned

```