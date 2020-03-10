// self
#include "Common.h"

// windows
#include <Windows.h>


void XTrace(const wchar_t* Format, ...)
{
    va_list args = NULL;
    va_start(args, Format);
    size_t length = _vscwprintf(Format, args) + 1;
    wchar_t* buffer = new wchar_t[length];
    _vsnwprintf_s(buffer, length, length, Format, args);
    va_end(args);
    OutputDebugStringW(buffer);
    delete[] buffer;
}



#define SPLIT_2_TYPE_ARRAY                                 \
    unsigned short start = 0, index = 0, stop, len;        \
    for (stop = 0; Src[stop]; stop++)                      \
    {                                                      \
        if (Src[stop] == Separator)                        \
        {                                                  \
            len = stop - start;                            \
            wcsncpy(Tmp, Src + start, len);                \
            Tmp[len] = L'\0';                              \
            Dst[index++] = Convert(Tmp);                   \
            start = stop + 1;                              \
        }                                                  \
    }                                                      \
    len = stop - start;                                    \
    wcsncpy(Tmp, Src + start, len);                        \
    Tmp[len] = L'\0';                                      \
    Dst[index++] = Convert(Tmp);                           \
    return index;                                          \



unsigned short SplitInt32Array(const wchar_t* Src, int* Dst, wchar_t* Tmp, STR_2_INT32 Convert, wchar_t Separator)
{
    SPLIT_2_TYPE_ARRAY
}



unsigned short SplitFloatArray(const wchar_t* Src, float* Dst, wchar_t* Tmp, STR_2_float Convert, wchar_t Separator)
{
    SPLIT_2_TYPE_ARRAY
}

