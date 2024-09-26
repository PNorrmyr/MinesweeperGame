#pragma once
#include <vector>
#include "Square.h"
#include <string>

class Board
{
public:
	Board(int rows, int columns, int mines);
	~Board();
	void randomlyPlaceMines();
	void displayBoard();
	bool revealSquareAndCheck(std::pair<int, int> square);
	std::pair<int, int> inputToCoord(const std::string& input);
	void revealSurrSquares(std::pair<int, int> square);
	void revealBoard();
	void toggleFlag(std::pair<int, int> square);
	bool checkIsGameWon();
	int countSurrMines(int row, int col);
	void saveGame(const std::string& filename);
	Board* loadGame(const std::string& filename);
	



private:
	int rows, columns;
	int totalMines;
	Square** board;
	


};

