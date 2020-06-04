// self
#include "Xml.h"

// project
#include "Common.h"

// windows
#include <shlwapi.h>

// namespace
using namespace std;

// link
#pragma comment(lib, "shlwapi")

// define
// value type
#define XML_TAG           L"TAG"
#define XML_TEXT          L"TEXT"
// tag name
#define XML_TAG_BG        L"background"
#define XML_TAG_TEXT      L"text"
#define XML_TAG_NORMAL    L"normal"
// attr name
#define XML_ATTR_ID       L"id"
#define XML_ATTR_CLASS    L"class"


XmlNode::XmlNode() :
    m_Parent(nullptr),
    m_Depth(0)
{
}


XmlNode::~XmlNode()
{
    m_Attr.clear();

    for (const auto& child : m_Children)
    {
        delete child;
    }

    m_Children.clear();
}


void XmlNode::Append(XmlNode* Child)
{
    m_Children.push_front(Child);
}


void XmlNode::Print()
{
    XTRACE("  <");
    XTRACE("    Depth = %d", m_Depth);
    XTRACE("    Tag = %s", m_Tag.c_str());
    for (const auto& iter : m_Attr)
    {
        XTRACE("    %s = %s", iter.first.c_str(), iter.second.c_str());
    }
    if (m_Text.length() > 0)
    {
        XTRACE("    Text = %s", m_Text.c_str());
    }
    XTRACE("  >\n\n");

    for (const auto& child : m_Children)
    {
        child->Print();
    }
}


wstring XmlNode::Get(wstring Key)
{
    if (Key == XML_TAG)
    {
        return Tag();
    }
    else if (Key == XML_TEXT)
    {
        return Text();
    }
    else
    {
        return Attr(Key);
    }
}


void XmlNode::Set(wstring Key, wstring Value)
{
    if (Key == XML_TAG)
    {
        Tag(Value);
    }
    else if (Key == XML_TEXT)
    {
        Text(Value);
    }
    else
    {
        Attr(Key, Value);
    }
}


wstring XmlNode::Tag()
{
    return m_Tag;
}


void XmlNode::Tag(wstring Name)
{
    m_Tag = Name;
}


wstring XmlNode::Attr(wstring Key)
{
    return m_Attr[Key];
}


map<wstring, wstring>& XmlNode::Attr()
{
    return m_Attr;
}


void XmlNode::Attr(wstring Key, wstring Value)
{
    m_Attr[Key] = Value;
}


wstring XmlNode::Text()
{
    return m_Text;
}


void XmlNode::Text(wstring Value)
{
    m_Text = Value;
}


XmlNode* XmlNode::Parent()
{
    return m_Parent;
}


void XmlNode::Parent(XmlNode* Target)
{
    m_Parent = Target;
}


uint32_t XmlNode::Depth()
{
    return m_Depth;
}


void XmlNode::Depth(uint32_t Value)
{
    m_Depth = Value;
}



XmlNode* XmlNode::FirstChild()
{
    return m_Children.size() > 0 ? m_Children.front() : nullptr;
}


std::list<XmlNode*>& XmlNode::Children()
{
    return m_Children;
}


void StyleNode::Inherit(StyleNode& Parent)
{
    //// background: normal -> clear
    //// text: attrs

    //for (const auto& typeNode : Parent.Children())
    //{
    //    auto tag = typeNode->Tag();
    //    if (tag != XML_TAG_BG && tag != XML_TAG_TEXT)
    //    {
    //        continue;
    //    }

    //    auto type = new XmlNode();
    //    type->Tag(typeNode->Tag());

    //    for (const auto& statusNode : typeNode->Children())
    //    {
    //        if (tag == XML_TAG_BG && tag != XML_TAG_NORMAL)
    //        {
    //            continue;
    //        }

    //        auto status = new XmlNode();
    //        status->Tag(tag != XML_TAG_BG ? statusNode->Tag() : XML_TAG_CLEAR);

    //        for (const auto& attr : statusNode->Attr())
    //        {
    //            status->Attr(attr.first, attr.second);
    //        }

    //        type->Append(status);
    //    }
    //}
}


