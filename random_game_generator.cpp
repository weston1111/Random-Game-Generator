#include "Random_game_generator.hpp"

std::vector<Game> read_games_from_file()
{
    std::ifstream file(FILE_NAME);
    std::vector<Game> games;
    std::string game_title, game_genre, line;
    int chance_multiplier;

    while(std::getline(file, line))
    {
        if(line == "")
            break;

        std::stringstream ss(line);

        std::getline(ss, game_title, ';');
        std::getline(ss, game_genre, ';');
        ss >> chance_multiplier;

        Game new_game = {game_title, game_genre, chance_multiplier};
        games.push_back(new_game);
    }

    file.close();
    return games;
}

void print_games_from_file()
{
    std::vector<Game> games = read_games_from_file();
    for(size_t i = 0; i < games.size(); i++)
    {
        if(i != games.size() - 1)
            std::cout << games[i].title + ", ";
        else
            std::cout << games[i].title;
    }
}

std::vector<std::string> get_and_print_genres_from_games(std::vector<Game> games)
{
    std::vector<std::string> genres; 
    for(Game game : games)
        genres.push_back(game.game_genre);
    

    std::sort(genres.begin(), genres.end());
    auto i = std::unique(genres.begin(), genres.end());
    genres.erase(i, genres.end());

    for(size_t i = 0; i < genres.size(); i++)
    {
        if(i != games.size() - 1)
            std::cout << genres[i] + ", ";
        else
            std::cout << genres[i];
    }

    return genres;
}

void create_file()
{
    std::ofstream file(FILE_NAME);
    
    if(file.is_open())
        file.close();
}

