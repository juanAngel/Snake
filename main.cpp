

#include "snake_global.h"
#include "renderer.h"

#include "gameview.h"
#include "guiview.h"

#include <SDL/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <time.h>

#include "randontree.h"
#include "records.h"
#include <SDL2/SDL_mixer.h>

uint updateEventID;
uint gameOverEventID;

const char* fontName = "font/lazy.ttf";

typedef struct Context{
    //Indica si es necesario redibujar
    bool needRedraw;
    //Render y ventana
    SDL_Window* window;
    //SDL_Renderer* renderer;
    //Guarda el recuadro de la pantalla
    SDL_Rect display;
    //Draw
    renderer::DrawContext drawContext;
    GameView* gameView;
    GuiView* guiView;
    inputBox* inputPlayer;
    recodListView* records;
    char* playerName;
    view* currentView;
    event::EventLoop defaultLoop;
    TTF_Font* defaultFont;
    TTF_Font* defaultTitleFont;
    Mix_Music* gameOverSound;
    bool exitApp;
}Context;

void initContext(Context* c,int x,int y,int w,int h);
void uninitContext(Context* c);
int DefaultDispacher(event::EventLoop *loop, SDL_Event *e);
void retry_onClick(struct GuiEventLoop* loop,SDL_Event* e);
bool impPlayer_onAccept(inputBox* iBox,char* text);
void onGameOver(Context* c);

void initGame(Context* c);
void uinitGame(Context* c);
void initGui(Context* c);
void uinitGui(Context* c);

int main(int /*argc*/, char* /*argv*/[]){
    Context context;
    forestRandon forest;

    //Inicializo el contexto
    initContext(&context,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480);

    //Intento abrir las fuentes
    context.defaultFont = TTF_OpenFont(fontName,25);
    context.defaultTitleFont = TTF_OpenFont(fontName,50);
    if(!context.defaultFont && context.defaultTitleFont){
        printf("No se pudo cargar las fuentes\n");
        exit(-1);
    }
    renderer::setDefaultFont(context.defaultFont);

    initGame(&context);
    initGui(&context);

    //Registro los eventos
    updateEventID = SDL_RegisterEvents(2);
    gameOverEventID = updateEventID+1;

    //Pongo la vista por defecto
    context.currentView = context.inputPlayer;

    forest.Display = &context.display;
    forest.treeColor = {0,0x70,0,0xff};
    forest.backgrundColor = {0xFF,0xFF,0x80,0xff};
    forest.gridSize = {5,5};

    drawForestRandon(context.drawContext,&forest);

    renderer::setBackgroundImg(context.drawContext,renderer::screenshot(context.window));

    //Arranco la vista
    runView(&context.currentView,&context.exitApp);

    uninitContext(&context);
    return 0;
}
void initGame(Context* c){
    Snake* snake;
    //Creo una vista de juego
    c->gameView = (GameView*)malloc(sizeof(*c->gameView));
    initGameView(c->gameView,&c->display,&c->defaultLoop);

    c->gameView->defaultFont = c->defaultTitleFont;

    //Configuro la serpiente para un paso de 5 y direccion arriba
    snake = &c->gameView->drawSnake.snake;
    snake->speed = snakeSize;
    snake->vecDirector = direcUp;

}
void resetGame(Context* c){
    resetSnake(&c->gameView->drawSnake,&c->display);
    c->gameView->start = false;
}

void uinitGame(Context* /*c*/){

}
void initGui(Context* c){
    SDL_Color blanco = {0xff,0xff,0xff,0xff};
    SDL_Color negro = {0x0,0x0,0x0,0xff};
    SDL_Color gris = {0xC0,0xC0,0xC0,0xFF};
    SDL_Color transparente = {0x0,0x0,0x0,0};
    windowsView* windows = (windowsView*)malloc(sizeof(*windows));
    buttonView* btnRetry = (buttonView*)malloc(sizeof(*btnRetry));
    groupView* grupo = (groupView*)malloc(sizeof(*grupo));
    labelView* label = (labelView*)malloc(sizeof(*label));
    textFieldView* txtUserName = (textFieldView*)malloc(sizeof(*txtUserName));
    inputBox* impPlayer = (inputBox*)malloc(sizeof(*impPlayer));
    recodListView* record = (recodListView*)malloc(sizeof(*record));

    c->gameOverSound = Mix_LoadMUS("sound/GameOver.ogg");

    initWindows(windows,&c->defaultLoop);
    initButton(btnRetry,&c->defaultLoop);
    initGroupView(grupo,&c->defaultLoop);
    initLabel(label,&c->defaultLoop);
    initTextField(txtUserName,&c->defaultLoop);
    initRecodListView(record,&c->defaultLoop);
    initInputBox(impPlayer,&c->defaultLoop,c->defaultFont,blanco,(char*)"Introduce el nombre del jugador",{c->display.w/2-200,c->display.h/2-25},nullptr);

    c->guiView = windows;
    c->inputPlayer = impPlayer;
    c->records = record;


    btnRetry->font = c->defaultTitleFont;
    btnRetry->text = "Reintentar";

    btnRetry->r = {c->display.w/2-125,40,0,0};
    btnRetry->r.w = 250;
    btnRetry->r.h = 50;

    //Seteamos los contextos
    setUserContext(btnRetry,c);
    setUserContext(impPlayer,c);
    //Seteamos los callback

    setOnClick(btnRetry,&retry_onClick);
    impPlayer->onAccept = &impPlayer_onAccept;

    btnRetry->fontColor = negro;

    windows->layoutManager = grupo;
    windows->font = c->defaultTitleFont;
    windows->backgroundColor = transparente;
    setSize(windows,{(ulong)c->display.w,(ulong)c->display.h});

    label->r = {c->display.w/2-100,c->display.h/4-25,0,0};
    labelSetText(label,"Game Over");
    label->font = c->defaultTitleFont;
    label->fontColor = blanco;

    record->r = label->r;
    record->r.y += 50;
    record->r.w = 210;
    record->r.h = 300;
    record->backgroundColor = gris;
/*
    txtUserName->font = label->font = c->defaultTitleFont;
    txtUserName->fontColor = blanco;
    txtUserName->borderColor = blanco;
    txtUserName->r = {0,0,200,50};*/

    impPlayer->backgroundColor = gris;
    impPlayer->fontColor = blanco;


    addChild(windows->layoutManager,btnRetry);
    addChild(windows->layoutManager,label);
    addChild(windows->layoutManager,record);
}
void uinitGui(Context* /*c*/){

}
void setPlayerName(Context* c, const char* name){
    if(name){
        int len = strlen(name);
        //Ajusto el buffer
        c->playerName  = (char*)(c->playerName?
                    realloc(c->playerName,(len+1)*sizeof(*name))
                  :malloc((len+1)*sizeof(*name)));
        strcpy(c->playerName,name);
    }
}

