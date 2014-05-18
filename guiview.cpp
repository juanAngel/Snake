#include "guiview.h"

#include "SDL2/SDL_ttf.h"
#include "renderer.h"
#include "records.h"

TTF_Font* font;
SDL_Color fontColor;
SDL_Color fontColorBacground;
SDL_Rect r = {200,200,135,16};
//const char* fontName = "font/lazy.ttf";

typedef struct GuiEventLoop:event::EventLoop{
    void (*onClick)(GuiEventLoop* loop,SDL_Event* e);
    void (*onTextInput)(GuiView* v,const char* text);
    void (*onKey)(GuiView* v,SDL_Keysym* key,bool down);
    void (*onGetFocus)(GuiView* v);
    void (*onLostFocus)(GuiView* v);
}GuiEventLoop;

GuiView* focusView;


void buttonOnClick(GuiEventLoop* loop,SDL_Event* /*e*/){
    buttonView* boton = (buttonView*)eventUserContext(loop);
    if(boton->onPress)
        boton->onPress(boton);
}
void inputBox_onPress(buttonView* b){
    inputBox* iBox = (inputBox*)b->userContext;
    if(iBox->onAccept)
        iBox->onAccept(iBox,(char*)labelGetText(&iBox->txtInput));
    //printf("inputBox_onPress\n\t%s\n",iBox->txtInput.text);
}
void inputBox_onEnter(textFieldView* txtFiel){
    inputBox* iBox = (inputBox*)txtFiel->userContext;
    iBox->btnAccept.onPress(&iBox->btnAccept);
}

void textField_onGetFocus(GuiView* g){
    SDL_SetTextInputRect(&g->r);
    SDL_StartTextInput();
    //printf("textField_onGetFocus\n");
}
void textField_onLostFocus(GuiView* /*g*/){
    SDL_StopTextInput();
}
void textField_onKey(textFieldView* v,SDL_Keysym* key,bool down){
    switch (key->sym) {
        case SDLK_BACKSPACE:
            if(v->buff.nElementUsaje && down){
                getDataVector(v->buff,char)[--v->buff.nElementUsaje] = 0;
                if(v->Offset)
                    v->Offset--;
                renderer::sendUpdateWindows();
            }
            break;
        case SDLK_RETURN:
            if(down && v->onPressEnter)
                v->onPressEnter(v);
            break;
        default:
            break;
    }
}
void textField_onTextInput(textFieldView* t,const char* text){
    SDL_Event e;

    e.type = updateEventID;
    if(t && text){
        int len = strlen(text);
        t->buff.nElementUsaje += len;
        if(t->buff.nElementUsaje > t->buff.nElementMax){
            resizeVector(&t->buff,t->buff.nElementMax);
        }
        strcat(getDataVector(t->buff,char),text);

        Size textSize;
        TTF_SizeText(t->font,labelGetText(t),(int*)&textSize.w,(int*)&textSize.h);
        if(((ulong)t->r.w)<=textSize.w){
            t->Offset++;
        }

        printf("text %s\n",labelGetText(t));
    }
    SDL_PushEvent(&e);
}

int groupEventDispacher(event::EventLoop* loop,SDL_Event* e){
    int status = EVENT_NOT_HANDLE;
    if(loop && e){
        groupView* group = (groupView*)loop->loopContext;
        SDL_Rect* r;
        switch (e->type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                for (GuiViewList* children = group->child; children; children = (GuiViewList*)children->next) {
                    r = &children->view->r;
                    if(r->x <= e->button.x && (r->x+r->w)>= e->button.x
                            &&
                            r->y <= e->button.y && (r->y+r->h)>= e->button.y){
                        status = event::dispachEvent(children->view->loop,e);

                        if(e->type == SDL_MOUSEBUTTONDOWN && status == EVENT_NOT_HANDLE){
                            setFocus(children->view);
                            status = EVENT_HANDLE;
                        }
                        break;
                    }
                }
                break;
            default:
                break;
        }
    }
    return status;
}

