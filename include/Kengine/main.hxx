#include "Kengine/engine.hxx"
#include "Kengine/game.hxx"

#define RUN_GAME(create_func, name)                                            \
    int main()                                                                 \
    {                                                                          \
        if (Kengine::run(&create_func, name))                                  \
            return EXIT_SUCCESS;                                               \
        else                                                                   \
            return EXIT_FAILURE;                                               \
    }
