// self
#include "MainWnd.h"

// project
#include "UI.h"
#include "Style.h"



MainWnd* MainWnd::s_Instance(nullptr);


MainWnd::MainWnd() :
    m_Resizing(false),
    m_ClientRect(ZERO_RECT),
    m_AnimationEnabled(false)
{
    s_Instance = this;
}


MainWnd::~MainWnd()
{
    s_Instance = nullptr;
}


MainWnd* MainWnd::Instance()
{
    return s_Instance;
}


LRESULT MainWnd::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(CreateGraphicsResources()))
        {
            return -1;  // Fail CreateWindowEx.
        }

        m_UI->Create();

        return 0;

    case WM_DESTROY:
        m_UI->StopAnimation(m_AnimationEnabled);
        Style::ReleaseResourceCache();
        DiscardGraphicsResources();
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaint(wParam, lParam);
        return 0;

    case WM_SIZE:
        Resize();
        return 0;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
        m_UI->OnMouseEvent(uMsg, wParam, lParam);
        return 0;
    }

    return DefWindowProc(m_NativeWnd, uMsg, wParam, lParam);
}


void MainWnd::EnableAnimation()
{
    m_AnimationEnabled = true;
}


void MainWnd::DisableAnimation()
{
    m_AnimationEnabled = false;
}


bool MainWnd::Resizing()
{
    return m_Resizing;
}


RECT MainWnd::Position()
{
    return m_ClientRect;
}


bool MainWnd::IsRoot(Control* Target)
{
    return m_UI->IsRoot(Target);
}


HRESULT MainWnd::CreateGraphicsResources()
{
    HRESULT hr = S_OK;

    if (m_D2DFactory == nullptr)
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2DFactory);
    }

    if (m_DWriteFactory == nullptr)
    {
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_DWriteFactory));
    }

    if (m_RenderTarget == nullptr)
    {
        GetClientRect(m_NativeWnd, &m_ClientRect);

        D2D1_SIZE_U size = D2D1::SizeU(m_ClientRect.right, m_ClientRect.bottom);

        hr = m_D2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_NativeWnd, size),
            &m_RenderTarget
        );

        m_RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        m_RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
    }

    return hr;
}


void MainWnd::DiscardGraphicsResources()
{
    XSafeRelease(&m_RenderTarget);
    XSafeRelease(&m_D2DFactory);
    XSafeRelease(&m_DWriteFactory);
}


void MainWnd::OnPaint(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_NativeWnd, &ps);

        m_RenderTarget->BeginDraw();

        if (wParam == ON_PAINT_WPARAM)
        {
            m_UI->OnPaint((Control*)lParam);  // paint hinted control
        }
        else if (wParam == ON_ANIMATED_WPARAM && !m_Resizing)
        {
            m_UI->ExecuteAnimation();  // animated all controls
        }
        else
        {
            m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            m_UI->OnPaint(nullptr, true);  // paint all controls
            m_UI->StartAnimation(m_AnimationEnabled, 30);  // start animation

            m_Resizing = false;
        }

        hr = m_RenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }

        EndPaint(m_NativeWnd, &ps);
    }
}


void MainWnd::Resize()
{
    m_Resizing = true;

    GetClientRect(m_NativeWnd, &m_ClientRect);

    if (m_RenderTarget != nullptr)
    {
        m_RenderTarget->Resize(D2D1::SizeU(m_ClientRect.right, m_ClientRect.bottom));

        m_UI->Resize();

        InvalidateRect(m_NativeWnd, NULL, FALSE);
    }
}