void StyleNode::Extend(StyleNode& Source)
{
    for (const auto& typeNode : Source.Children())
    {
        // get exists type node or create one
        XmlNode* type = nullptr;
        auto newTypeNode = false;
        auto typeNodeTag = typeNode->Tag();
        for (const auto& exists : m_Children)
        {
            if (exists->Tag() == typeNodeTag)
            {
                type = exists;
                XTRACE("= %s", typeNodeTag.c_str());
                break;
            }
        }
        if (type == nullptr)
        {
            type = new XmlNode();
            newTypeNode = true;
            type->Tag(typeNodeTag);
            XTRACE("+ %s", typeNodeTag.c_str());
        }

        for (const auto& statusNode : typeNode->Children())
        {
            // get exists status node or create one
            XmlNode* status = nullptr;
            auto newStatusNode = false;
            auto statusNodeTag = statusNode->Tag();
            if (!newTypeNode)
            {
                for (const auto& exists : type->Children())
                {
                    if (exists->Tag() == statusNodeTag)
                    {
                        status = exists;
                        XTRACE("    = %s", statusNodeTag.c_str());
                        break;
                    }
                }
            }
            if (status == nullptr)
            {
                status = new XmlNode();
                newStatusNode = true;
                status->Tag(statusNodeTag);
                XTRACE("    + %s", statusNodeTag.c_str());
            }

            // update attr
            for (const auto& attr : statusNode->Attr())
            {
                status->Attr(attr.first, attr.second);
            }

            // update text
            auto text = statusNode->Text();
            if (text.length() > 0)
            {
                status->Text(text);
            }

            // append new status node
            if (newStatusNode)
            {
                type->Append(status);
            }
        }

        // append new type node
        if (newTypeNode)
        {
            Append(type);
        }
    }
}



ControlNode::ControlNode() :
    m_Style(nullptr)
{
}


ControlNode::~ControlNode()
{
    XSafeDelete(m_Style);
}


StyleNode* ControlNode::Style()
{
    return m_Style;
}


void ControlNode::Style(StyleNode* ParentStyle, map<wstring, XmlNode*>& TagStyles, map<wstring, XmlNode*>& ClassStyles, map<wstring, XmlNode*>& IDStyles)
{
    XTRACE("=========style: %s", Attr(XML_ATTR_ID).c_str());

    m_Style = new StyleNode();

    if (ParentStyle != nullptr)
    {
        m_Style->Inherit(*ParentStyle);
    }

    ExtendStyle(m_Tag, TagStyles);

    if (m_Attr.find(XML_ATTR_CLASS) != m_Attr.end())
    {
        ExtendStyle(m_Attr[XML_ATTR_CLASS], ClassStyles);
    }

    if (m_Attr.find(XML_ATTR_ID) != m_Attr.end())
    {
        ExtendStyle(m_Attr[XML_ATTR_ID], IDStyles);
    }

    for (const auto& child : m_Children)
    {
        ((ControlNode*)child)->Style(m_Style, TagStyles, ClassStyles, IDStyles);
    }
}


void ControlNode::ExtendStyle(wstring Name, map<wstring, XmlNode*>& Styles)
{
    auto style = Styles.find(Name);
    if (style != Styles.end())
    {
        m_Style->Extend((StyleNode&)*style->second);
    }
}



