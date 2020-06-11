#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Control.h"



class RadioBox : public Control
{
public:
    // Constructor
    RadioBox(
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
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    ~RadioBox();

    void SetGrouped();
    void SetChecked(bool Status = true, bool Notify = true);

    size_t GetPos();
    void SetPos(size_t Value);

    // Handle left button click event
    virtual void OnLeftButtonClick(LONG X, LONG Y);

    void Check();
    void Uncheck();

    bool Checked();


protected:
    LONG* m_User;

    bool m_Checked;

    bool m_Grouped;

    size_t m_Pos;
};



class RadioGroup : public Control
{
public:
    // Constructor
    RadioGroup(
        std::wstring ID,
        RECT Position,
        bool Vertical,
        bool Scroll = false,
        std::initializer_list<Control*> Children = {},
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    RadioGroup(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        bool Vertical = false,
        bool Scroll = true,
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
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    ~RadioGroup();

    virtual void Attach(RadioBox* Child, bool Checked = false, bool Pin = false, bool Paint = true);
    virtual void Dettach(RadioBox* Child);

    virtual void Resize();

    virtual void OnChildChecked(Control* Child, bool Notify = true);
    RadioBox* Checked();

    void Siblings(RadioBox* Current, RadioBox*& Previous, RadioBox*& Next);

    bool Check(size_t Pos);

    void Pin(std::wstring ID);
    void Unpin(std::wstring ID);
    RadioBox* Pinned();


protected:
    float m_Padding;

    float m_Margin;

    bool m_Vertical;

    RadioBox* m_Checked;

    RadioBox* m_Pinned;
};