#include "UserStat.h"
#include"myutils.h"


UserStat::UserStat(unsigned char userid):userID(userID)
{
}

void UserStat::setFontColor(FontColor fc)
{
	fontcolor = fc;
}

FontColor UserStat::getFontColor()
{
	return fontcolor;
}

unsigned char UserStat::getUserID()const
{
	return userID ;
}

void UserStat::setRow(int orow)
{
	row = orow;
}

void UserStat::setCol(int ocol){
	col = ocol; 
}


int& UserStat::getRow() 
{
	return row;
}

int& UserStat::getCol() 
{
	return col;
}

UserStat::~UserStat()
{
}
