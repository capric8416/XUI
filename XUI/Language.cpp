// self
#include "Language.h"


Language::Language()
    : m_Name("")
{
}


Language::Language(std::string Name)
    : m_Name(Name)
{
}


Language::~Language()
{
}


void Language::Read()
{
}


void Language::Init()
{
}


void Language::Write()
{
}


std::string Language::Name()
{
    return m_Name;
}


void Language::Name(std::string Value)
{
    if (Value != m_Name)
    {
        m_Name = Value;
    }
}


std::wstring Language::Translate(std::wstring Key)
{
    auto iter = m_Data[m_Name].find(Key);
    return iter != m_Data[m_Name].end() ? iter->second : Key;
}
