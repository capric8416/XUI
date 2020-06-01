#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Common.h"
#include "Style.h"

// windows
#include <d2d1.h>
#include <dwrite.h>

// c++
#include <string>
#include <thread>
#include <list>

// forward
class MainWnd;
class Event;



class Animation : public Style
{
public:
    Animation
    (
        CONTROL_STATUS Status,

        D2D_RECT_F Position = ZERO_RECT,

        float RadiusX = 0,
        float RadiusY = 0
    );

    ~Animation();

    static void Start(UINT8 FPS);
    static void Stop();
    static void Run();
    static void Add(Control* Target);
    static void Execute();

    virtual void OnAnimated();


protected:
    static UINT8 s_Tick;
    static Event s_Stop;
    static std::thread* s_Thread;
    static std::list<Control*> s_Tasks;
};