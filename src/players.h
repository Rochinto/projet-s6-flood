#ifndef PLAYERS_H
#define PLAYERS_H

#include <stdlib.h>
#include <dlfcn.h>
#include "color.h"
#include "graph.h"

/**
 * @brief A structure containing the functions of a player's library
 */
struct dlplayer{
    void * handle;
    ///< A pointer to the opened library
    void (*initialize)(size_t, struct graph_t*, enum color_t*, struct color_set_t*);
    ///< A function that initializes the player
    struct move_t (*play)(struct move_t);
    ///< A function that computes the moves of the player
    void (*finalize)();
    ///< A function that announces the end of the game to the player, and cleans up the memory he may have been using.
    char const* (*get_player_name)();
    ///< A function that accesses to the information of a player
};

/**
 * @brief Loads the functions of the player in a structure
 *
 * @param filename The name of the player's library
 * @return p A structure containing the functions of the player
 *
 * PRECOND: 
 * - filename must be a library containing the four functions of the struct dlplayer
 */
struct dlplayer player_load(char * filename);

/**
 * @brief Closes the library of the player p
 *
 * @param p A structure containing the functions of the player
 *
 * POSTCOND: 
 * - the dynamically opened library of the player p should be closed
 */
void player_close(struct dlplayer * p);

#endif // PLAYERS_H
