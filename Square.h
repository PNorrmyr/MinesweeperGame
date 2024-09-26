#pragma once
class Square
{
public:
	Square();
	void setMine(bool mine);
	bool getIsMine() const;
	int getValue() const;
	void setValue(int noOfSurrMines);
	void makeVisible();
	bool getIsVisible() const;
	void setFlag();
	void unFlag();
	bool getIsFlagged();

	
	
private:
	int value;
	bool isMine;
	bool isVisible;
	bool isFlagged;
};

