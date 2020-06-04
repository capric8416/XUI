#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Common.h"

// windows
#include <d2d1.h>

// c++
#include <initializer_list>
#include <list>
#include <string>
#include <unordered_map>

// forward
class BaseControl;
class MainWnd;
class Animation;
class Background;
class Border;
class Image;
class Text;



// Base control
class Control
{
public:
    // Constructor
    Control(
        std::wstring ID,
        RECT Position,
        std::initializer_list<Control*> Children,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {},
        std::initializer_list<Image*> ImageStyle = {}
    );
    Control(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        bool Hidden = false,
        bool Disabled = false,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        bool Hintable = true,
        std::initializer_list<Control*> Children = {},
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {},
        std::initializer_list<Image*> ImageStyle = {}
    );
    // Deconstructor
    ~Control();

    // Set parent
    virtual Control* Parent();
    virtual void SetParent(Control* Parent);

    // Get children
    virtual std::vector<Control*>* Children();

    // Attach child control
    virtual void Attach(Control* Control);
    virtual void Attach(std::initializer_list<Control*> Children);

    virtual void Style(CONTROL_STATUS Status);
    virtual void Style(uint8_t Index, Background* BackgroundStyle = nullptr, Border* BorderStyle = nullptr, Text* TextStyle = nullptr, Animation* AnimationStyle = nullptr);

    std::wstring GetContent();
    bool SetContent(std::wstring Content, bool Paint = true);

    bool SetImage(std::wstring Path, bool Paint = true);

    // Is hidden
    virtual inline bool Hidden();
    // Hide it
    void Hide();
    // Show it
    void Show();
    // Set visibility
    void SetVisibility(bool Value);
    // Flip visibility
    void FlipVisibility();


    // Is disabled
    virtual inline bool Disabled();
    // Disable it
    void Disable();
    // Enable it
    void Enable();
    // Set usability
    void SetUsability(bool Value);
    // Flip usability
    void FlipUsability();

    // Can capture event
    virtual inline bool Hintable();

    // Can be vertical scroll
    virtual inline bool VerticalScrollEnabled();
    // Can be horizontal scroll
    virtual inline bool HorizontalScrollEnabled();

    // Get id
    virtual inline std::wstring ID();

    // Get main wnd
    virtual inline MainWnd* Wnd();

    virtual void Resize();

    // Get position
    virtual inline RECT Position();
    virtual inline D2D_RECT_F DPIPosition();
    virtual inline LONG WidthPercentage();
    virtual inline LONG HeightPercentage();
    // Set position
    virtual void SetPosition(RECT Pos);
    virtual void SetPosition(LONG Left, LONG Top, LONG Right, LONG Bottom);

    virtual RECT Padding();
    virtual D2D_RECT_F DPIPadding();


    // Trigger OnPaint
    virtual void Invalidate();

    // Handle paint event
    virtual void OnPaint(Control* target, bool Refresh = false);
    virtual void OnPaint(bool PaintChildren = false);
    virtual void OnAnimated();

    // Handle left button down event
    virtual void OnLeftButtonDown(POINT Pt);
    virtual void OnLeftButtonDown(LONG X, LONG Y);
    // Handle left button up event
    virtual void OnLeftButtonUp(POINT Pt);
    virtual void OnLeftButtonUp(LONG X, LONG Y);
    // Handle left button click event
    virtual void OnLeftButtonClick(LONG X, LONG Y);
    // Handle left button double click event
    virtual void OnLeftButtonDoubleClick(LONG X, LONG Y);

    // Handle right button down event
    virtual void OnRightButtonDown(POINT Pt);
    virtual void OnRightButtonDown(LONG X, LONG Y);
    // Handle right button up event
    virtual void OnRightButtonUp(POINT Pt, WPARAM wParam);
    virtual void OnRightButtonUp(LONG X, LONG Y);
    // Handle right button click event
    virtual void OnRightButtonClick(LONG X, LONG Y);
    // Handle right button double click event
    virtual void OnRightButtonDoubleClick(LONG X, LONG Y);