int windowsEventDispacher(GuiEventLoop* loop,SDL_Event* e){
    int status = EVENT_NOT_HANDLE;
    if(loop && e){
        windowsView* windows = (windowsView*)loop->loopContext;
        SDL_Rect* r = &windows->r;
        switch (e->type){
            case SDL_MOUSEBUTTONDOWN:
                if(r->x <= e->button.x && (r->x+r->w)>= e->button.x
                        &&
                        r->y <= e->button.y && (r->y+r->h)>= e->button.y){
                    if(loop->onClick){
                        loop->onClick(loop,e);
                        status = EVENT_HANDLE;
                        if(e->type == SDL_MOUSEBUTTONDOWN)
                                setFocus(windows);
                    }else{
                        GuiView* lastFocus = focusView;
                        if(windows->layoutManager){
                            SDL_Event mouseEvent = *e;
                            mouseEvent.button.x -= r->x;
                            mouseEvent.button.y -= r->y;

                            status = windows->layoutManager->loop->d(windows->layoutManager->loop,&mouseEvent);
                        }
                        if(lastFocus == focusView && e->type == SDL_MOUSEBUTTONDOWN)
                            setFocus(windows);
                    }
                }
                break;

        case SDL_TEXTEDITING:
        case SDL_TEXTINPUT:
            //Redirijo los eventos a el que tiene el foco
            if(focusView != windows && focusView){
                //Distribullo el evento
                status = event::dispachEvent(focusView->loop,e);
            }else{
                if(loop->onTextInput){
                    loop->onTextInput(windows,e->type == SDL_TEXTEDITING?e->edit.text:e->text.text);
                    status = EVENT_HANDLE;
                }
            }
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if(focusView != windows && focusView){
                status = event::dispachEvent(focusView->loop,e);
            }else{
                if(loop->onKey)
                    loop->onKey(windows,&e->key.keysym,e->type == SDL_KEYDOWN);
            }
            break;
        }
    }
    return status;
}

int GuiDefaultDispacher(GuiEventLoop* loop,SDL_Event* e){
    int status = EVENT_NOT_HANDLE;
    if(e){
        GuiView* gui = (GuiView*)loop->loopContext;
        switch (e->type) {
            case SDL_MOUSEBUTTONDOWN:
                if(loop->onClick){
                    loop->onClick(loop,e);
                    status = EVENT_HANDLE;
                }

                break;
            case SDL_TEXTEDITING:
            case SDL_TEXTINPUT:
                if(loop->onTextInput){
                    loop->onTextInput(gui,e->type == SDL_TEXTEDITING?e->edit.text:e->text.text);
                }
                //printf("SDL_TEXTINPUT %s, %i\n",e->text.text,TTF_FontHeight(gui->font));
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if(loop->onKey)
                    loop->onKey(gui,&e->key.keysym,e->type == SDL_KEYDOWN);
                break;
            default:
                break;
        }
    }
    return status;
}
/**
  Funciones de pintado
*/

void GuiViewDraw(renderer::DrawContext context,GuiView* drawableObject){
    if(context && drawableObject){
        //Pinto el color de fondo
        renderer::fillRect(context,&drawableObject->r,&drawableObject->backgroundColor);

        //Pinto el borde
        renderer::drawRect(context,&drawableObject->r,&drawableObject->borderColor);
    }
}
void groupDraw(renderer::DrawContext context,groupView* drawableObject){
    SDL_Point oldOffset = renderer::offsetPoint(context);
    //Modifico el desplazamiento de las coordenadas
    renderer::offsetPointAdd(context,(SDL_Point*)&drawableObject->r);
    for (GuiViewList* child = (GuiViewList*)drawableObject->child;
            child; child = (GuiViewList*)child->next) {
        child->view->renderDrawable(context,child->view);
    }
    renderer::setOffsetPoint(context,&oldOffset);
}

