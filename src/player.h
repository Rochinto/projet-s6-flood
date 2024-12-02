#ifndef _FLOOD_PLAYER_H_
#define _FLOOD_PLAYER_H_

#include "color.h"
#include "graph.h"

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name();

/* Player initialization
 * PARAM:
 * - id        : the id assigned to the player
 * - graph     : the graph where the game is played
 * PRECOND:
 * - `id` is either 0 or 1
 * - `graph` is a heap-allocated copy of the graph where the game is
 *   played, that must be freed in the end
 * - `colors` is a heap allocated copy array associating to each vertex
 *   id a color, that must be freed in the end
 * - `forbidden` is an array of color sets, one for each player,
 *   specifying the forbidden colors for each player (one per id)
 * - initialize has never been called before
 */
void initialize(size_t id, struct graph_t* graph,
                enum color_t* colors,
                struct color_set_t forbidden[NUM_PLAYERS]);

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player. If this is the
 *   first move of the game, `previous_move` is equal to NO_COLOR.
 * RETURNS: - the next move for the player.
 */
struct move_t play(struct move_t previous_move);

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize();

#endif // _FLOOD_PLAYER_H_
