#ifndef SNAKE_GLOBAL_H
#define SNAKE_GLOBAL_H

#ifdef _WIN32
    #include "SDL.h"
#else
#ifdef __ANDROID__
    #include "SDL.h"
#else
    #include "SDL2/SDL.h"
#endif
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


typedef struct rectangle:SDL_Rect{
    SDL_Color fillColor;
    SDL_Color borderColor;
}rectangle;



extern uint updateEventID;
extern uint gameOverEventID;

#define snakeSize 10
#define snakeInitialLength 10

#ifdef _DEBUG
#define debug(x) x
#else
#define debug(x)
#endif

#endif // SNAKE_GLOBAL_H
