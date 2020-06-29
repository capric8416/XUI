#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Control.h"


class Language;
class MainWnd;


class UI
{
public:
    UI(Language* Lang);
    virtual ~UI();

    virtual MainWnd* GetMainWnd();
    virtual void SetMainWnd(MainWnd* Wnd);

    virtual Control* Create() = 0;
    virtual void Resize();
    virtual void Prepared();

    virtual inline SHORT MouseScrollLines();

    virtual void StartAnimation(bool Enabled, UINT8 FPS);
    virtual void StopAnimation(bool Enabled);
    virtual void ExecuteAnimation();

    virtual void OnPaint(Control* Target, bool Refresh = false);

    virtual void OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    virtual void OnCharInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual int OnCharInput(std::wstring ID, std::wstring Text, wchar_t* Buffer);
    
    virtual void OnKeyInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual int OnKeyInput(std::wstring ID, int Key, wchar_t* Buffer);

    virtual bool IsRoot(Control* Target);

    virtual void OnLeftClick(Control* Target);
    virtual void OnLeftDoubleClick(Control* Target);

    virtual int OnLeftClick(std::wstring ID, wchar_t* Buffer);
    virtual int OnLeftDoubleClick(std::wstring ID, wchar_t* Buffer);

    virtual void OnRightClick(Control* Target);
    virtual void OnRightDoubleClick(Control* Target);

    virtual int OnRightClick(std::wstring ID, wchar_t* Buffer);
    virtual int OnRightDoubleClick(std::wstring ID, wchar_t* Buffer);

    virtual void OnGroupItemChanged(Control* Parent, Control* Last, Control* Current, LONG Value = 0);
    virtual void OnSliderValueChanged(Control* Parent, Control* Target, INT32 Last, INT32 Current);

    virtual int OnGroupItemChanged(std::wstring ID, LONG Pos, LONG Value, wchar_t* Buffer);
    virtual int OnSliderValueChanged(std::wstring ID, LONG Value, wchar_t* Buffer);

    virtual void OnMouseVerticalScroll(Control* Target, SHORT Delta);
    virtual void OnMouseHorizontalScroll(Control* Target, SHORT Delta);

    virtual int OnMouseVerticalScroll(std::wstring ID, SHORT Delta, wchar_t* Buffer);
    virtual int OnMouseHorizontalScroll(std::wstring ID, SHORT Delta, wchar_t* Buffer);

    virtual void OnFocus(Control* Target);

    virtual Language* GetLanguage();

protected:
    bool m_Resized;
    bool m_Resizing;
    bool m_Prepared;

    Control* m_Tree;

    MainWnd* m_MainWnd;

    Control* m_Focused;

    Language* m_Language;

    SHORT m_MouseScrollLines;
};