void initContext(Context* c, int x, int y, int w, int h){
    memset(c,0,sizeof(*c));
    srand(time(nullptr));

    c->display.x = x;
    c->display.y = y;
    c->display.w = w;
    c->display.h = h;

    c->needRedraw = true;
    c->exitApp = false;
    //Inicializo el defalutLoop
    c->defaultLoop.d = &DefaultDispacher;
    c->defaultLoop.next = nullptr;
    c->defaultLoop.loopContext = c;

    //TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    //Inicializo ttf
    TTF_Init();


    c->window = SDL_CreateWindow(
            "An SDL2 window",                  // window title
            x,           // initial x position
            y,           // initial y position
            w,                               // width, in pixels
            h,                               // height, in pixels
            SDL_WINDOW_OPENGL                  // flags - see below
        );
    SDL_GetWindowSize(c->window,&c->display.w,&c->display.h);
    c->drawContext = renderer::createDrawContext();
    SDL_Renderer* r = SDL_CreateRenderer(c->window,0,SDL_RENDERER_ACCELERATED);

    renderer::setRenderer(c->drawContext,r);
}
void uninitContext(Context* c){
    if(c){
        if(renderer::getRenderer(c->drawContext))
            SDL_DestroyRenderer(renderer::getRenderer(c->drawContext));
        if(c->window)
            SDL_DestroyWindow(c->window);
    }
    SDL_Quit();
}
int DefaultDispacher(event::EventLoop *loop, SDL_Event *e){
    int result = 0;
    if(loop){
        Context* context = (Context*)loop->loopContext;
        //Seteamos como manejado
        result = EVENT_HANDLE;
        if(e){
            //Manejo de los eventos por defecto
            switch (e->type){
                case SDL_QUIT:
                    context->exitApp = true;
                    break;
                case SDL_WINDOWEVENT:
                    if(e->window.event == SDL_WINDOWEVENT_RESTORED){
                        printf("windows restored\n");
                        renderer::sendUpdateWindows();
                    }
                    break;
                default:
                    result = EVENT_NOT_HANDLE;
                    if(updateEventID == e->type){
                        context->needRedraw = true;
                    }else if(gameOverEventID == e->type){
                        onGameOver(context);
                    }else{
                        printf("unkanow event %i\n",e->type);
                    }
            }
        }else{
            //si no hay eventos entramos en modo IDL
            if(context->needRedraw){
                renderer::draw(context->drawContext,context->currentView);
                context->needRedraw = false;
            }else{
                SDL_Delay(5);
            }
        }
    }
    return result;
}

void onGameOver(Context* c){
    //Cambiamos a la gui
    c->currentView = c->guiView;
    printf("GameOver\n");
    renderer::sendUpdateWindows();

    recodList* records;
    int recordIndex;

    records = manageRecord(loadRecord(),getNAppletMeals(&c->gameView->drawSnake),c->playerName,&recordIndex);
    if(recordIndex != -1){
        printf("nuevo record\n\tjugador %s\n\tpuntuacion %u\n\tposicion %u\n",c->playerName,getNAppletMeals(&c->gameView->drawSnake),recordIndex);
        saveRecord(records);
    }
    recodListViewSetRecord(c->records,records);
    recodListViewSetSelected(c->records,recordIndex);
    freeRecordList(records);

    //reiniciamos el juego
    resetGame(c);
    Mix_PlayMusic(c->gameOverSound,0);
}

void retry_onClick(struct GuiEventLoop* loop,SDL_Event* /*e*/){
    if(loop){
       buttonView* boton = (buttonView*)eventUserContext(loop);
       Context* c = (Context*)boton->userContext;

       c->currentView = c->gameView;


       SDL_Event e;
       e.type = updateEventID;

       SDL_PushEvent(&e);
    }
    printf("reintentar pulsado\n");
}
bool impPlayer_onAccept(inputBox* iBox,char* text){
    printf("impPlayer_onAccept\n");
    if(iBox && text && strlen(text)){
        Context* c = (Context*)iBox->userContext;
        setPlayerName(c,text);
        c->currentView = c->gameView;

        renderer::sendUpdateWindows();
    }
    return false;
}
