/** @file */
#ifndef RENDERER_H
#define RENDERER_H

#include "snake_global.h"
#include <SDL2/SDL_ttf.h>
/**
* Utilidades para el dibujado\n
* Soporta:\n
*   texto\n
*   Rectangulos con y sin relleno\n
*   ClipRect\n
*   Imagen de fondo\n
*   limpieza y actualizado de pantalla\n
*   Cmptura de pantalla
* @author juan angel
**/

namespace renderer {
    /**
     * @typedef DrawContext
     * @brief Contexto de dibujo
     */
    typedef void* DrawContext;

    /**
     * @typedef DrawFunc
     * @brief Punteroa funcion para dibujar
     */
    typedef void (*DrawFunc)(DrawContext context,struct drawable* drawableObject);
    /**
    * @struct drawable
    * @brief Representa la interface de elementos dibujables
    */
    typedef struct drawable{
        DrawFunc renderDrawable;/**< Funcion que se llama al dibujar el drawable*/
    }drawable;
    /**
    * @struct drawableText
    * @brief Representa la interface de un texto dibujable
    */
    typedef struct drawableText:drawable{
        TTF_Font* font;
        SDL_Color textColor;
        char* text;
    }drawableText;
    /**
     * @brief initDrawableText Inicializa una estructura drawableText
     * @param dText
     * @param font
     * @note no implementado
     */
    void initDrawableText(drawableText* dText,TTF_Font* font);
    //void copyDrawContext(DrawContext c, DrawContext o = 0);
    /**
     * @brief createDrawContext Crea un DrawContext
     * @return el DrawContext creado
     */
    renderer::DrawContext   createDrawContext();
    /**
     * @brief draw Dibuja un drawable
     * @param c Contexto de dibujo
     * @param d Objeto a dibujar
     */
    void draw(DrawContext c, drawable* d);
    /**
     * @brief setOffsetPoint Cambia el desplazamiento de pintado
     *
     * Al cambiarlo, todo lo que se dibuje se dibujara en una posicion relativa al desplazamiento
     * @param c contexto de dibujo
     * @param p Nuevo desplazamiento
     */
    void setOffsetPoint(DrawContext c,SDL_Point* p);
    /**
     * @brief offsetPoint Obtiene el desplazamiento de dibujo actual
     * @param c Contexto de dibujo
     * @return Punto a partir del cual se toman las posiciones
     */
    SDL_Point offsetPoint(DrawContext c);
    /**
     * @brief offsetPointAdd suma al desplazamiento actual, el punto p
     * @param c Contexto de dibujo
     * @param p Punto que se le sumara al desplazamiento
     */
    void offsetPointAdd(DrawContext c,SDL_Point* p);
    /**
     * @brief screenshot Realiza una captura de la ventana
     * @param w Ventana que se captura
     * @return Textura on la captura
     */
    SDL_Texture* screenshot(SDL_Window *w);
    /**
     * @brief setBackgroundImg Limpia la pantalla con una imagen
     * @param context Contexto de dibujo
     * @param img Imagen de fondo
     * @note La imagen de fondo prevlece sobre el color de fondo
     */
    void setBackgroundImg(renderer::DrawContext context, SDL_Texture* img);
    /**
     * @brief setClearColor Cambia el color con el que se borra la pantalla
     * @param c Contexto de dibujo
     * @param color nuevo color de fondo
     * @note si hay una imagen de fondo, este color se ignora
     */
    void setClearColor(renderer::DrawContext c, SDL_Color* color);
    /**
     * @brief clear Limpia la pantalla
     * @param c Contexto de dibujo
     */
    void clear(DrawContext c);
    /**
     * @brief fillRect rellena un rectangulo con el color especificado
     * @param context Contexto de dibujo
     * @param r Rectangulo
     * @param c Color de relleno
     */
    void fillRect(renderer::DrawContext context, SDL_Rect* r, SDL_Color *c);
    /**
     * @brief drawRect Dibuja los bordes de un rectangulo
     * @param c Contexto de dibujo
     * @param r Rectangulo a dibujar
     * @param color Color de los bordes
     */
    void drawRect(DrawContext c, SDL_Rect* r, SDL_Color *color);
    /**
     * @brief drawLine Dibuja una linea entre p1 y p2
     * @param c Contexto de dibujo
     * @param p1 Punto inicio
     * @param p2 Punto final
     * @param color Color de la linea
     */
    void drawLine(DrawContext c, SDL_Point* p1, SDL_Point* p2, SDL_Color *color);
    /**
     * @brief drawText Dibuja un texto
     * @param c Contexto de dibujo
     * @param font Fuente que se aplica al texto
     * @param text Texto a dibujar
     */
    void drawText(renderer::DrawContext c, TTF_Font* font, const char* text);
    /**
     * @brief getHeigthFont Consulta el espacio recomendado para dejar entre linea y linea de texto
     * @param font Objeto fuente
     * @return espacio en pixel
     */
    int getHeigthFont(TTF_Font* font);
    /**
     * @brief defaultFont
     * @param c
     * @return
     */
    TTF_Font* defaultFont();
    /**
     * @brief defaultSetFont
     * @param c
     * @param font
     */
    void setDefaultFont(TTF_Font* font);
    /**
     * @brief clipRect Obtiene el rectangulo de recorte
     * @param c Contexto de dibujo
     * @return El rectangulo de recorte
     */
    SDL_Rect clipRect(DrawContext c);
    /**
     * @brief getRenderer Obtiene el renderer actual
     * @param context Contexto de dibujo
     * @return El renderer actual
     */
    SDL_Renderer* getRenderer(renderer::DrawContext context);
    /**
     * @brief setRenderer Cambia el render con el que se dibuja
     * @param c Contexto de dibujo
     * @param r nuevo renderer
     */
    void setRenderer(DrawContext c, SDL_Renderer* r);
    /**
     * @brief setClipRect Cambia el rectangulo de recorte
     *
     * El rectangulo de recorte hace que lo que este fuera de este rectangulo no se dibuje
     * @param c Contexto de dibujo
     * @param r nuevo rectangulo de recorte
     */
    void setClipRect(DrawContext c, SDL_Rect* r);
    /**
     * @brief sendUpdateWindows Envia el evento de repintado
     */
    void sendUpdateWindows();
}


#endif // RENDERER_H
