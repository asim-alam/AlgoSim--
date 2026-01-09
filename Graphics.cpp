#include "Graphics.h"
#include <stdlib.h>
#include <math.h>


int Graphics::width = 0;
int Graphics::height = 0;
bool Graphics::keys[256] = {0};
bool Graphics::mouseButtons[3] = {0};
int Graphics::mouseX = 0;
int Graphics::mouseY = 0;
Graphics::ResizeCallback resizeCallback = nullptr;

void Graphics::SetResizeCallback(ResizeCallback callback) {
    resizeCallback = callback;
}


#ifdef _WIN32
#include <windows.h>

static HWND hwnd = nullptr;
static HDC hdc = nullptr;
static uint32* buffer = nullptr;
static BITMAPINFO bitmapInfo = {};

void Graphics::Resize(int w, int h) {
    if (w <= 0 || h <= 0) return;
    if (width == w && height == h && buffer != nullptr) return;

    if (buffer) delete[] buffer;
    buffer = new uint32[w * h];
    width = w;
    height = h;
    
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = w;
    bitmapInfo.bmiHeader.biHeight = -h;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    if (resizeCallback) resizeCallback(w, h);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE: PostQuitMessage(0); return 0;
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            Graphics::Resize(rect.right - rect.left, rect.bottom - rect.top);
            return 0;
        }
        case WM_KEYDOWN: Graphics::OnKeyDown((int)wParam); return 0;
        case WM_KEYUP: Graphics::OnKeyUp((int)wParam); return 0;
        case WM_LBUTTONDOWN: Graphics::OnMouseDown(0); return 0;
        case WM_LBUTTONUP: Graphics::OnMouseUp(0); return 0;
        case WM_RBUTTONDOWN: Graphics::OnMouseDown(1); return 0;
        case WM_RBUTTONUP: Graphics::OnMouseUp(1); return 0;
        case WM_MBUTTONDOWN: Graphics::OnMouseDown(2); return 0;
        case WM_MBUTTONUP: Graphics::OnMouseUp(2); return 0;
        case WM_MOUSEMOVE: Graphics::OnMouseMove(LOWORD(lParam), HIWORD(lParam)); return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Graphics::Init(int w, int h, const char* title) {
    width = w; height = h;
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "SPL1_Graphics";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClass(&wc);

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd = CreateWindowEx(0, "SPL1_Graphics", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    if (!hwnd) return false;
    hdc = GetDC(hwnd);
    
    // Initial buffer allocation
    Graphics::Resize(w, h);
    return true;
}

void Graphics::Shutdown() {
    if (buffer) delete[] buffer;
    ReleaseDC(hwnd, hdc);
}

bool Graphics::ProcessMessages() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Graphics::Present() {
    if (buffer)
        StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, buffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}



void Graphics::Clear(uint32 color) {
    for (int i = 0; i < width * height; i++) buffer[i] = color;
}

void Graphics::DrawPixel(int x, int y, uint32 color) {
    if (x >= 0 && x < width && y >= 0 && y < height) buffer[y * width + x] = color;
}

void Graphics::DrawRect(int x, int y, int w, int h, uint32 color) {
    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) DrawPixel(i, j, color);
    }
}

void Graphics::DrawLine(int x0, int y0, int x1, int y1, uint32 color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (true) {
        DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

bool Graphics::IsKeyDown(int key) { return keys[key]; }
bool Graphics::IsMouseButtonDown(int button) { if (button >= 0 && button < 3) return mouseButtons[button]; return false; }
#endif