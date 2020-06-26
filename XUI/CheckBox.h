#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Control.h"

// c++
#include <initializer_list>
#include <mutex>
#include <set>


// forward
class CheckGroup;


class CheckBox : public Control
{
public:
    // Constructor
    CheckBox(
        std::wstring ID,
        bool Checked = false,
        RECT Position = ZERO_RECT,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        LONG* User = nullptr,
        bool Hidden = false,
        bool Disabled = false,
        bool Hintable = true,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Image*> ImageStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    virtual ~CheckBox();

    void SetGrouped();
    void SetChecked(bool Status = true, bool Notify = true);

    bool Checked();

    // Handle left button click event
    virtual void OnLeftButtonClick(LONG X, LONG Y);

    // Handle mouse enter event
    virtual void OnMouseEnter(LONG X, LONG Y);

    // Handle mouse leave event
    virtual void OnMouseLeave(LONG X, LONG Y);


protected:
    LONG* m_User;

    bool m_Checked;

    bool m_Grouped;
};



class CheckGroup : public Control
{
public:
    // Constructor
    CheckGroup(
        std::wstring ID,
        RECT Position,
        bool Vertical,
        bool Scroll = false,
        std::initializer_list<Control*> Children = {},
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Image*> ImageStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    CheckGroup(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        bool Vertical = false,
        bool Scroll = false,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        bool Hidden = false,
        bool Disabled = false,
        bool Hintable = true,
        std::initializer_list<Control*> Children = {},
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Image*> ImageStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    virtual ~CheckGroup();

    virtual void Attach(CheckBox* Child, bool Checked = false, bool Pin = false, bool Paint = true, bool Lazy = false);
    virtual void Dettach(CheckBox* Child);

    virtual void Resize();

    virtual void OnChildChecked(CheckBox* Child, bool Checked, bool Notify = true);
    std::set<CheckBox*> Checked();

    void Pin(std::wstring ID);
    void Unpin(std::wstring ID);
    CheckBox* Pinned();

    // Handle mouse vertical wheel event
    virtual void OnMouseVerticalWheel(LONG X, LONG Y, WPARAM wParam);
    // Handle mouse horizontal wheel event
    virtual void OnMouseHorizontalWheel(LONG X, LONG Y, WPARAM wParam);


protected:
    bool m_Vertical;

    CheckBox* m_LastCheck;
    std::set<CheckBox*> m_Checked;

    CheckBox* m_Pinned;

    mutable std::mutex m_Mutex;
};