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

typedef struct layoutView:GuiView{

}layoutView;
typedef struct buttonView:GuiView{

}buttonView;

typedef struct windowsView:GuiView{
    char* title;
    layoutView layoutManager;
}windowsView;

void initWindows(windowsView* w);

#endif // GUIVIEW_H
