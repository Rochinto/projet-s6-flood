
#include "include/test_graph_basic_struct.h"
#include "../src/graph_struct.h"

int test_graph__empty(){
    int n = 4;
    struct graph *g = graph__empty(n);

    RET_TEST(g->is_compressed == 0);
    RET_TEST(g->graph->num_vertices == n);
    RET_TEST(g->graph->positions[0] == 0);
    RET_TEST(g->graph->positions[1] == 0);
    for (int i = 0; i < n; i++) RET_TEST(g->colors[i] == NO_COLOR);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) RET_TEST(gsl_spmatrix_uint_get(g->graph->t, i, j) == 0);
    }
    graph__free(g);

    g = graph__empty(0);
    RET_TEST(g == NULL);

    return 0;
}

int test_graph__add_edge(){
    int n = 4;
    struct graph *g = graph__empty(n);

    graph__add_edge(g, 0, 1);
    graph__add_edge(g, 1, 2);
    graph__add_edge(g, 2, 3);
    graph__add_edge(g, 3, 0);

    for (int i = 0; i < n; i++){
        RET_TEST(gsl_spmatrix_uint_get(g->graph->t, i, (i-1+n)%n) == 1);
        RET_TEST(gsl_spmatrix_uint_get(g->graph->t, i, (i+1+n)%n) == 1);
        RET_TEST(gsl_spmatrix_uint_get(g->graph->t, i, (i+2+n)%n) == 0);
        RET_TEST(gsl_spmatrix_uint_get(g->graph->t, (i-1+n)%n, i) == 1);
        RET_TEST(gsl_spmatrix_uint_get(g->graph->t, (i+1+n)%n, i) == 1);
        RET_TEST(gsl_spmatrix_uint_get(g->graph->t, (i+2+n)%n, i) == 0);
        RET_TEST(gsl_spmatrix_uint_get(g->graph->t, i, i) == 0);
    }

    RET_TEST(graph__add_edge(g, 0, 0) < 0);
    RET_TEST(gsl_spmatrix_uint_get(g->graph->t, 0, 0) == 0);
    RET_TEST(graph__add_edge(g, n, 0) < 0);
    RET_TEST(graph__add_edge(g, 0, n) < 0);

    graph__free(g);
    return 0;
}

int test_graph__set_color(){
    int n = 4;
    struct graph *g = graph__empty(n);
    
    graph__set_color(g, 0, RED);
    graph__set_color(g, 1, BLUE);
    graph__set_color(g, 2, BLUE);
    graph__set_color(g, 3, GREEN);

    RET_TEST(g->colors[0] == RED);
    RET_TEST(g->colors[1] == BLUE);
    RET_TEST(g->colors[2] == BLUE);
    RET_TEST(g->colors[3] == GREEN);

    RET_TEST(graph__set_color(g, n, BLUE) < 0);
    RET_TEST(graph__set_color(g, 0, MAX_COLOR) < 0);
    RET_TEST(graph__set_color(g, 0, NO_COLOR) < 0);

    graph__free(g);
    return 0;
}


int test_graph__compress(){
    int n = 4;
    struct graph *g = graph__empty(n);

    RET_TEST(g->is_compressed == 0);
    RET_TEST(g->graph->t->sptype == GSL_SPMATRIX_COO);

    graph__compress(g);

    RET_TEST(g->is_compressed == 1);
    RET_TEST(g->graph->t->sptype == GSL_SPMATRIX_CSR);
    
    graph__free(g);
    return 0;
}

int test_graph__set_positions(){
    int n = 4;
    struct graph *g = graph__empty(n);

    graph__set_positions(g,1,2);
    RET_TEST(g->graph->positions[0] == 1);
    RET_TEST(g->graph->positions[1] == 2);

    graph__set_positions(g,3,0);
    RET_TEST(g->graph->positions[0] == 3);
    RET_TEST(g->graph->positions[1] == 0);

    RET_TEST(graph__set_positions(g,n,0) < 0);
    RET_TEST(g->graph->positions[0] == 3);
    RET_TEST(g->graph->positions[1] == 0);

    RET_TEST(graph__set_positions(g,0,n) < 0);
    RET_TEST(g->graph->positions[0] == 3);
    RET_TEST(g->graph->positions[1] == 0);

    graph__free(g);
    return 0;
}

int test_graph_basic_struct(){
    test(test_graph__empty(), "graph__empty");
    test(test_graph__add_edge(), "graph__add_edge");
    test(test_graph__set_color(), "graph__set_color");
    test(test_graph__compress(), "graph__compress");
    test(test_graph__set_positions(), "graph__set_positions");
}
