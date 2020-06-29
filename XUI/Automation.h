#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN


// c/c++
#include <map>
#include <string>

// windows
#include <Windows.h>


// define
#define XUI_AUTOMATION_SHARED_BYTES 4096
#define XUI_AUTOMATION_SHARED_MEMORY L"XUI_SHARED_MEMORY"
#define XUI_AUTOMATION_COPYDATA_TOKEN L"4efaf19f9db714429b7be29bbf4b5987bfdea237"


// forward
class UI;


class Automation
{
public:
    static int Route(UI* Ui, PCOPYDATASTRUCT Data);
    static void Parse(std::wstring Text, std::wstring HDelimiter, std::wstring VDelimiter, std::map<std::wstring, std::wstring>& Resulsts);
    
    static void Open();
    static void Close();

    static void Map();
    static void UnMap();

protected:
    static HANDLE m_File;
    static LPVOID m_View;
};