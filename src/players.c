#include <stdio.h>
#include <stdlib.h>
#include "players.h"

static void * load_symbol(void * handle, char * filename, char * symbol){
    void * sym = dlsym(handle, symbol);

    if (sym == NULL){
        printf("Cannot load symbol %s from file %s \n", symbol, filename);
        exit(EXIT_FAILURE);
    }

    return sym;
}

struct dlplayer player_load(char * filename){
    struct dlplayer p;
    p.handle = dlopen(filename, RTLD_LAZY);

    if (p.handle == NULL){
        printf("Cannot open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    p.initialize = load_symbol(p.handle, filename, "initialize");
    p.play = load_symbol(p.handle, filename, "play");
    p.finalize = load_symbol(p.handle, filename, "finalize");
    p.get_player_name = load_symbol(p.handle, filename, "get_player_name");
    return p;
}

void player_close(struct dlplayer * p){
    dlclose(p->handle);
    p->handle = NULL;
    p->initialize = NULL;
    p->play = NULL;
    p->finalize = NULL;
    p->get_player_name = NULL;
}
