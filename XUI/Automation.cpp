// self
#include "Automation.h"
#include "UI.h"

// c/c++
#include <algorithm>

// namespace
using namespace std;


// static
void* Automation::m_File = nullptr;
void* Automation::m_View = nullptr;


bool IsNumber(wstring Text)
{
    if (Text[0] == L'+' || Text[0] == L'-')
    {
        Text.erase(0, 1);
    }
    return !Text.empty() && std::all_of(Text.begin(), Text.end(), iswdigit);
}


bool IsPositiveNumber(wstring Text)
{
    if (Text[0] == L'+')
    {
        Text.erase(0, 1);
    }
    return !Text.empty() && std::all_of(Text.begin(), Text.end(), iswdigit);
}


int Automation::Route(UI* Ui, PCOPYDATASTRUCT Data)
{
    memset(m_View, 0, XUI_AUTOMATION_SHARED_BYTES);

    std::map<std::wstring, std::wstring> request = { {L"token", L""} };
    Parse((wchar_t*)Data->lpData, L"\r", L"\n", request);

    if (request[L"token"] != XUI_AUTOMATION_COPYDATA_TOKEN)
    {
        wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid token\"}");
        return -1;
    }
    if (request.size() < 3)
    {
        wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid params\"}");
        return -1;
    }

    if (request.find(L"id") == request.end() || request[L"id"].empty())
    {
        wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid id\"}");
        return -1;
    }

    if (request[L"action"] == L"OnLeftClick")
    {
        return Ui->OnLeftClick(request[L"id"], (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnLeftDoubleClick")
    {
        return Ui->OnLeftDoubleClick(request[L"id"], (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnRightClick")
    {
        return Ui->OnRightClick(request[L"id"], (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnRightDoubleClick")
    {
        return Ui->OnRightDoubleClick(request[L"id"], (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnGroupItemChanged")
    {
        if (request.find(L"position") == request.end() || !IsPositiveNumber(request[L"position"]))
        {
            wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid position\"}");
            return -1;
        }
        if (request.find(L"value") == request.end() || !IsPositiveNumber(request[L"value"]))
        {
            wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid value\"}");
            return -1;
        }

        return Ui->OnGroupItemChanged(request[L"id"], _wtol(request[L"position"].c_str()), _wtol(request[L"value"].c_str()), (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnSliderValueChanged")
    {
        if (request.find(L"value") == request.end() || !IsPositiveNumber(request[L"value"]))
        {
            wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid value\"}");
            return -1;
        }

        return Ui->OnSliderValueChanged(request[L"id"], _wtol(request[L"value"].c_str()), (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnMouseVerticalScroll")
    {
        if (request.find(L"delta") == request.end() || !IsNumber(request[L"delta"]))
        {
            wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid delta\"}");
            return -1;
        }

        return Ui->OnMouseVerticalScroll(request[L"id"], _wtoi(request[L"delta"].c_str()), (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnMouseHorizontalScroll")
    {
        if (request.find(L"delta") == request.end() || !IsNumber(request[L"delta"]))
        {
            wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid delta\"}");
            return -1;
        }

        return Ui->OnMouseHorizontalScroll(request[L"id"], _wtoi(request[L"delta"].c_str()), (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnCharInput")
    {
        if (request.find(L"text") == request.end() || request[L"text"].empty())
        {
            wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid text\"}");
            return -1;
        }

        return Ui->OnCharInput(request[L"id"], request[L"text"].c_str(), (wchar_t*)m_View);
    }
    else if (request[L"action"] == L"OnKeyInput")
    {
        if (request.find(L"key") == request.end() || !IsPositiveNumber(request[L"key"]))
        {
            wcscpy((wchar_t*)m_View, L"{\"message\": \"invalid key\"}");
            return -1;
        }

        return Ui->OnKeyInput(request[L"id"], _wtoi(request[L"key"].c_str()), (wchar_t*)m_View);
    }

    wcscpy((wchar_t*)m_View, L"{\"message\": \"unsupport action\"}");

    return -1;
}


void Automation::Parse(wstring Text, wstring HDelimiter, wstring VDelimiter, map<wstring, wstring>& Resulsts)
{
    size_t vPos = 0;
    size_t hPos = 0;

    std::wstring line = L"";

    while ((vPos = Text.find(VDelimiter)) != std::wstring::npos)
    {
        line = Text.substr(0, vPos);
        hPos = line.find(HDelimiter);
        if (hPos != std::wstring::npos)
        {
            Resulsts[line.substr(0, hPos)] = line.substr(hPos + VDelimiter.length(), line.size() - hPos - VDelimiter.length());
        }

        Text.erase(0, vPos + VDelimiter.length());
    }

    line = Text;
    hPos = line.find(HDelimiter);
    if (hPos != std::wstring::npos)
    {
        Resulsts[line.substr(0, hPos)] = line.substr(hPos + VDelimiter.length(), line.size() - hPos - VDelimiter.length());
    }
}


void Automation::Open()
{
    m_File = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        XUI_AUTOMATION_SHARED_BYTES,
        XUI_AUTOMATION_SHARED_MEMORY
    );
}


void Automation::Close()
{
    if (m_File != nullptr)
    {
        CloseHandle(m_File);
        m_File = nullptr;
    }
}


void Automation::Map()
{
    m_View = MapViewOfFile(
        m_File,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        XUI_AUTOMATION_SHARED_BYTES
    );
}


void Automation::UnMap()
{
    if (m_View != nullptr)
    {
        UnmapViewOfFile(m_View);
        m_View = nullptr;
    }
}
