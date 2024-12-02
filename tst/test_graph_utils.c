
#include "include/test_graph_utils.h"

int test_create_square_graph(){
    int n = 3;
    struct graph *g = create_square_graph(n);
    
    /*struct 

    RET_TEST(winnable[ORANGE] == 0);
    RET_TEST(winnable[VIOLET] == 0);
    RET_TEST(winnable[CYAN] == 0);
    RET_TEST(winnable[PINK] == 0);
    */
    graph__free(g);
    return 0;
}

int test_number_vertices_winnable_from_player(){
    int n = 4;
    struct graph *g = create_square_graph(n);
    graph__compress(g);
    enum color_t colors[16] = {BLUE,  BLUE,   GREEN,  GREEN,
                               BLUE,  YELLOW, YELLOW, GREEN,
                               BLUE,  RED,    BLUE,   GREEN,
                               GREEN, GREEN,  GREEN,  GREEN};
    for(size_t vertex = 0; vertex < n*n; vertex++) graph__set_color(g, vertex, colors[vertex]);
    //display_graph_as_square(g, n);
    int winnable[MAX_COLOR] = {};

    number_vertices_winnable_from_player(g, 0, winnable);
    RET_TEST(winnable[BLUE] == 0);
    RET_TEST(winnable[RED] == 1);
    RET_TEST(winnable[GREEN] == 8);
    RET_TEST(winnable[YELLOW] == 2);

    RET_TEST(winnable[ORANGE] == 0);
    RET_TEST(winnable[VIOLET] == 0);
    RET_TEST(winnable[CYAN] == 0);
    RET_TEST(winnable[PINK] == 0);

    number_vertices_winnable_from_player(g, 1, winnable);
    RET_TEST(winnable[BLUE] == 5);
    RET_TEST(winnable[RED] == 1);
    RET_TEST(winnable[GREEN] == 0);
    RET_TEST(winnable[YELLOW] == 2);

    graph__free(g);
    return 0;
}

int test_is_touching_opponent(){
    int n = 4;
    struct graph *g = create_square_graph(n);
    graph__compress(g);
    enum color_t colors[16] = {BLUE,  BLUE,   GREEN,  GREEN,
                               BLUE,  YELLOW, YELLOW, GREEN,
                               BLUE,  RED,    BLUE,   GREEN,
                               GREEN, GREEN,  GREEN,  GREEN};
    for(size_t vertex = 0; vertex < n*n; vertex++) graph__set_color(g, vertex, colors[vertex]);

    RET_TEST(is_touching_opponent(g));

    enum color_t colors2[16] = {BLUE,  BLUE,   GREEN,  GREEN,
                                BLUE,  YELLOW, YELLOW, RED,
                                BLUE,  RED,    BLUE,   GREEN,
                                RED,   GREEN,  GREEN,  GREEN};
    for(size_t vertex = 0; vertex < n*n; vertex++) graph__set_color(g, vertex, colors2[vertex]);

    RET_TEST(!is_touching_opponent(g));

    graph__free(g);
    return 0;
}

int test_coloring_graph_from_player(){
    int n = 4;
    struct graph *g = create_square_graph(n);
    graph__compress(g);
    enum color_t colors[16] =       {BLUE,  BLUE,   GREEN,  GREEN,
                                     BLUE,  YELLOW, YELLOW, RED,
                                     BLUE,  RED,    BLUE,   GREEN,
                                     RED,   YELLOW,  GREEN, GREEN};
    for(size_t vertex = 0; vertex < n*n; vertex++) graph__set_color(g, vertex, colors[vertex]);

    coloring_graph_from_player(g, 0, RED);

    enum color_t colors_test[16] = {RED,  RED,    GREEN,  GREEN,
                                    RED,  YELLOW, YELLOW, RED,
                                    RED,  RED,    BLUE,   GREEN,
                                    RED,  YELLOW, GREEN,  GREEN};
    for(size_t vertex = 0; vertex < n*n; vertex++) RET_TEST(graph__get_color(g, vertex) == colors_test[vertex]);

    coloring_graph_from_player(g, 1, RED);

    enum color_t colors_test2[16] = {RED,  RED,    GREEN,  GREEN,
                                     RED,  YELLOW, YELLOW, RED,
                                     RED,  RED,    BLUE,   RED,
                                     RED,  YELLOW, RED,    RED};
    for(size_t vertex = 0; vertex < n*n; vertex++) RET_TEST(graph__get_color(g, vertex) == colors_test2[vertex]);

    coloring_graph_from_player(g, 0, YELLOW);

    enum color_t colors_test3[16] = {YELLOW,  YELLOW, GREEN,  GREEN,
                                     YELLOW,  YELLOW, YELLOW, RED,
                                     YELLOW,  YELLOW, BLUE,   RED,
                                     YELLOW,  YELLOW, RED,    RED};
    for(size_t vertex = 0; vertex < n*n; vertex++) RET_TEST(graph__get_color(g, vertex) == colors_test3[vertex]);

    coloring_graph_from_player(g, 1, YELLOW);

    enum color_t colors_test4[16] = {YELLOW,  YELLOW, GREEN,  GREEN,
                                     YELLOW,  YELLOW, YELLOW, YELLOW,
                                     YELLOW,  YELLOW, BLUE,   YELLOW,
                                     YELLOW,  YELLOW, YELLOW, YELLOW};
    for(size_t vertex = 0; vertex < n*n; vertex++) RET_TEST(graph__get_color(g, vertex) == colors_test4[vertex]);

    graph__free(g);
    return 0;
}


int test_graph_utils(){
    test(test_create_square_graph(), "create_square_graph");
    test(test_number_vertices_winnable_from_player(), "number_vertices_winnable_from_player");
    test(test_coloring_graph_from_player(), "test_coloring_graph_from_player");
    test(test_is_touching_opponent(), "test_is_touching_opponent");
}