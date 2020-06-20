#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// c
#include <stdint.h>

// c++
#include <list>
#include <map>
#include <stack>
#include <string>

// windows
#include <combaseapi.h>
#include <xmllite.h>

// link
#pragma comment(lib, "xmllite")



class XmlNode
{
public:
    XmlNode();
    virtual ~XmlNode();

    void Append(XmlNode* Child);

    virtual void Print();

    std::wstring Get(std::wstring Key);
    void Set(std::wstring Key, std::wstring Value);

    std::wstring Tag();
    void Tag(std::wstring Name);

    std::wstring Attr(std::wstring Key);
    std::map<std::wstring, std::wstring>& Attr();
    void Attr(std::wstring Key, std::wstring Value);

    std::wstring Text();
    void Text(std::wstring Value);

    XmlNode* Parent();
    void Parent(XmlNode* Target);

    uint32_t Depth();
    void Depth(uint32_t Value);

    XmlNode* FirstChild();
    std::list<XmlNode*>& Children();

protected:
    uint32_t m_Depth;

    std::wstring m_Tag;
    std::wstring m_Text;
    std::map<std::wstring, std::wstring> m_Attr;

    XmlNode* m_Parent;
    std::list<XmlNode*> m_Children;
};



class StyleNode : public XmlNode
{
public:
    void Inherit(StyleNode& Parent);
    void Extend(StyleNode& Source);
};



class ControlNode : public XmlNode
{
public:
    ControlNode();
    ~ControlNode();

    StyleNode* Style();
    void Style(StyleNode* ParentStyle, std::map<std::wstring, XmlNode*>& TagStyles, std::map<std::wstring, XmlNode*>& ClassStyles, std::map<std::wstring, XmlNode*>& IDStyles);

protected:
    void ExtendStyle(std::wstring Name, std::map<std::wstring, XmlNode*>& Styles);

    StyleNode* m_Style;
};



class Xml
{
public:
    static XmlNode* Read(std::wstring Path);

private:
    static HRESULT ReadNode(IXmlReader* Reader, XmlNodeType Type, std::stack<XmlNode*>& Controls, std::stack<std::pair<std::wstring, std::wstring>>& Nodes);
    static HRESULT ReadAttributes(IXmlReader* Reader, std::stack<std::pair<std::wstring, std::wstring>>& Nodes);
};

