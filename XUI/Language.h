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
#include <unordered_map>



class Language
{
public:
    Language();
    Language(std::string Name);
    virtual ~Language();

    virtual void Read();
    virtual void Init();
    virtual void Write();

    std::string Name();
    void Name(std::string Value);

    std::wstring Translate(std::wstring Key);


protected:
    std::string m_Name;
    std::map<std::string, std::unordered_map<std::wstring, std::wstring>> m_Data;
};