void windowsDraw(renderer::DrawContext context,windowsView* drawableObject){
    GuiViewDraw(context,drawableObject);

    //Guardo el rectangulo de recorte
    SDL_Rect clipRect = renderer::clipRect(context);


    if(drawableObject->r.w && drawableObject->r.h)
        renderer::setClipRect(context,&drawableObject->r);
    if(drawableObject->layoutManager){
        SDL_Point oldOffset = renderer::offsetPoint(context);
        //Modifico el desplazamiento de las coordenadas
        renderer::offsetPointAdd(context,(SDL_Point*)&drawableObject->r);

        drawableObject->layoutManager->renderDrawable(context,drawableObject->layoutManager);

        renderer::setOffsetPoint(context,&oldOffset);

    }
    //Restauro el rectangulo de recorte
    renderer::setClipRect(context,&clipRect);

}
void buttonDraw(renderer::DrawContext context,buttonView* drawableObject){
    SDL_Point oldOffset = renderer::offsetPoint(context);

    GuiViewDraw(context,drawableObject);

    SDL_Color* fontColor = &drawableObject->fontColor;
    SDL_SetRenderDrawColor(renderer::getRenderer(context),fontColor->r,fontColor->g,fontColor->b,fontColor->a);


    //Modifico el desplazamiento de las coordenadas
    renderer::offsetPointAdd(context,(SDL_Point*)&drawableObject->r);
    renderer::drawText(context,drawableObject->font,drawableObject->text);

    renderer::setOffsetPoint(context,&oldOffset);
}
void labelDraw(renderer::DrawContext context,labelView* drawableObject){
    //Guardo el rectangulo de recorte
    SDL_Rect oldClipRect,clipRect;
    oldClipRect = renderer::clipRect(context);

    clipRect = oldClipRect;
    clipRect.x += drawableObject->r.x;
    clipRect.y += drawableObject->r.y;
    clipRect.w = drawableObject->r.w;
    clipRect.h = drawableObject->r.h;


    if(drawableObject->r.w && drawableObject->r.h)
        renderer::setClipRect(context,&clipRect);
    GuiViewDraw(context,drawableObject);

    SDL_Point oldOffset = renderer::offsetPoint(context);

    SDL_Color* fontColor = &drawableObject->fontColor;
    renderer::offsetPointAdd(context,(SDL_Point*)&drawableObject->r);

    SDL_SetRenderDrawColor(renderer::getRenderer(context),fontColor->r,fontColor->g,fontColor->b,fontColor->a);

    renderer::drawText(context,drawableObject->font,labelGetText(drawableObject));

    renderer::setOffsetPoint(context,&oldOffset);

    //Restauro el rectangulo de recorte
    renderer::setClipRect(context,&oldClipRect);
}
void recodListViewDraw(renderer::DrawContext context,recodListView* drawableObject){
    GuiViewDraw(context,drawableObject);
    SDL_Point oldOffset = renderer::offsetPoint(context)
            ,fontOffset = {10,20};
    //Modifico el desplazamiento de las coordenadas
    renderer::offsetPointAdd(context,(SDL_Point*)&drawableObject->r);


    renderer::offsetPointAdd(context,&fontOffset);
    fontOffset.x = 0;
    fontOffset.y += renderer::getHeigthFont(renderer::defaultFont());

    SDL_Point lineP1,lineP2;
    SDL_Color lineColor = {0,0,0,0xff};
    lineP1.y = fontOffset.y-10;
    lineP1.x -= 10;
    lineP2.y = fontOffset.y-10;
    lineP2.x = drawableObject->r.w-10;

    if(drawableObject->selectedIndex>-1){
        SDL_Rect rect = {lineP1.x,lineP1.y,drawableObject->r.w,fontOffset.y};
        rect.y  += (drawableObject->selectedIndex) * rect.h;
        SDL_Color azul = {0,0,0xff,0xff};
        renderer::fillRect(context,&rect,&azul);
    }
    renderer::drawLine(context,&lineP1,&lineP2,&lineColor);

    SDL_SetRenderDrawColor(renderer::getRenderer(context),0,0,0,0xff);
    renderer::drawText(context,nullptr,"Records");
    for(int i = 0;i<5;i++){
        renderer::offsetPointAdd(context,&fontOffset);

        drawableObject->labels[i].renderDrawable(context,&drawableObject->labels[i]);
    }
    renderer::setOffsetPoint(context,&oldOffset);
}

void initGuiView(GuiView *g, event::EventLoop *defaultLoop,typeGui t){
    if(g){
        initView(g,defaultLoop);

        g->guiEvent = (GuiEventLoop*)malloc(sizeof(*g->guiEvent));
        memset(g->guiEvent,0,sizeof(*g->guiEvent));
        g->guiEvent->next = nullptr;
        addNodeBefore(g->guiEvent,g->loop);

        g->loop = g->guiEvent;
        event::setContext(g->guiEvent,g);
        event::setDispacher(g->loop,(event::EventLoopDispacher)&GuiDefaultDispacher);

        g->renderDrawable = (renderer::DrawFunc)&GuiViewDraw;

        g->type = t;
    }

}
/**
  Funciones de Inicializacion
*/
void initWindows(windowsView *w, event::EventLoop *defaultLoop){
    if(w){
        memset(w,0,sizeof(*w));

        initGuiView(w,defaultLoop,typeGui::windows);
        event::setDispacher(w->guiEvent,(event::EventLoopDispacher)&windowsEventDispacher);

        w->renderDrawable = (renderer::DrawFunc)&windowsDraw;

        fontColor.r = fontColor.g = fontColor.b = fontColor.a = 0xff;
        fontColorBacground.r = fontColorBacground.g = fontColorBacground.b = fontColorBacground.a = 0x10;
    }
}


