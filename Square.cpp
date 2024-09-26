#include "Square.h"
#include <ctime>
#include <cstdlib>

Square::Square() : value(0), isMine(false), isVisible(false), isFlagged(false) {}

void Square::setMine(bool mine) {
	isMine = mine;
}

bool Square::getIsMine() const {
	return isMine;
}

int Square::getValue() const {
	return value;
}

void Square::makeVisible() {
	isVisible = true;
}

bool Square::getIsVisible() const {
	return isVisible;
}

void Square::setValue(int noOfSurrMines) {
	value = noOfSurrMines;
}

void Square::setFlag() {
	isFlagged = true;
}

void Square::unFlag() {
	isFlagged = false;
}

bool Square::getIsFlagged() {
	return isFlagged;
}
