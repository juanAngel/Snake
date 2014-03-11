#ifndef GUIVIEW_H
#define GUIVIEW_H

#include "view.h"
typedef enum typeGui{
    windows,
    layout,
    label,
    button,
    radioButton,
    checkBox,
    listBox,
    textField,
    scrollBar
}typeGui;

typedef struct GuiView:view{
    typeGui type;
}GuiView;

typedef struct groupView:GuiView{

}groupView;
typedef struct buttonView:GuiView{

}buttonView;

typedef struct windowsView:GuiView{
    char* title;
    groupView* layoutManager;
}windowsView;

typedef struct labelView:GuiView{
    char* text;
}labelView;
typedef struct textFieldView:GuiView{
    char* text;
}textFieldView;

void initWindows(windowsView* w, event::EventLoop *defaultLoop);
void initLabel(labelView* w, event::EventLoop *defaultLoop);

void initButton(buttonView* w, event::EventLoop *defaultLoop);
void initTextField(textFieldView* w, event::EventLoop *defaultLoop);

#endif // GUIVIEW_H
