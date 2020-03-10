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
#include <string>

// define
#define SliderBarLabel Label
#define SliderBarValue Label

// forward
class Button;
class Label;
class SliderBar;



class SliderBarBody : public Control
{
public:
    SliderBarBody(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        INT32 Value = 0,
        INT32 MinValue = 0,
        INT32 MaxValue = 100,
        INT32 StepValue = 1,
        bool Vertical = false,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        bool Hidden = false,
        bool Disabled = false,
        bool Hintable = true,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    ~SliderBarBody();

    virtual void OnLeftButtonClick(LONG X, LONG Y);

    virtual void OnMouseDrag(POINT Pt, POINT LastPt);
    
    void UpdateValue(LONG X, LONG Y, bool Init = false, bool Paint = true);

    void Value(INT32 Value);

    virtual void SavePosition(UINT16 Index);

protected:
    INT32 m_Value;
    INT32 m_MinValue;
    INT32 m_MaxValue;
    INT32 m_StepValue;
    bool m_Vertical;
};



class SliderBar : public Control
{
public:
    SliderBar(
        std::wstring ID,
        RECT Position,
        INT32 Value = 0,
        INT32 MinValue = 0,
        INT32 MaxValue = 100,
        INT32 StepValue = 1,
        bool Vertical = false,
        SliderBarLabel* TitleLabel = nullptr,
        SliderBarBody* SliderBody = nullptr,
        SliderBarLabel* ValueLabel = nullptr,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        bool Hidden = false,
        bool Disabled = false,
        bool Hintable = true,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );

    ~SliderBar();

    virtual void OnValueChange(INT32 Value);

    INT32 Value();
    void Value(INT32 Value);
    void Min();
    void Max();


protected:
    INT32 m_Value;

    INT32 m_MinValue;
    INT32 m_MaxValue;
    INT32 m_StepValue;

    bool m_Vertical;

    SliderBarLabel* m_SliderLabel;
    SliderBarValue* m_SliderValue;
    SliderBarBody* m_SliderBody;
};