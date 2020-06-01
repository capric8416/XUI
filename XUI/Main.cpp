#ifdef _DEBUG


// project
#include "MainWnd.h"
#include "UI.h"
#include "Button.h"
#include "CheckBox.h"
#include "Label.h"
#include "RadioBox.h"
#include "ProgressBar.h"
#include "SliderBar.h"
#include "LoadingCircle.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"
#include "Animation.h"
#include "Tab.h"

#include <Windows.h>
#include "Xml.h"
#include "Common.h"

using namespace std;



class MyUI : public UI
{
public:
    virtual void OnGroupItemChanged(Control* Parent, Control* Last, Control* Current, LONG Value)
    {
    }

    virtual Control* Create()
    {
        m_Tree = new Control(
            L"Body", ZERO_RECT, false, false, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, { -5, -5, -5, -5 }, true,
            {
                new TabGroup(
                    L"RadioTab", {0, 0, 300, 300},
                    new TAB_RADIO_NAV(
                        L"RadioNav", { 0, 0, 300, 1000 }, true, false, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, { -1, -1, -1, -1}, false, false, true,
                        {
                            new RadioBox(
                                L"CastScreen", true, {0, 0, 1000, 500}, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, nullptr, false, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                    new Background(CONTROL_STATUS_CHECKED, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Border(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Text(L"投屏幕", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                    new Text(L"投屏幕", CONTROL_STATUS_CHECKED, ZERO_RECT, 0, 0, D2D1::ColorF(0xffffff))
                                }
                            ),
                            new RadioBox(
                                L"CastWindow", false, {0, 500, 1000, 1000}, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, nullptr, false, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                    new Background(CONTROL_STATUS_CHECKED, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Border(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Text(L"投窗口", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                    new Text(L"投窗口", CONTROL_STATUS_CHECKED, ZERO_RECT, 0, 0, D2D1::ColorF(0xffffff))
                                }
                            ),
                        },
                        {},
                        {
                            new Border(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0x009688))
                        }
                    ),
                    new TAB_BOX(
                        L"RadioBox", {300, 0, 1000, 1000}, false, false, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, {-10, 0, 0, 0}, {-1, -1, -1, -1}, true,
                        {
                            new Label(
                                L"Screen", ZERO_RECT, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, false, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                },
                                {},
                                {
                                    new Text(L"屏幕", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                }
                            ),
                            new Label(
                                L"Window", ZERO_RECT, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, true, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                },
                                {},
                                {
                                    new Text(L"窗口", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                }
                            ),
                        }
                    )
                ),

                new TabGroup(
                    L"CheckTab", {0, 400, 300, 700},
                    new TAB_CHECK_NAV(
                        L"CheckNav", { 0, 800, 1000, 1000 }, false, false, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, { -1, -1, -1, -1}, false, false, true,
                        {
                            new CheckBox(
                                L"CastVideo", false, {0, 0, 500, 1000}, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, nullptr, false, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                    new Background(CONTROL_STATUS_CHECKED, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Border(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Text(L"投视频", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                    new Text(L"投视频", CONTROL_STATUS_CHECKED, ZERO_RECT, 0, 0, D2D1::ColorF(0xffffff))
                                }
                            ),
                            new CheckBox(
                                L"CastAudio", false, {500, 0, 1000, 1000}, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, nullptr, false, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                    new Background(CONTROL_STATUS_CHECKED, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Border(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0x009688))
                                },
                                {
                                    new Text(L"投音频", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                    new Text(L"投音频", CONTROL_STATUS_CHECKED, ZERO_RECT, 0, 0, D2D1::ColorF(0xffffff))
                                }
                            ),
                        },
                        {},
                        {
                            new Border(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0x009688))
                        }
                    ),
                    new TAB_BOX(
                        L"CheckBox", {0, 0, 1000, 800}, false, false, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, {-1, -1, -1, -1}, true,
                        {
                            new Label(
                                L"Video", {0, 0, 500, 1000}, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, true, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                },
                                {},
                                {
                                    new Text(L"视频", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                }
                            ),
                            new Label(
                                L"Audio", {500, 0, 1000, 1000}, CONTROL_ALIGN_HORIZONTAL_CENTER, CONTROL_ALIGN_VERTICAL_CENTER, ZERO_RECT, ZERO_RECT, true, false, true,
                                {
                                    new Background(CONTROL_STATUS_NORMAL, ZERO_RECT, D2D1::ColorF(0xffffff)),
                                },
                                {},
                                {
                                    new Text(L"音频", CONTROL_STATUS_NORMAL, ZERO_RECT),
                                }
                            ),
                        }
                    )
                )
            }
        );


        return m_Tree;
    }
};



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    //XmlNode* layout = Xml::Read(L"D:\\airfly\\XUI\\XUI\\Layout.xml");
    ////layout->Print();

    //XmlNode* style = Xml::Read(L"D:\\airfly\\XUI\\XUI\\Style.xml");
    ////style->Print();

    //XmlNode* text = Xml::Read(L"D:\\airfly\\XUI\\XUI\\Text.xml");
    ////text->Print();

    //// style = parent + tag + class + id
    //// group by tag, class, id
    //map<wstring, XmlNode*> tagStyles;
    //map<wstring, XmlNode*> classStyles;
    //map<wstring, XmlNode*> idStyles;
    //for (const auto& style : style->Children())
    //{
    //    wstring type = style->Attr(L"type");
    //    if (type == L"tag")
    //    {
    //        tagStyles[style->Tag()] = style;
    //    }
    //    else if (type == L"class")
    //    {
    //        classStyles[style->Tag()] = style;
    //    }
    //    else if (type == L"id")
    //    {
    //        idStyles[style->Tag()] = style;
    //    }
    //}

    //auto root = (ControlNode*)layout->FirstChild();
    //root->Style(nullptr, tagStyles, classStyles, idStyles);

    //delete layout;
    //delete style;
    //delete text;

    MainWnd win;

    // Setup ui
    win.Setup(new MyUI());

    // Create window
    if (!win.Create(L"XUI", L"XUI Window", WS_OVERLAPPEDWINDOW, 0, 660, 240, 800, 640))
    {
        return 0;
    }

    // Show window
    win.Show(nCmdShow);

    // Run the message loop
    win.Loop();

    return 0;
}


#endif // _DEBUG
