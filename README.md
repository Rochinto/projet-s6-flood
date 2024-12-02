# Projet Flood

- [Sujet du projet](https://www.labri.fr/perso/renault/working/teaching/projets/2021-22-S6-C-Flood.php)

- [Page sur thor](https://thor.enseirb-matmeca.fr/ruby/projects/projetss6-flood)



This project uses the GNU Scientific Libray.

### Before compiling the project

- install the GNU Scientific Library (GSL) by unzipping *gsl.zip* file
- `export LD_LIBRARY_PATH=` *gsl*

### Project compilation
- `make server` : compiles the server at ./build/server
- `make player_valid` : compiles a player making random but valid moves at ./build/libplayer_valid.so
- `make player_glouton` : compiles a "gluttonous" player, maximizing the instant wins for each move at ./build/libplayer_max.so
- `make alltests` : compiles a series of tests and some ".gcno" trace files for code coverage
- `GSL_PATH=` *gsl* 
- `make` : compiles all previous files
- `make install` : among the previously compiled files, creates a copy of every file that could be usefull to run a part of the project at ./install/*file_name*

### Project running
- `./install/server ./install/libplayer_max.so ./install/libplayer_valid.so` : plays a game between a gluttonous player and a random player.
Make sure not to use the same file for both players (you can still use a copy).
- `./install/alltests` : runs a series of tests