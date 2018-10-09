#pragma once
#include"Display.h"

class UserStat
{
public:
	UserStat(unsigned char userid);
	void setFontColor(FontColor fc);
	FontColor getFontColor();
	unsigned char getUserID() const;
	void setRow(int orow);
	void setCol(int ocol);
	int& getRow();
	int& getCol();
	~UserStat();
	int mineNum = 0;
private:
	unsigned char userID;
	FontColor fontcolor;
	int row=1, col = 1;

};