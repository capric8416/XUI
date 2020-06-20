#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// c/c++
#include <stdio.h>

// disable warning
#pragma warning(disable: 4127)   // conditional expression is constant
#pragma warning(disable: 4996)   // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 6387)   // could be 0
#pragma warning(disable: 26812)  // The enum type ... is unscorped
#pragma warning(disable: 28159)  // GetTickCount 49 days overflow



// Control status
enum CONTROL_STATUS
{
    CONTROL_STATUS_BTIV = -1,  // BTIV - Before the initial value
    CONTROL_STATUS_NORMAL,
    CONTROL_STATUS_HOVER,
    CONTROL_STATUS_DOWN,
    CONTROL_STATUS_CHECKED,
    CONTROL_STATUS_DISABLED,
    CONTROL_STATUS_SIZE,
};



// Control align
enum CONTROL_ALIGN
{
    CONTROL_ALIGN_BITV = -1,
    CONTROL_ALIGN_LEFT,
    CONTROL_ALIGN_HORIZONTAL_CENTER,
    CONTROL_ALIGN_RIGHT,
    CONTROL_ALIGN_TOP,
    CONTROL_ALIGN_VERTICAL_CENTER,
    CONTROL_ALIGN_BOTTOM,
    CONTROL_ALIGN_SIZE,
};



//
// Safe delete pointer
//
template <class T>
void XSafeDelete(T*& P, bool Array = false, bool NullCheck = true)
{
    if (!NullCheck || P != nullptr)
    {
        if (Array)
            delete[] P;
        else
            delete P;
        P = nullptr;
    }
}



// Safe relase pointer
template <class T>
inline void XSafeRelease(T*& P, bool NullCheck = true)
{
    if (!NullCheck || P != nullptr)
    {
        P->Release();
        P = nullptr;
    }
}





/*
Split string to int32 array
    * @argument Src: src string

    * @argument Dst: dst int32 array
    * @argument Tmp: int32 string buffer
    * @argument Convert: funtion convert int32 string buffer to int32
    * @argument Separator: spliting char

    * @return: dst int32 length

    * @example: WCHAR i32[20]; INT32 rect[4];
    * @example: UINT16 count = SplitInt32Array(L"1234567890 1234567 12345 123", rect, i32, _wtoi);
*/
typedef int(*STR_2_INT32) (const wchar_t*);   // Convert string to int32
unsigned short SplitInt32Array(const wchar_t* Src, int* Dst, wchar_t* Tmp, STR_2_INT32 Convert, wchar_t Separator = L' ');



/*
Split string to float array

    * @argument Src: src string
    * @argument Dst: dst float array
    * @argument Tmp: float string buffer
    * @argument Convert: funtion convert float string buffer to float
    * @argument Separator: spliting char

    * @return: dst float length

    * @example: WCHAR f[20]; float rectf[4];
    * @example: UINT16 count = SplitFloatArray(L"1.2 12.34 123.456 1234.5678", rectf, f, _wtof);
*/
typedef double(*STR_2_float) (const wchar_t*);  // Convert string to float
unsigned short SplitFloatArray(const wchar_t* Src, float* Dst, wchar_t* Tmp, STR_2_float Convert, wchar_t Separator = L' ');



// XTrace like printf
void XTrace(const wchar_t* Format, ...);



// define
#define TICK_COUNT GetTickCount()

#define ON_PAINT_WPARAM    10001  // WM_PAINT wParaw - xui paint
#define ON_ANIMATED_WPARAM 10002  // WM_PAINT wParaw - xui animation

#define CLICK_DELTA        600    // up -> down <= 600ms: click
#define DBL_CLICK_DELTA    400    // click -> click <= 400ms: double click

#define DENOMINATOR    1000
#define ZERO_RECT      {0, 0, 0, 0}
#define FULL_RECT      {0, 0, DENOMINATOR, DENOMINATOR}

#define IS_ZERO_RECT(Rect) (Rect.left == 0 && Rect.top == 0 && Rect.right == 0 && Rect.bottom == 0)
#define IS_FULL_RECT(Rect) (Rect.left == 0 && Rect.top == 0 && Rect.right == DENOMINATOR && Rect.bottom == DENOMINATOR)


// XTRACE only working on debug mode
#ifndef _DEBUG
#define XTRACE(Format, ...) (void)0
#else
#define BACKSLASH L'\\'
#define LAST_BACKSLASH_POS wcsrchr(__FILEW__, BACKSLASH)
#define FILE_NAME LAST_BACKSLASH_POS ? LAST_BACKSLASH_POS + 1 : __FILEW__
#define XTRACE(Format, ...)                          \
        XTrace(                                      \
            L"[%ld %s %d %s] " ## Format ## "\n",    \
            TICK_COUNT,                              \
            FILE_NAME,                               \
            __LINE__,                                \
            __FUNCTIONW__,                           \
            __VA_ARGS__                              \
        )
#endif // !_DEBUG
