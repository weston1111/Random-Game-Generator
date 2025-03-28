#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>

const std::string FILE_NAME = "games_list.txt";

struct Game
{
    std::string title;
    std::string game_genre;
    int chance_multiplier;
};

// main functions


// randomize game 
// parameter : games to be randomized from
// 1. user will decide to randomize from certain genre or from all games
// 2. prints out randomized game
void randomize_game(std::vector<Game> & games);

// add game
// 1. has user input game details
// 2. adds game to file
void add_game_in_file();

// edit game
// 1. displays games
// 2. user inputs what game to edit
// 3. user picks what to edit
// 4. edited changes in game made to file
void edit_game_in_file();

// remove gmae
// 1. display games
// 2. user inputs what game to remove
// 3. game gets removed from file
void remove_game_in_file();

void print_games_from_file();
void display_info();


// helper functions
std::vector<Game> read_games_from_file();
void create_file();
bool does_file_exist_and_not_empty();
bool is_valid_string(const std::string & str);