XmlNode* Xml::Read(wstring Path)
{
    HRESULT hr = S_OK;

    IStream* stream = NULL;

    IXmlReader* reader = NULL;

    XmlNodeType type;

    stack<XmlNode*> m_Controls;
    stack<pair<wstring, wstring>> m_Nodes;

    // Open read-only input stream
    if (FAILED(hr = SHCreateStreamOnFile(Path.c_str(), STGM_READ, &stream)))
    {
        XTRACE("Error creating file reader, error is %08.8lx", hr);
        goto CleanUp;
    }

    if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&reader, NULL)))
    {
        XTRACE("Error creating xml reader, error is %08.8lx", hr);
        goto CleanUp;
    }

    if (FAILED(hr = reader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
    {
        XTRACE("Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
        goto CleanUp;
    }

    if (FAILED(hr = reader->SetInput(stream)))
    {
        XTRACE("Error setting input for reader, error is %08.8lx", hr);
        goto CleanUp;
    }

    // Read until there are no more nodes
    while (S_OK == (hr = reader->Read(&type)))
    {
        hr = ReadNode(reader, type, m_Controls, m_Nodes);
        if (FAILED(hr))
        {
            goto CleanUp;
        }
    }

CleanUp:
    XSafeRelease(reader);
    XSafeRelease(stream);

    return m_Controls.top();
}


HRESULT Xml::ReadNode(IXmlReader* Reader, XmlNodeType Type, stack<XmlNode*>& Controls, stack<pair<wstring, wstring>>& Nodes)
{
    HRESULT hr = S_OK;

    const wchar_t* tag;
    const wchar_t* value;

    switch (Type)
    {
        case XmlNodeType_Element:
        {
            if (FAILED(hr = Reader->GetLocalName(&tag, NULL)))
            {
                XTRACE("Error getting local name, error is %08.8lx", hr);
                return hr;
            }

            // push tag name
            Nodes.push(make_pair(XML_TAG, tag));

            if (FAILED(hr = ReadAttributes(Reader, Nodes)))
            {
                XTRACE("Error writing attributes, error is %08.8lx", hr);
                return hr;
            }
        }
        break;

        case XmlNodeType_EndElement:
        {
            if (FAILED(hr = Reader->GetLocalName(&tag, NULL)))
            {
                XTRACE("Error getting local name, error is %08.8lx", hr);
                return hr;
            }

            // new node, and set depth
            auto curent = new XmlNode();
            UINT depth;
            Reader->GetDepth(&depth);
            curent->Depth(depth);

            // set tag name, pop and set attrs
            curent->Set(XML_TAG, tag);
            while (Nodes.top().first != XML_TAG && Nodes.top().second != tag)
            {
                curent->Set(Nodes.top().first, Nodes.top().second);
                Nodes.pop();
            }
            Nodes.pop();

            // pop children
            while (Controls.size() > 0)
            {
                XmlNode* last = Controls.top();
                if (last->Depth() != depth + 1)
                {
                    break;
                }

                last->Parent(curent);
                curent->Append(last);
                Controls.pop();
            }

            // push self
            Controls.push(curent);
        }
        break;

        case XmlNodeType_Text:
        {
            if (FAILED(hr = Reader->GetValue(&value, NULL)))
            {
                XTRACE("Error getting value, error is %08.8lx", hr);
                return hr;
            }

            // push text value
            Nodes.push(make_pair(XML_TEXT, value));
        }
        break;
    }

    return hr;
}


HRESULT Xml::ReadAttributes(IXmlReader* Reader, stack<pair<wstring, wstring>>& Nodes)
{
    const wchar_t* key;
    const wchar_t* value;

    HRESULT hr = Reader->MoveToFirstAttribute();

    if (S_FALSE == hr)
    {
        return hr;
    }

    if (S_OK != hr)
    {
        XTRACE("Error moving to first attribute, error is %08.8lx", hr);
        return hr;
    }

    while (TRUE)
    {
        if (!Reader->IsDefault())
        {
            if (FAILED(hr = Reader->GetLocalName(&key, NULL)))
            {
                XTRACE("Error getting local name, error is %08.8lx", hr);
                return hr;
            }

            if (FAILED(hr = Reader->GetValue(&value, NULL)))
            {
                XTRACE("Error getting value, error is %08.8lx", hr);
                return hr;
            }

            // push attr key-value
            Nodes.push(make_pair(wstring(key), wstring(value)));
        }

        if (S_OK != Reader->MoveToNextAttribute())
        {
            break;
        }
    }

    return hr;
}