    // Handle mouse move event
    virtual void OnMouseMove(POINT Pt, WPARAM wParam);
    // Handle mouse enter event
    virtual void OnMouseEnter(Control* Target, POINT Pt);
    virtual void OnMouseEnter(LONG X, LONG Y);
    // Handle mouse leave event
    virtual void OnMouseLeave(Control* Target, POINT Pt);
    virtual void OnMouseLeave(LONG X, LONG Y);
    // Handle mouse drag event
    virtual void OnMouseDrag(Control* Target, POINT Pt, POINT LastPt);
    virtual void OnMouseDrag(POINT Pt, POINT LastPt);
    // Hnadle mouse vertical wheel event
    virtual void OnMouseVerticalWheel(POINT Pt, WPARAM wParam);
    virtual void OnMouseVerticalWheel(LONG X, LONG Y, WPARAM wParam);
    // Hnadle mouse horizontal wheel event
    virtual void OnMouseHorizontalWheel(POINT Pt, WPARAM wParam);
    virtual void OnMouseHorizontalWheel(LONG X, LONG Y, WPARAM wParam);


    // Find which control's rect has the point
    virtual inline Control* Find(POINT& Pt);
    // Find which control's rect equal the rect
    virtual inline Control* Find(RECT& Position);
    // Find by id
    virtual inline Control* Find(std::wstring ID);
    // Collect
    virtual inline void Collect(std::unordered_map<std::wstring, Control*>& Container);

    // Test point is in the area
    virtual inline bool Hint(POINT& Pt);
    // Test rect is equal the area
    virtual inline bool Hint(RECT& Position);
    // Test rect is in the area
    virtual inline bool Contains(RECT& Position);


    virtual void SavePosition(UINT16 Index = 0);


    virtual const char* TypeName();


    std::size_t Size();

protected:
    virtual void SaveStylePosition();
    virtual void SaveStyleValues(
        std::initializer_list<Background*>& BackgroundStyle, 
        std::initializer_list<Border*>& BorderStyle, 
        std::initializer_list<Text*>& TextStyle, 
        std::initializer_list<Animation*>& AnimationStyle,
        std::initializer_list<Image*>& ImageStyle
    );

    // Main wnd
    MainWnd* m_Wnd;

    // Parent
    Control* m_Parent;

    // ID
    std::wstring m_ID;

    // Position
    RECT m_Position;
    RECT m_PositionPercentage;
    D2D_RECT_F m_DPIPosition;

    // Margin
    RECT m_Margin;
    RECT m_MarginPercentage;
    D2D_RECT_F m_DPIMargin;

    // Padding
    RECT m_Padding;
    RECT m_PaddingPercentage;
    D2D_RECT_F m_DPIPadding;

    // Hidden
    bool m_Hidden;
    bool m_Clear;

    // Disabled
    bool m_Disabled;

    // Capture event
    bool m_Hintable;

    // Background styles
    CONTROL_STATUS m_BackgroundStyle;
    std::list<Background*> m_BackgroundStyles[CONTROL_STATUS_SIZE];

    // Border styles
    CONTROL_STATUS m_BorderStyle;
    std::list<Border*> m_BorderStyles[CONTROL_STATUS_SIZE];

    // Text styles
    CONTROL_STATUS m_TextStyle;
    std::list<Text*> m_TextStyles[CONTROL_STATUS_SIZE];

    // Animation styles
    CONTROL_STATUS m_AnimationStyle;
    std::list<Animation*> m_AnimationStyles[CONTROL_STATUS_SIZE];

    // Image styles
    CONTROL_STATUS m_ImageStyle;
    std::list<Image*> m_ImageStyles[CONTROL_STATUS_SIZE];

    // Child controls
    std::vector<Control*> m_Children;

    // Left button opation point
    POINT m_LeftButtonDownPoint;
    POINT m_LeftButtonUpPoint;
    // Left button opation tikcout
    DWORD64 m_LeftButtonDownTick;
    DWORD64 m_LeftButtonUpTick;
    DWORD64 m_LeftButtonClickTick;
    DWORD64 m_LeftButtonDoubleClickTick;

    // Right button opation point
    POINT m_RightButtonDownPoint;
    POINT m_RightButtonUpPoint;
    // Right button opation tikcout
    DWORD64 m_RightButtonDownTick;
    DWORD64 m_RightButtonUpTick;
    DWORD64 m_RightButtonClickTick;
    DWORD64 m_RightButtonDoubleClickTick;

    // The control mouse enters
    Control* m_MouseEnter;
    // Last mouse point
    POINT m_LastMousePoint;

    // Scrool enabled
    bool m_VerticalScroolEnabled;
    bool m_HorizontalScroolEnabled;
};