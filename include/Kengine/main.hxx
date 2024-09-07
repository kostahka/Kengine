#pragma once

#include "Kengine/engine.hxx"
#include "Kengine/game.hxx"

#ifdef __ANDROID__
 #define main SDL_main
 #define RUN_GAME(create_func, name)                                           \
     extern "C" int SDL_main(int argc, char* argv[])                                                            \
     {                                                                         \
         if (Kengine::run(&create_func, name))                                 \
             return EXIT_SUCCESS;                                              \
         else                                                                  \
             return EXIT_FAILURE;                                              \
     }
#else
 #define RUN_GAME(create_func, name)                                           \
     int main()                                                                \
     {                                                                         \
         if (Kengine::run(&create_func, name))                                 \
             return EXIT_SUCCESS;                                              \
         else                                                                  \
             return EXIT_FAILURE;                                              \
     }
#endif