void initLabel(labelView *l, event::EventLoop *defaultLoop){
    if(l){
        memset(l,0,sizeof(*l));
        initGuiView(l,defaultLoop,typeGui::label);

        l->renderDrawable = (renderer::DrawFunc)labelDraw;
        initVector(&l->buff,10,sizeof(char));
    }
}


void initButton(buttonView *b, event::EventLoop *defaultLoop){
    if(b){
        memset(b,0,sizeof(*b));
        initGuiView(b,defaultLoop,typeGui::button);

        b->backgroundColor.a = 255;
        //Color por defecto del boton
        b->backgroundColor.r = b->backgroundColor.g = b->backgroundColor.b = 0xC0;
        //Cambio a un renderer especifico del boton
        b->renderDrawable = (renderer::DrawFunc)&buttonDraw;
        setOnClick(b,&buttonOnClick);
    }
}


void initTextField(textFieldView *txt, event::EventLoop *defaultLoop){
    if(txt){
        memset(txt,0,sizeof(*txt));
        initLabel(txt,defaultLoop);
        txt->type = textField;

        setOnGetFocus(txt,&textField_onGetFocus);
        setOnLostFocus(txt,&textField_onLostFocus);
        setOnKey(txt,(void (*)(GuiView *, SDL_Keysym *, bool))&textField_onKey);
        setOnTextInput(txt,(void (*)(GuiView* v,const char* text))textField_onTextInput);
    }
}
void initGroupView(groupView *group, event::EventLoop *defaultLoop){
    if(group){
        memset(group,0,sizeof(*group));
        initView(group,defaultLoop);
        group->renderDrawable = (renderer::DrawFunc)&groupDraw;

        event::EventLoop* loop = (event::EventLoop*)malloc(sizeof(*loop));
        memset(loop,0,sizeof(*loop));
        addNodeBefore(loop,group->loop);
        group->loop = loop;

        event::setContext(loop,group);
        event::setDispacher(loop,&groupEventDispacher);

    }
}

void initInputBox(inputBox* iBox, event::EventLoop *defaultLoop,TTF_Font* font,SDL_Color fontColor,
                  char* msg,SDL_Point pos,bool (*onAccept)(inputBox* iBox,char* text)){
    if(iBox){
        groupView* grupo = (groupView*)malloc(sizeof(*grupo));
        initWindows(iBox,defaultLoop);
        initLabel(&iBox->lbDescripcion,defaultLoop);
        initTextField(&iBox->txtInput,defaultLoop);
        initButton(&iBox->btnAccept,defaultLoop);
        initGroupView(grupo,defaultLoop);

        iBox->lbDescripcion.userContext = iBox->txtInput.userContext = iBox->btnAccept.userContext = iBox;


        //fuente
        iBox->btnAccept.fontColor = iBox->txtInput.fontColor = iBox->lbDescripcion.fontColor = iBox->fontColor = fontColor;
        iBox->lbDescripcion.font = iBox->txtInput.font = iBox->btnAccept.font = iBox->font = font;
//        if(!font){
//            renderer::defaultFont();
//        }
        //Seteo a negro no transparente el borde
        iBox->btnAccept.borderColor.a = iBox->txtInput.borderColor.a = iBox->borderColor.a = 0xff;


        labelSetText(&iBox->lbDescripcion,msg);
        iBox->btnAccept.text = "Acceptar";
        setPoss(iBox,pos);
        Size lbMinSize = {0,0},textBoxMinSize = {0,0},btnAcceptMinSize = {0,0},iBoxMinSize = {0,0};
        //Calculo  el tamaño necesario para la etiqueta
        TTF_SizeText(font,msg,(int*)&lbMinSize.w,(int*)&lbMinSize.h);
        TTF_SizeText(font,iBox->btnAccept.text,(int*)&btnAcceptMinSize.w,(int*)&btnAcceptMinSize.h);
        setSize(&iBox->lbDescripcion,lbMinSize);

        textBoxMinSize.h = lbMinSize.h+2;
        textBoxMinSize.w = lbMinSize.w/strlen(msg)*20+2;
        //Configuracion del boton
        //btnAcceptMinSize.h = textBoxMinSize.h;
        //btnAcceptMinSize.h = textBoxMinSize.h;


        iBoxMinSize.w = lbMinSize.w>textBoxMinSize.w?
                            lbMinSize.w:textBoxMinSize.w;
        iBoxMinSize.h = lbMinSize.h+textBoxMinSize.h+btnAcceptMinSize.h+5;

        setSize(&iBox->txtInput,textBoxMinSize);
        setSize(&iBox->btnAccept,btnAcceptMinSize);
        setSize(iBox,iBoxMinSize);

        //Pongo el text box centrado horizontalmente y debajo de la etiqueta
        setPoss(&iBox->txtInput,{((int)(lbMinSize.w-textBoxMinSize.w)/2),(int)lbMinSize.h});
        //Pongo el boton debajo del textbox
        setPoss(&iBox->btnAccept,{(int)(lbMinSize.w-btnAcceptMinSize.w)/2,iBox->txtInput.r.y+iBox->txtInput.r.h+2});

        iBox->layoutManager = grupo;
        addChild(iBox->layoutManager,&iBox->lbDescripcion);
        addChild(iBox->layoutManager,&iBox->txtInput);
        addChild(iBox->layoutManager,&iBox->btnAccept);
        //Seteo los callback
        iBox->onAccept = onAccept;
        iBox->btnAccept.onPress = &inputBox_onPress;
        iBox->txtInput.onPressEnter = &inputBox_onEnter;
    }
}

