#include <iostream>
#include <vector>
#include "Board.h"
#include "Square.h"
#include <time.h>
#include <cstdlib>
#include <fstream>

void setDifficulty(int& rows, int& columns, int& totalMines) {
    std::string userInput;

    std::cout << "Choose difficulty: (E)asy, (A)verage, (H)ard, E(x)treme" << std::endl;
    std::cout << "To exit and save game type 'exit' at any time." << std::endl;

    std::cin >> userInput;
    for (auto& c : userInput) c = toupper(c);

    if (userInput == "EXIT") {
        std::cout << "Exiting the game." << std::endl;
        exit(0);  // Avslutar programmet
    }

    switch (userInput[0]) {
    case 'E':
        rows = 10;
        columns = 10;
        totalMines = 10;
        break;
    case 'A':
        rows = 10;
        columns = 10;
        totalMines = 18;
        break;
    case 'H':
        rows = 15;
        columns = 15;
        totalMines = 50;
        break;
    case 'X': 
        rows = 15;
        columns = 15;
        totalMines = 75;
        break;
    default:
        std::cout << "Invalid input!" << std::endl;
        setDifficulty(rows, columns, totalMines);
    }
}

void gameLoop(Board* board) {
    bool gameRunning = true;
    std::string userInput;

    while (gameRunning) {
        board->displayBoard();

        std::cout << "Enter a square to check (e.g. A2), 'F' to flag a square or 'exit' to exit." << std::endl;
        std::cin >> userInput;

        if (userInput == "exit") {
            std::cout << "Saving and exiting the game..." << std::endl;
            // Här ska sparfunktion implementeras
            board->saveGame("savedGame");
            break;
        }

        //Spelare har valt att flagga en ruta
        if (toupper(userInput[0]) == 'F' && userInput.length() == 1) {
            std::cout << "Enter a square to flag (e.g. A2): ";
            std::cin >> userInput;

            std::pair<int, int> coord = board->inputToCoord(userInput);
            if (coord.first == -1 || coord.second == -1) {
                std::cout << "Index does not exist. Try again." << std::endl;
                continue;
            }
            board->toggleFlag(coord);  // Flagga eller avflagga rutan
            continue;
        }

        std::pair<int, int> coord = board->inputToCoord(userInput);
        if (coord.first == -1 || coord.second == -1) {
            std::cout << "Index does not exist. Try again." << std::endl;
            continue;
        }
        //Spelare kollar en ruta, om sant slutas loopen 
        if (board->revealSquareAndCheck(coord)) {
            gameRunning = false;
        }
    }
}

int main()
{
    srand(time(NULL));

    int rows = 0;
    int columns = 0;
    int totalMines = 0;
    Board* board = nullptr;
    

    std::string userInput;

    std::cout << "    Welcome to MineSweeper   \n";
    std::cout << " Would you like to: \n 1: Play a new game \n 2: Pick up where you left off?" << std::endl;
    std::cin >> userInput;

    //Kollar giltigt input
    if ((userInput[0] != '1' && userInput[0] != '2') || userInput.size() != 1)
    {
        std::cout << "Invalid input" << std::endl;
        return 1;
    } 

    //Starta nytt spel
    if (userInput[0] == '1') {
        setDifficulty(rows, columns, totalMines);
        board = new Board(rows, columns, totalMines);
    } 

    //Ladda sparat spel
    if (userInput[0] == '2') {
        try {
            board = new Board(0, 0, 0);
            if (board->loadGame("savedGame") == nullptr) {
                std::cout << "No saved game found. Starting new game" << std::endl;
                delete board;
                setDifficulty(rows, columns, totalMines);
                board = new Board(rows, columns, totalMines);
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return 1;
        }
    }


    gameLoop(board);
    
    delete board;

    return 0;  
}
