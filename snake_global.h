/** @file */
#ifndef SNAKE_GLOBAL_H
#define SNAKE_GLOBAL_H

#if defined(_WIN32) || defined(__ANDROID__)
    #include <SDL.h>
#else
    #include "SDL2/SDL.h"
#endif
#include <stdio.h>

#ifdef __ANDROID__
#include <android/log.h>
#ifdef printf
#undef printf
#endif
#ifdef vprintf
#undef vprintf
#endif
#define printf(args...) __android_log_print(ANDROID_LOG_INFO, "libtcod", ## args)
#define vprintf(args...) __android_log_vprint(ANDROID_LOG_INFO, "libtcod", ## args)

#ifdef assert
#undef assert
#endif
#define assert(cond) if(!(cond)) __android_log_assert(#cond, "libtcod", "assertion failed: %s", #cond)
#endif

typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef struct Size{
    ulong w,h;
}Size;

extern uint updateEventID;      /**< Identificador de el evento de actualizar pantalla */
extern uint gameOverEventID;    /**< Identificador de el evento de Partida terminada */

/**
 * Tamaño del lado que compone los bloques de la serpiente
 * */
#define snakeSize 15
/**
 * Numero de bloques iniciales de la serpiente
 * */
#define snakeInitialLength 10

#ifdef _DEBUG
#define debug(x) x
#else
#define debug(x)
#endif

#endif // SNAKE_GLOBAL_H
