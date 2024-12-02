#include<stdio.h>
#include<stdlib.h>
#include"include/testool.h"
#include"include/test_graph_basic_struct.h"
#include"include/test_graph_basic_func.h"
#include"include/test_graph_utils.h"

int main(){
  section("Graph_basic (struct)");
  test_graph_basic_struct();
  section("Graph_basic (func)");
  test_graph_basic_func();
  section("Graph_utils (func only)");
  test_graph_utils();

  return 0;
}