bool does_file_exist_and_not_empty()
{
    std::ifstream file(FILE_NAME);
    if(!file.good())
    {
        create_file();
        return false;
    }

    file.seekg(0, std::ios::end);

    // if we are at start of file after seeking to end then the file is empty
    if(file.tellg() == 0)
    {
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool is_valid_string(const std::string & str)
{
    if(str.empty())
    {
        return false;
    }
    for(char c : str)
    {
        if(!(std::isalpha(c) || std::isdigit(c) || c == ' ' || c == '/' || c == '!'))
            return false;
    }
    return true;
}

void display_info()
{
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "Welcome to Game Randomizer!\n";
    std::cout << "If you're new, add games to begin\n";
    std::cout << "The randomizer will spit out a random game selected from your games list.\n";
    std::cout << "\nChance multipliers will add more chances of the game to be selected in randomizer.\n";
    std::cout << "1 value will have 1 chance per randomize, 2 will have 2 chances, etc\n\n";
    std::cout << "------------------------------------------------------------------------------------\n";
}

void randomize_game(std::vector<Game> & games)
{
    std::string randomizing = "yes";
    while(randomizing == "yes")
    {
        bool option_found = false;
        bool random_genre = false;
        std::vector<Game> games_from_genre_picked;
        int randomize_option = 0;
        std::cout << "Would you like to randomize from a genre or from all games? 1 for specific genre, 2 for all games\n";

        while(!option_found)
        {
            std::cin >> randomize_option;
            std::string picked_genre;
            std::vector<std::string> genres;
            
            switch(randomize_option)
            {
                case 1:
                    option_found = true;
                    random_genre = true;
                    std::cout << "\nSelect a genre:\n";
                    genres = get_and_print_genres_from_games(games);
                    std::cout << "\n";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    while(true)
                    {
                        std::getline(std::cin, picked_genre);
                        if(std::find(genres.begin(), genres.end(), picked_genre) != genres.end()) // if user inputted genre from list
                            break;
                        if(picked_genre == "back")
                            continue;
                        else
                            std::cout << "Error. Enter valid genre from list. \n";
                    }
                    for(Game game : games)
                    {
                        if(game.game_genre == picked_genre)
                            games_from_genre_picked.push_back(game);
                    }
                    std::cout << "\nRandomizing game from genre \"" << picked_genre << "\"...\n";
                    break;
                case 2:
                    option_found = true;
                    break;
                default:
                    std::cout << "Error. Enter valid option.\n";
                    break;
            }
        }

        std::vector<Game> new_games = random_genre ? games_from_genre_picked : games;
        std::vector<Game> updated_games = new_games;
        Game randomized_game;

        // apply chance multipliers
        for(Game game : new_games)
        {
            if(game.chance_multiplier > 1)
            {
                for(int i = 0; i < game.chance_multiplier; i++)
                    updated_games.push_back(game);
            }
        }

        // randomize games
        std::random_device rd;
        std::shuffle(updated_games.begin(), updated_games.end(), rd);   
        randomized_game = updated_games[updated_games.size() - 1];
        updated_games.pop_back();

        std::cout << "Randomized game is: \n" << randomized_game.title << "\n";

        std::cout << "Would you like to randomize another game? yes or no\n";
        while(true)
        {
            std::cin >> randomizing;
            if(randomizing == "yes" || randomizing == "no")
            {
                std::cout << "\n";
                break;
            }
            else
            {
                std::cout << "Error, input either yes or no\n";
            }
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void add_game_in_file()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::ofstream file(FILE_NAME, std::ios::app);
    std::string title, genre, input;
    int chance_multiplier;
    std::string user_is_inputting_game = "yes";
    
    while(user_is_inputting_game == "yes")
    {
        // want them to seperate by semicolon cuz some games have spaces
        std::cout << "Input title, genre, and chance multiplier for your game seperated by semilcolon: <title>;<genre>;<chance multiplier>\n";
        
        std::getline(std::cin, input);
        std::stringstream ss(input);

        std::getline(ss, title, ';');
        std::getline(ss, genre, ';');
        ss >> chance_multiplier;
        
        if(is_valid_string(title) && is_valid_string(genre) && chance_multiplier > 0 && chance_multiplier <= 10)
        {
            file << title << ';' << genre << ';' << chance_multiplier << '\n';
            std::cout << "\nNew game \"" << title << "\" in genre \"" << genre << "\" with chance multiplier of " << chance_multiplier << " successfully added\n";
            std::cout << "Would you like to add another game? yes or no\n";
            while(true)
            {
                std::cin >> user_is_inputting_game;
                if(user_is_inputting_game == "yes" || user_is_inputting_game == "no")
                {
                    std::cout << "\n";
                    break;
                }
                else
                {
                    std::cout << "Error, input either yes or no\n";
                }
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else 
        {
            std::cout << "\nInvalid input. Try again\n";
        }
    }
    file.close();
}

void edit_game_in_file()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string is_editing_games = "yes";
    while(is_editing_games == "yes")
    {
        std::vector<Game> games_from_file = read_games_from_file();
        std::cout << "\nEnter a game to edit from your games list" << "\n";
        print_games_from_file();
        std::cout << "\n";
        std::string game_to_edit;
        std::getline(std::cin, game_to_edit);
        std::ifstream file(FILE_NAME);

        // copy file
        std::vector<std::string> lines;
        std::string line, game_title, game_genre;
        int chance_multiplier, line_index_to_edit;
        bool is_game_found = false;
        Game found_game;

        // extract game from file
        while(std::getline(file, line))
        {
            std::stringstream ss(line);
            lines.push_back(line);
            std::getline(ss, game_title, ';');
            std::getline(ss, game_genre, ';');
            ss >> chance_multiplier;
            if(game_title == game_to_edit)
            {
                found_game = {game_title, game_genre, chance_multiplier};
                line_index_to_edit = lines.size();
                is_game_found = true;
            }
        }

        if(is_game_found)
        {
            std::string new_line;
            std::getline(file, new_line);
            int edit_option;
            bool is_editing_current_game = true;
            std::string new_game_title = found_game.title;
            std::string new_genre = found_game.game_genre;
            char new_chance_multiplier = found_game.chance_multiplier;
            
            // get user edits for game
            std::cout << "\nSelect an option on how you want to edit for the game \"" << game_to_edit << "\"\n";
            while(is_editing_current_game)
            {
                std::cout << "1. Edit game title" << "\n";
                std::cout << "2. Edit genre" << "\n";
                std::cout << "3. Edit chance multiplier" << "\n";
                std::cout << "4. Done editing" << "\n";
                std::cin >> edit_option;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                switch(edit_option)
                {
                    case(1):
                        std::cout << "Input new game title:" << "\n";
                        std::getline(std::cin, new_game_title);
                        break;
                    case(2):
                        std::cout << "Input new genre for your game:" << "\n";
                        std::getline(std::cin, new_genre);
                        break;
                    case(3):
                        std::cout << "Input new chance multiplier:" << "\n";
                        std::cin >> new_chance_multiplier;
                        while(std::cin.fail() || !(std::isdigit(new_chance_multiplier)))
                        {
                            std::cout << "Error. input valid integer\n";
                            std::cin.clear();
                            std::cin.ignore(256, '\n');
                            std::cin >> new_chance_multiplier;
                        }
                        break;
                    case(4):
                        is_editing_current_game = false;
                        break;
                    default:
                        std::cout << "Error. Input valid option." << "\n";
                        break;
                }
            }
            // store new changes in file
            std::ofstream file("games_list.txt", std::ios::trunc);
            std::string new_game_in_file = new_game_title + ";" + new_genre + ";" + new_chance_multiplier;
            std::cout << "Game successfully edited to: \"" << new_game_title << "\" in genre \"" << new_genre << "\" with a chance multiplier of " << new_chance_multiplier << "\n";
            lines[line_index_to_edit - 1] = new_game_in_file;
            
            for(const std::string & line: lines)
                file << line << std::endl;
            
            
            std::cout << "Would you like to edit another game? yes or no\n";
            while(true)
            {
                std::cin >> is_editing_games;
                if(is_editing_games == "yes" || is_editing_games == "no")
                {
                    std::cout << "\n";
                    break;
                }
                else
                {
                    std::cout << "Error, input either yes or no\n";
                }
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        }
        else 
        {
            std::cout << "Game not found. Enter game exactly as listed\n";
        }
        file.close();
    }
}

void remove_game_in_file()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string is_removing_games = "yes";
    while(is_removing_games == "yes")
    {
        std::vector<Game> games_from_file = read_games_from_file();
        std::cout << "\nEnter a game to remove" << "\n";
        print_games_from_file();
        std::cout << "\n";
        std::string game_to_edit;
        std::getline(std::cin, game_to_edit);
        std::ifstream file(FILE_NAME);

        // copy file
        std::vector<std::string> lines;
        std::string line, game_title, found_game_title;
        int line_index_to_remove;
        bool is_game_found = false;

        while(std::getline(file, line))
        {
            std::stringstream ss(line);
            lines.push_back(line);
            std::getline(ss, game_title, ';');
            if(game_title == game_to_edit)
            {
                found_game_title = game_title;
                line_index_to_remove = lines.size();
                is_game_found = true;
            }
        }

        if(is_game_found)
        {
            // seek to the line we are removing
            file.seekg(std::ios::beg);
            for(int i = 0; i < line_index_to_remove - 1; i++)
                file.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); 
            
            std::string new_line;
            std::getline(file, new_line);
            
            lines.erase(lines.begin() + line_index_to_remove - 1);

            std::ofstream file("games_list.txt", std::ios::trunc);
            
            for(const std::string & line: lines)
                file << line << std::endl;
            std::cout << "\n\"" << found_game_title << "\" successfully removed.\n";

            std::cout << "Would you like to remove another game? yes or no\n";
            while(true)
            {
                std::cin >> is_removing_games;
                if(is_removing_games == "yes" || is_removing_games == "no")
                {
                    std::cout << "\n";
                    break;
                }
                else
                {
                    std::cout << "Error, input either yes or no\n";
                }
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else 
        {
            std::cout << "Game not found. Enter game exactly as listed\n";
        }
        file.close();
    }
}

int main(int args, char * argv[])
{
    bool randomizing = 1;
    while(randomizing)
    {
        std::vector<Game> games;
        int option_selection = 0;
        std::cout << "1. Randomize" << "\n";
        std::cout << "2. Add game" << "\n";
        std::cout << "3. Edit game" << "\n";
        std::cout << "4. Remove game" << "\n";
        std::cout << "5. Print games" << "\n";
        std::cout << "6. Display information" << "\n";
        std::cout << "7. Exit" << "\n";
        std::cin >> option_selection;
        
        switch(option_selection)
        {
            case(1):
                if(does_file_exist_and_not_empty())
                {
                    games = read_games_from_file();
                    randomize_game(games);
                }
                else
                    std::cout << "Empty games list. Add games in order to randomize!\n";
                break;
            case(2):
                add_game_in_file();
                break;
            case(3):
                if(does_file_exist_and_not_empty())
                    edit_game_in_file();
                else
                    std::cout << "Empty games list. Add games in order to edit!\n";
                break;
            case(4):
                if(does_file_exist_and_not_empty())
                    remove_game_in_file();
                else
                    std::cout << "Empty games list. Add games in order to remove!\n";
                break;
            case(5):
                if(does_file_exist_and_not_empty())
                {
                    std::cout << "\nYour games are: \n";
                    print_games_from_file();
                    std::cout << "\n\n";
                }
                else
                    std::cout << "Empty games list. Add games!\n";  
                break;
            case(6):
                display_info();
                break;
            case(7):
                randomizing = 0;
                break;
            default:
                std::cout << "Error. Input valid option." << "\n";
                break;
        } 
    }
    return 0;
}