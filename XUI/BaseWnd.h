#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Common.h"
#include "UI.h"

// windows
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

// link
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "WindowsCodecs")



template <class Class>
class BaseWnd
{
public:
    // Constructor
    BaseWnd() :
        m_UI(nullptr),
        m_NativeWnd(nullptr),
        m_DpiScaleX(1.0f),
        m_DpiScaleY(1.0f),
        m_D2DFactory(nullptr),
        m_DWriteFactory(nullptr),
        m_IWICFactory(nullptr),
        m_RenderTarget(nullptr)
    {
    }

    // Decontructor
    virtual ~BaseWnd()
    {
        XSafeDelete(m_UI);
    }

    // Get hWnd
    inline HWND NativeWnd() const
    {
        return m_NativeWnd;
    }

    // Get horizontal dpi scale ratio
    inline float DpiScaleX()
    {
        return m_DpiScaleX;
    }

    // Get vertical dpi scale ratio
    inline float DpiScaleY()
    {
        return m_DpiScaleY;
    }

    // Get ID2D1Factory
    inline ID2D1Factory* D2DFactory()
    {
        return m_D2DFactory;
    }

    // Get IDWriteFactory
    inline IDWriteFactory* DWriteFactory()
    {
        return m_DWriteFactory;
    }

    // Get IWICImagingFactory
    inline IWICImagingFactory* IWICFactory()
    {
        return m_IWICFactory;
    }

    // Get ID2D1HwndRenderTarget
    inline ID2D1HwndRenderTarget* RenderTarget()
    {
        return m_RenderTarget;
    }

    // Get UI
    inline UI* GetUI()
    {
        return m_UI;
    }

    // Setup ui
    void Setup(UI* Tree)
    {
        m_UI = Tree;

        // Get the dpi information.
        HDC screen = GetDC(0);
        m_DpiScaleX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
        m_DpiScaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
        ReleaseDC(0, screen);
    }

    // Create window
    BOOL Create(
        const wchar_t* WindowName, const wchar_t* ClassName, DWORD Style, DWORD ExStyle = NULL, int Left = CW_USEDEFAULT, int Top = CW_USEDEFAULT,
        int Width = CW_USEDEFAULT, int Height = CW_USEDEFAULT, HWND Parent = NULL, HMENU Menu = NULL, HCURSOR Cursor = NULL, HICON Icon = NULL
    )
    {

        HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        WNDCLASS wc = ZERO_RECT;

        HINSTANCE hInstance = GetModuleHandle(NULL);

        wc.lpfnWndProc = Class::WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = ClassName;
        wc.hCursor = Cursor == NULL ? LoadCursor(nullptr, IDC_ARROW) : Cursor;
        if (Icon != NULL)
        {
            wc.hIcon = Icon;
        }

        RegisterClass(&wc);

        m_NativeWnd = CreateWindowEx(
            ExStyle, ClassName, WindowName, Style, Left, Top,
            (int)(float(Width) / m_DpiScaleX),
            (int)(float(Height) / m_DpiScaleY),
            Parent, Menu, hInstance, this
        );

        return m_NativeWnd != NULL;
    }

    // Show window
    BOOL Show(int nCmdShow)
    {
        return ShowWindow(m_NativeWnd, nCmdShow);
    }

    // Run message loop
    void Loop()
    {
        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        CoUninitialize();
    }

    // Window proc
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Class* This = nullptr;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* createStruct = (CREATESTRUCT*)lParam;
            This = (Class*)createStruct->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)This);

            This->m_NativeWnd = hWnd;
        }
        else
        {
            This = (Class*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        }

        if (This)
        {
            return This->HandleMessage(hWnd, uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }


protected:
    // Window's message hanler
    virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    // UI controls
    UI* m_UI;

    // hWnd
    HWND m_NativeWnd;

    // How much to scale a design that assumes 96-DPI pixels.
    float m_DpiScaleX;
    float m_DpiScaleY;

    // D2D and DWrite
    ID2D1Factory* m_D2DFactory;
    IDWriteFactory* m_DWriteFactory;
    IWICImagingFactory* m_IWICFactory;
    ID2D1HwndRenderTarget* m_RenderTarget;
};