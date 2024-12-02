
#include "include/test_graph_basic_func.h"

int test_graph__empty2(){
    //Test func
    int n = 4;
    struct graph *g = graph__empty(n);
    graph__compress(g);

    RET_TEST(graph__get_number_of_vertices(g) == n);
    struct doublon positions = graph__get_positions(g);
    RET_TEST(positions.n[0] == 0);
    RET_TEST(positions.n[1] == 0);
    for (int i = 0; i < n; i++){
        RET_TEST(graph__get_color(g, i) == NO_COLOR);
        RET_TEST(graph__get_neighbors(g, i).size == 0);
    }

    graph__free(g);
    return 0;
}

int test_graph__get_neighbors(){
    int n = 4;
    struct graph *g = graph__empty(n);

    graph__add_edge(g, 0, 1);
    graph__add_edge(g, 1, 2);
    graph__add_edge(g, 2, 3);
    graph__add_edge(g, 3, 0);

    graph__compress(g);

    for (int i = 0; i < n; i++){
        struct size_t_list neighbors = graph__get_neighbors(g, i);
        RET_TEST(neighbors.size == 2);
        RET_TEST((neighbors.list[0]==(i-1+n)%n && neighbors.list[1]==(i+1+n)%n)\
        || (neighbors.list[1]==(i-1+n)%n && neighbors.list[0]==(i+1+n)%n));
    }

    struct size_t_list neighbors = graph__get_neighbors(g, n);
    RET_TEST(neighbors.size == 0);
    RET_TEST(neighbors.list == NULL);

    graph__free(g);
    return 0;
}

int test_graph__get_color(){
    int n = 4;
    struct graph *g = graph__empty(n);
    
    graph__set_color(g, 0, RED);
    graph__set_color(g, 1, BLUE);
    graph__set_color(g, 2, BLUE);
    graph__set_color(g, 3, GREEN);

    RET_TEST(graph__get_color(g, 0) == RED);
    RET_TEST(graph__get_color(g, 1) == BLUE);
    RET_TEST(graph__get_color(g, 2) == BLUE);
    RET_TEST(graph__get_color(g, 3) == GREEN);

    graph__free(g);
    return 0;
}

int test_graph__get_positions(){
    int n = 4;
    struct graph *g = graph__empty(n);
    
    graph__set_positions(g,1,2);
    struct doublon positions = graph__get_positions(g);
    RET_TEST(positions.n[0] == 1);
    RET_TEST(positions.n[1] == 2);

    graph__set_positions(g,3,0);
    positions = graph__get_positions(g);
    RET_TEST(positions.n[0] == 3);
    RET_TEST(positions.n[1] == 0);

    graph__free(g);
    return 0;
}

int test_graph__get_number_of_vertices(){

    struct graph *g1 = graph__empty(1);
    RET_TEST(graph__get_number_of_vertices(g1) == 1);
    graph__free(g1);

    struct graph *g2 = graph__empty(42);
    RET_TEST(graph__get_number_of_vertices(g2) == 42);
    graph__free(g2);
    
    return 0;
}

int test_graph_basic_func(){
    test(test_graph__get_neighbors(), "graph__get_neighbors");
    test(test_graph__get_color(), "graph__get_color");
    test(test_graph__get_positions(), "graph__get_positions");
    test(test_graph__get_number_of_vertices(), "graph__get_number_of_vertices");
    test(test_graph__empty2(), "graph__empty");
}


int test_graph__compress();