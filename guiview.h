/** @file */
#ifndef GUIVIEW_H
#define GUIVIEW_H

#include "view.h"
#include "SDL2/SDL_ttf.h"
typedef enum typeGui{
    group,
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
    //GuiView* pather;
    TTF_Font* font;
    bool border;
    //bool focus;
    SDL_Color fontColor;
    SDL_Color borderColor;
    struct GuiEventLoop* guiEvent;
    void* userContext;
}GuiView;

typedef struct GuiViewList:linkList{
    GuiView* view;
}GuiViewList;

typedef struct groupView:view{
    GuiViewList* child;
}groupView;
typedef struct buttonView:GuiView{
    const char* text;
    bool tolgge;
    void (*onPress)(buttonView* btn);
}buttonView;

typedef struct windowsView:GuiView{
    char* title;
    groupView* layoutManager;
}windowsView;

typedef struct labelView:GuiView{
    ulong Offset;
    vector buff;
}labelView;
typedef struct textFieldView:labelView{
    int cursorPos;
    void (*onPressEnter)(textFieldView* txtFiel);
}textFieldView;
typedef struct inputBox:windowsView{
    labelView lbDescripcion;
    textFieldView txtInput;
    buttonView btnAccept;
    bool (*onAccept)(inputBox* iBox,char* text);
}inputBox;
typedef struct recodListView:GuiView{
    struct recodList* list;
    labelView labels[5];
    int selectedIndex;
}recodListView;

/**
*   metodos de inicializacion de cada elemento de la interface grafica.
*   @param Object //Es el objeto a inicializar
*           EventLoop //es el objeto de manejo de eventos por defecto
*/
/**
 * @brief initWindows Inicializa un objeto ventana
 * @param w
 * @param defaultLoop
 */
void initWindows(windowsView* w, event::EventLoop *defaultLoop);
/**
 * @brief initLabel
 * @param l
 * @param defaultLoop
 */
void initLabel(labelView* l, event::EventLoop *defaultLoop);
/**
 * @brief initButton
 * @param b
 * @param defaultLoop
 */
void initButton(buttonView* b, event::EventLoop *defaultLoop);
/**
 * @brief initTextField
 * @param txt
 * @param defaultLoop
 */
void initTextField(textFieldView* txt, event::EventLoop *defaultLoop);
/**
*   Parametros como los anteriores,
*   @param msg//Mensaje que se mostrara al usuario
*   @param pos//Posicion de la ventana
*   @param onAccept//Callback que se llama al pulsar el boton
*
**/
void initInputBox(inputBox* iBox, event::EventLoop *defaultLoop, TTF_Font *font, SDL_Color fontColor,
                  char* msg, SDL_Point pos, bool (*onAccept)(inputBox* iBox,char* text));
/**
 * @brief initRecodListView
 * @param view
 * @param defaultLoop
 */
void initRecodListView(recodListView* view, event::EventLoop *defaultLoop);
/**
* Agrega hijos al groupView
* @param group//Grupo al que se le grega
* @param child//Vista que se agrega al grupo
**/
void addChild(groupView* group,GuiView* child);
/**
*  Setea un puntero para el uso del usuario en la vista indicada
*/
void setUserContext(GuiView* g,void* userContext);
/**
* Setea en la vista el evento onClick
* @param g//Vista en la que se setea
* @param onClick//Funcion que sera llamada al prodicirse un click
**/
/**
 * @brief setOnClick Asigna una funcion para manejar el evento de click
 * @param g Objeto Vista de GUI
 */
void setOnClick(GuiView* g,void (*onClick)(struct GuiEventLoop* loop,SDL_Event* e));
/**
 * @brief setOnGetFocus Asigna una funcion para manejar el evento de conseguir el foco de teclado
 * @param g Objeto Vista de GUI
 */
void setOnGetFocus(GuiView* g, void (*onGetFocus)(GuiView *));
/**
 * @brief setOnLostFocus Asigna una funcion para manejar el evento perder el foco del teclado
 * @param g Objeto Vista de GUI
 */
void setOnLostFocus(GuiView* g, void (*onLostFocus)(GuiView *));
/**
 * @brief setOnKey Asigna una funcion para manejar el evento de pulsacion de una tecla
 * @param g Objeto Vista de GUI
 */
void setOnKey(GuiView* g, void (*onKey)(GuiView* v,SDL_Keysym* key,bool down));
/**
 * @brief setOnTextInput Asigna una funcion para manejar el evento de entrada de texto
 * @param g Objeto Vista de GUI
 */
void setOnTextInput(GuiView* g,void (*onTextInput)(GuiView* v,const char* text));
/**
 * @brief setFocus Establece la ventana que tiene el foco del teclado
 * @param child Objeto Vista de GUI
 */
void setFocus(GuiView* g);
/**
 * @fn   initGroupView
 * @brief initGroupView Inicializa un grupo de vistas (layaout)
 * @param group Grupo de vistas
 * @param defaultLoop
 */
void initGroupView(groupView *group, event::EventLoop *defaultLoop);
/**
 * @brief labelSetText Cambia el texto de una etiqueta
 * @param l Objeto label
 * @param text Texto a asignar
 */
void labelSetText(labelView* l,const char* text);
/**
 * @brief labelGetText
 * @param l
 * @return
 */
const char* labelGetText(labelView* l);
/**
 * @brief buttonSetText Cambia el texto de un boton
 * @param l
 * @param text Texto a asignar
 */
void buttonSetText(labelView* l,const char* text);
/**
 * @brief recodListViewSetRecord
 * @param r
 * @param list
 */
void recodListViewSetRecord(recodListView* r,struct recodList* list);
void recodListViewSetSelected(recodListView* r,int selectedIndex);
/**
 * @brief eventUserContext Recupera la variable de contexto del eventLoop
 * @param loop Manejador de eventos
 * @return el puntero asignado con setUserContext
 */
void* eventUserContext(struct GuiEventLoop* loop);

#endif // GUIVIEW_H