void addChild(groupView *group, GuiView *child){
    if(group && child){
        GuiViewList* node = (GuiViewList*)malloc(sizeof(*node));
        memset(node,0,sizeof(*node));
        node->view = child;
        if(group->child)
            addNodeBefore(group->child,node);
        else
            group->child = node;
    }
}


void setOnClick(GuiView *g, void (*onClick)(GuiEventLoop *, SDL_Event *)){
    if(g){
        g->guiEvent->onClick = onClick;
    }
}


void setUserContext(GuiView *g, void *userContext){
    g->userContext = userContext;
}


void *eventUserContext(GuiEventLoop *loop){
    void* context = nullptr;

    if(loop)
        context = loop->loopContext;

    return context;
}


void setFocus(GuiView *g){
    if(focusView != g){
        if(focusView && focusView->guiEvent->onLostFocus)
            focusView->guiEvent->onLostFocus(focusView);
        focusView = g;
        if(focusView && g->guiEvent->onGetFocus)
            g->guiEvent->onGetFocus(g);
    }
}


void setOnGetFocus(GuiView *g, void (*onGetFocus)(GuiView* v)){
    if(g)
        g->guiEvent->onGetFocus = onGetFocus;
}


void setOnLostFocus(GuiView *g, void (*onLostFocus)(GuiView* v)){
    if(g)
        g->guiEvent->onLostFocus = onLostFocus;
}


void setOnTextInput(GuiView *g, void (*onTextInput)(GuiView *, const char *)){
    if(g)
        g->guiEvent->onTextInput = onTextInput;
}


void setOnKey(GuiView *g, void (*onKey)(GuiView *, SDL_Keysym *, bool)){
    g->guiEvent->onKey = onKey;
}


void initRecodListView(recodListView *view, event::EventLoop *defaultLoop){
    memset(view,0,sizeof(*view));
    initGuiView(view,defaultLoop,listBox);
    view->renderDrawable = (renderer::DrawFunc)&recodListViewDraw;

    for (int i = 0; i < 5; ++i) {
        initLabel(&view->labels[i],defaultLoop);
    }
}


void labelSetText(labelView *l, const char *text){
    size_t len = strlen(text);
    if((len+1)>l->buff.nElementMax){
        resizeVector(&l->buff,len+1);
    };
    l->buff.nElementUsaje = len+1;
    strcpy(getDataVector(l->buff,char),text);
}


const char *labelGetText(labelView *l){
    return getDataVector(l->buff,char)+l->Offset;
}


void recodListViewSetRecord(recodListView *r, recodList *list){
     int i = 0;
    char tmpBuff[0x200];
    for(recodList* tmpList = list;tmpList && i<5;tmpList = (recodList*)tmpList->next,i++){
        sprintf(tmpBuff,"%u %u %s",i+1,tmpList->Points,tmpList->PlayerName);
        labelSetText(&r->labels[i],tmpBuff);
    }
}


void recodListViewSetSelected(recodListView *r, int selectedIndex){
    r->selectedIndex = selectedIndex;
}
