#include "guiview.h"




void windowsDraw(renderer::DrawContext* /*context*/,windowsView* drawableObject){
    printf("windowsDraw\n");
}

void initWindows(windowsView *w){
    if(w){
        memset(w,0,sizeof(*w));
        initView(w);
        w->renderDrawable = (renderer::DrawFunc)&windowsDraw;
    }
}
