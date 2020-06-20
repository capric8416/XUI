#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "BaseWnd.h"

// forward
class UI;
class Control;



class MainWnd : public BaseWnd<MainWnd>
{
public:
    // Constructor
    MainWnd();
    // Decontructor
    virtual ~MainWnd();

    static MainWnd* Instance();

    // Message handler
    LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Disable animation
    void EnableAnimation();
    // Enable animation
    void DisableAnimation();

    // Is resizeing
    bool Resizing();

    // Get client rect
    RECT Position();

    // Is root control
    bool IsRoot(Control* Target);

private:
    // Create graphics resources
    HRESULT CreateGraphicsResources();

    // Discard graphics resouces
    void DiscardGraphicsResources();

    // Handle paint event
    void OnPaint(WPARAM wParam, LPARAM lParam);

    // Handle resize event
    void Resize();

    // Resizing
    bool m_Resizing;

    // Position
    RECT m_ClientRect;

    // Animation
    bool m_AnimationEnabled;

    static MainWnd* s_Instance;
};
