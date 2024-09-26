#include "Board.h"
#include "Square.h"
#include <iostream>
#include <fstream>


Board::Board(int rows, int columns, int mines) : rows(rows), columns(columns), totalMines(mines) {
    board = new Square* [rows];
    for (int i = 0; i < rows; ++i) {
        board[i] = new Square[columns];
    }

	randomlyPlaceMines();
}

Board::~Board() {
    for (int i = 0; i < rows; ++i) {
        delete[] board[i];
    }
    delete[] board;
}


void Board::randomlyPlaceMines() {
	int minesPlaced = 0;

	while (minesPlaced < totalMines) { 
		int randRow = rand() % rows;
		int randColumn = rand() % columns;

		if (!board[randRow][randColumn].getIsMine()) { //S�tter mina i square om den inte har en redan
			board[randRow][randColumn].setMine(true);
			minesPlaced++;
		}
	}
}


void Board::displayBoard() {
    // Printar header f�r kolumner
    std::cout << "   ";  
    for (int col = 1; col <= columns; ++col) {
        std::cout << "  " << col << " ";
    }
    std::cout << std::endl;

    // Printar horisontell delare mellan header och rader
    std::cout << "   +";
    for (int i = 0; i < columns; ++i) {
        std::cout << "---+";
    }
    std::cout << std::endl;

    // Printar raderna med bokst�ver samt spelplanen
    for (int r = 0; r < rows; ++r) {
        // Printar radernas namn
        std::cout << " " << static_cast<char>('A' + r) << " |";

        for (int c = 0; c < columns; ++c) {
            std::string cellContent = "   ";  // F�rvald tomt utrymme

            // Om rutan inte �r synlig
            if (!board[r][c].getIsVisible()) {
                if (board[r][c].getIsFlagged()) {
                    cellContent = " F ";  // Flagga
                }
            }
            // Om rutan �r synlig
            else {
                if (board[r][c].getIsMine()) {
                    cellContent = " * ";  // Mina
                }
                else {
                    cellContent = " " + std::to_string(board[r][c].getValue()) + " ";  // Siffra
                }
            }

            std::cout << cellContent << "|";
        }
        std::cout << std::endl;
       
        // Printar horisontell delare mellan rader
        std::cout << "   +";
        for (int i = 0; i < columns; ++i) {
            std::cout << "---+";
        }
        std::cout << std::endl;
    }
}


//konverterar ett input till ett pair f�r enklare hantering
std::pair<int, int> Board::inputToCoord(const std::string& input) {
    if (input.length() < 2 || !isalpha(input[0]) || !isdigit(input[1])) { //kollar att formatet �r giltigt
        return std::make_pair(-1, -1);
    }
    
    //konvertera input till rad och kolumn
    char rowChar = toupper(input[0]); //Tar radens karakt�r
    int col = std::stoi(input.substr(1)) - 1; //Plockar ut och konverterar columnen
    int row = rowChar - 'A'; //Konverterar 'A' till 0, 'B' till 1...

    if (row >= rows || row < 0 || col >= columns || col < 0) { //kollar att valt index finns
        return std::make_pair(-1, -1);
    }
    return std::make_pair(row, col);
}

//Logik f�r att kolla vad som finns i rutan, �ppna upp fler angr�nsade rutor samt kolla om alla rutor utan minor �r �ppnade
bool Board::revealSquareAndCheck(std::pair<int, int> square) {
    int row = square.first;
    int col = square.second;

    //En ruta som �r �ppnad och mina leder till alla rutor �ppnar och en f�rlorad omg�ng
    if (board[row][col].getIsMine()) {
        std::cout << "Game over! You hit a mine!" << std::endl;
        revealBoard();
        return true;
    }

    //Ber�knar och s�tter v�rdet p� antal omgivande minor i rutan
    int surroundingMines = countSurrMines(row, col);
    board[row][col].setValue(surroundingMines);


    //G�r rutan synlig om den inte �r flaggad
    if (!board[row][col].getIsFlagged()) {
        board[row][col].makeVisible();
    }

    //Om inga minor finns runt rutan, g�r de omgivande rutorna synliga
    if (surroundingMines == 0) {
        revealSurrSquares(square);
    }

    //Kollar vilkor f�r att spelet �r vunnet mha en annan funktion
    if (checkIsGameWon()) {
        std::cout << "You've won the game!" << std::endl;
        return true;
    }

    return false;
}

//Logik f�r att �ppna upp omkringliggande rutor som inte �r synliga
void Board::revealSurrSquares(std::pair<int, int> square) {
    int row = square.first;
    int col = square.second;

    //Kollar angr�nsande rutor �t alla h�ll
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            //Hoppar �ver aktuell ruta
            if (i == 0 && j == 0) {
                continue;
            }

            //Kontrollerar att rutan �r inom spelplanen. �r rutan ej synlig, g�rs den synlig
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < columns) {
                if (!board[newRow][newCol].getIsVisible()) {
                    board[newRow][newCol].makeVisible();

                    //Den nya synliga rutan skickas till funktionen f�r att �terigen kolla om det �r mina etc.
                    revealSquareAndCheck({ newRow, newCol });
                }
            }
        }
    }
}

//G�r alla rutor synliga och printar spelplanen
void Board::revealBoard() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            board[r][c].makeVisible();
        }
    }
    displayBoard();
}
    
//Flaggar eller av flaggar en ruta
void Board::toggleFlag(std::pair<int, int> square) {
    int row = square.first;
    int col = square.second;

    if (board[row][col].getIsFlagged()) {
        board[row][col].unFlag();
    }
    else
    {
        board[row][col].setFlag();
    }
}

//�r alla rutor som inte �r minor �r synliga, �r spelet vunnet.
bool Board::checkIsGameWon() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (!board[r][c].getIsMine() && !board[r][c].getIsVisible()) {
                return false;
            }
        }
    }
    return true;
}

//R�knar hur m�nga omgivande rutor som har en mina och returnerar summan
int Board::countSurrMines(int row, int col) {
 
    int noOfSurrMines = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            if (i == 0 && j == 0) {
                continue;
            }

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < columns) {
                if (board[newRow][newCol].getIsMine()) {
                    noOfSurrMines++;
                }
            }
        }
    }
    return noOfSurrMines;
}

void Board::saveGame(const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "Error opening file for saving!" << std::endl;
        return;
    }

    outFile << rows << " " << columns << " " << totalMines << std::endl;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            Square& square = board[r][c];
            outFile << (square.getIsMine() ? 1 : 0) << " "
                << (square.getIsVisible() ? 1 : 0) << " "
                << (square.getIsFlagged() ? 1 : 0) << " "
                << square.getValue() << std::endl;
        }
    }

    outFile.close();
    std::cout << "Game saved to " << filename << std::endl;
}

Board* Board::loadGame(const std::string& filename) {
    std::ifstream inFile(filename);

    if (!inFile) {
        return nullptr;
    }

    inFile >> rows >> columns >> totalMines;

    // F�r att skapa ett nytt br�de med den laddade storleken
    board = new Square * [rows];
    for (int i = 0; i < rows; ++i) {
        board[i] = new Square[columns];
    }

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            int isMine, isVisible, isFlagged, value;
            inFile >> isMine >> isVisible >> isFlagged >> value;
            board[r][c].setMine(isMine == 1);
            if (isVisible == 1) {
                board[r][c].makeVisible();
            }
            if (isFlagged == 1) {
                board[r][c].setFlag();
            }
            board[r][c].setValue(value);
        }
    }
    inFile.close();
    std::cout << "Game loaded from " << filename << ".txt" << std::endl;
    return this;
}


