GXX_FLAGS_OBJ= gcc -c -std=c99 -g -O0 -fPIC
GXX_FLAGS_EXE= gcc -std=c99 -g

COVERAGE_TESTS= -lgconv
TEST_FILES=./tst
SOURCE_FILES=./src
TEST_FLAGS= -fprofile-arcs -ftest-coverage

BUILD=./build

GSL_PATH?=gsl
SDL?=0
#VALGRIND?=valgrind --xml=yes -xml-file=valgrind.xml
execution_by_lgcov= gcov -n

DYNAMIC_LINKED= -lgsl -lgslcblas -lm
GSL_LIBRARY= -lgsl -lgslcblas -lm
GSL_FLAGS= -I$(GSL_PATH)/include -L$(GSL_PATH)/lib -lgsl -lgslcblas -lm

TEST_OBJ_FILES= testool.o test_graph.o test_graph_basic_struct.o test_graph_basic_func.o test_graph_utils.o
ALL_OBJ_FILES= players.o server.o graph_basic.o graph_utils.o $(TEST_OBJ_FILES)

#---------------------------------------------------------------------------------------------

.PHONY: build install

all: build

createBuild:
	mkdir -p build


build: createBuild $(ALL_OBJ_FILES) graph server player_clever player_glouton player_valid test

state_ON: clean path.sh build install
	./path.sh
	./install/server -m 100 ./install/libplayer_random.so ./install/libplayer_valid.so 

state_OFF: clean build install 
	./install/server -m 64 ./install/libplayer_random.so ./install/libplayer_max.so

#---------------------------------------------------------------------------------------------

#
#make clean ; SDL=1 ; make install
#./install/server -m 100 ./install/libplayer_valid_so ./libplayer_max.so | ./sdl -m (pour visualiser l'execution en sdl pour tous les autres membres du groupe).
#


#------------------------------------------------------------------------

