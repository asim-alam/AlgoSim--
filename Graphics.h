#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Core.h"

class Graphics
{
private:
    static int width;
    static int height;
    static bool keys[256];
    static bool mouseButtons[3];
    static int mouseX;
    static int mouseY;

public:
    typedef void (*ResizeCallback)(int width, int height);
    static void SetResizeCallback(ResizeCallback callback);

    static bool Init(int w, int h, const char *title);
    static void Shutdown();
    static bool ProcessMessages();
    static void Resize(int w, int h);

    static void Clear(int color);
    static void DrawPixel(int x, int y, int color);
    static void DrawRect(int x, int y, int w, int h, int color);
    static void DrawLine(int x0, int y0, int x1, int y1, int color);
    static void Present();

    static int GetWidth() { return width; }
    static int GetHeight() { return height; }

    static bool IsKeyDown(int key);
    static bool IsMouseButtonDown(int button);
    static int GetMouseX() { return mouseX; }
    static int GetMouseY() { return mouseY; }

    static void OnKeyDown(int key)
    {
        if (key >= 0 && key < 256)
            keys[key] = true;
    }
    static void OnKeyUp(int key)
    {
        if (key >= 0 && key < 256)
            keys[key] = false;
    }
    static void OnMouseDown(int button)
    {
        if (button >= 0 && button < 3)
            mouseButtons[button] = true;
    }
    static void OnMouseUp(int button)
    {
        if (button >= 0 && button < 3)
            mouseButtons[button] = false;
    }
    static void OnMouseMove(int x, int y)
    {
        mouseX = x;
        mouseY = y;
    }
};

#endif