install: #
	cp ./build/alltests ./install/alltests
	cp ./build/server ./install/server
	cp ./build/*.so ./install/

#----------------------------------

graph: tst/tst_debut.c src/graph.h
	gcc -std=c99 $< $(GSL_FLAGS) -o ./build/$@
graph_impl.o: src/graph_impl.c src/graph_impl.h
	gcc -std=c99 -c $< $(GSL_FLAGS) -o ./build/$@

#----------------------------------


#----------------------------------

server:
	$(GXX_FLAGS_EXE) -O0 ${TEST_FLAGS} $(BUILD)/server.o $(BUILD)/players.o $(BUILD)/graph_utils.o $(BUILD)/graph_basic.o $(GSL_FLAGS) -ldl -o ./build/$@

#----------------------------------

players.o: src/players.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} $< $(GSL_FLAGS) -ldl -o ./build/$@
player_impl.o: src/player_impl.c
	gcc -std=c99 -c $< $(GSL_FLAGS) -o ./build/$@

#----------------------------------

server.o: src/server.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} $< $(GSL_FLAGS) -DSDL=$(SDL) -ldl -o ./build/$@

#----------------------------------
player_valid: ${SOURCE_FILES}/player_valid.c $(BUILD)/graph_basic.o $(BUILD)/graph_utils.o
	$(GXX_FLAGS_EXE) -O0 ${TEST_FLAGS} $(GSL_FLAGS) -fPIC -shared -o $(BUILD)/libplayer_valid.so $^

player_clever: ${SOURCE_FILES}/player_clever.c $(BUILD)/graph_basic.o $(BUILD)/graph_utils.o
	$(GXX_FLAGS_EXE) -O0 ${TEST_FLAGS} $(GSL_FLAGS) -fPIC -shared -o $(BUILD)/libplayer_clever.so $^

player_random: ${SOURCE_FILES}/player_random.c $(BUILD)/graph_basic.o $(BUILD)/graph_utils.o
	 #$(#GXX_FLAGS_EXE) -O0 #${#TEST_FLAGS} #$(#GSL_FLAGS) -fPIC -shared -o $(#BUILD)/libplayer_random.so #$#^

player_glouton: ${SOURCE_FILES}/player_glouton.c $(BUILD)/graph_basic.o $(BUILD)/graph_utils.o
	$(GXX_FLAGS_EXE) -O0 ${TEST_FLAGS} $(GSL_FLAGS) -fPIC -shared -o $(BUILD)/libplayer_max.so $^
#----------------------------------

graph_basic.o: src/graph_basic.h src/graph.h src/color.h src/graph_basic.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} src/graph_basic.c $(GSL_FLAGS) -o $(BUILD)/graph_basic.o

graph_utils.o: src/graph_utils.h src/graph.h src/color.h src/graph_utils.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} src/graph_utils.c $(GSL_FLAGS) -o $(BUILD)/graph_utils.o

#----------------------------------
test: testool.o alltests test_ended_up test_finished direct_testing_code

compilation_tests: test_graph.o
run_all_tests: run_test1 run_test2 test_graph_impl_colors

testool.o: tst/include/testool.c tst/include/testool.h
	$(GXX_FLAGS_OBJ) tst/include/testool.c -o ./build/$@

test_graph.o: tst/test_graph.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} $< $(GSL_FLAGS) -o ./build/$@

run_test1: $(BUILD)/test_graph.o
	$(GXX_FLAGS_EXE) $< $(GSL_FLAGS) -o ./build/$@

run_test2: src/color_impl.c tst/test_color.c
	$(GXX_FLAGS_EXE) $^ -I$(GSL_PATH)/include  -o ./build/$@

test_graph_basic_struct.o: tst/include/testool.h tst/include/test_graph_basic_struct.h tst/test_graph_basic_struct.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} tst/test_graph_basic_struct.c $(GSL_FLAGS) -o ./build/$@

test_graph_basic_func.o: tst/include/testool.h tst/include/test_graph_basic_func.h tst/test_graph_basic_func.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} tst/test_graph_basic_func.c -o ./build/$@

test_graph_utils.o: tst/include/testool.h tst/include/test_graph_utils.h tst/test_graph_utils.c
	$(GXX_FLAGS_OBJ) ${TEST_FLAGS} tst/test_graph_utils.c -o ./build/$@

alltests: graph_basic.o graph_utils.o test_graph_basic_struct.o test_graph_basic_func.o test_graph_utils.o tst/alltests.c tst/include/testool.h
	$(GXX_FLAGS_EXE) ${TEST_FLAGS} $(BUILD)/testool.o $(BUILD)/graph_basic.o $(BUILD)/graph_utils.o $(BUILD)/test_graph_basic_struct.o $(BUILD)/test_graph_basic_func.o $(BUILD)/test_graph_utils.o tst/alltests.c $(GSL_FLAGS) -o $(BUILD)/$@
	chmod 777 tst/alltests.c

test_ended_up: graph_impl.o player_impl.o



test_finished: ./src/main_testing.c ./src/color_impl.c
	$(GXX_FLAGS_EXE) $(GSL_FLAGS) ./build/graph_impl.o ./build/player_impl.o $^  -lgsl -lgslcblas -lm -o $@



direct_testing_code: ./src/player_impl.c ./src/graph_impl.c ./tst/test_graph_impl.c ./src/color_impl.c
	$(GXX_FLAGS_EXE) $(GSL_FLAGS) $^ -lgsl -lgslcblas -lm -o $@


coverage_code: alltests
	./install/alltests #generation d'un fichier .gcda binaire
exec_coverage_code: coverage_code ./alltests.gcno
	$(execution_by_lgcov) alltests.gcno #montre la couverture de code

#----------------------------------
test_graph_impl_colors: tst/test_graph_impl.c src/graph_impl.c src/color_impl.c
	$(GXX_FLAGS_EXE) $^ $(GSL_FLAGS) -o ./build/$@

test_server: tst/test_server.c src/graph_utils.h src/graph_utils.c src/color.h src/graph_basic.c
	$(GXX_FLAGS_EXE) ${TEST_FLAGS} $^ $(GSL_FLAGS) -o ./build/$@


clean:
	rm -f a.out tst/*.o Makefile~ *.sh~ *.o *.so src/*.c~ src/*.h~ tst/*.c~ src/*.o
	rm -rf build
	rm -f install/*
	touch install/.keep
	rm -f *.gcno *.gcda
	rm -f install/*.gcno install/*.gcda
	rm -f test_finished
	rm -f *.o
	rm -f direct_testing_code
	rm -rf doc/latex doc/html
