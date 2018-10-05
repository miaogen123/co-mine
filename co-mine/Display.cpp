#include "Display.h"



Display::Display()
{
}

void Display::clear(){
	printf("\033[2J");
}

void Display::reset(){
	printf("\033[H");
}

void Display::showCursor(){
	printf("\033[?25h");
}

void Display::moveUp(unsigned int x){
	printf("\033[%dA", (x));
}

void Display::moveTo(unsigned int x, unsigned int y){
	printf("\033[%d;%dH", x, 4*(y-1)+1);
}

void Display::moveDown(unsigned int x){
	printf("\033[%dB", (x));
}

void Display::moveLeft(unsigned int x){
	printf("\033[%dD", (x));
}

void Display::moveRight(unsigned int x){
	printf("\033[%dC", (x));
}

void Display::showSthAt(unsigned int x, unsigned int y, std::string content){
	printf("\033[%d;%dH", x, 4*(y-1)+1);
	printf("%s", content.c_str());
	fflush(stdout);
}

void Display::showSthAt(unsigned int x, unsigned int y, std::string content, FontColor before, FontColor after){
	printf("\033[%d;%dH", x, 4*(y-1)+1);
	//仅支持替换字体颜色
	printf("\033[;%d;1m%s\033[;%d;0m",before, content.c_str(), after);
	fflush(stdout);
}

void Display::showSth(std::string content){
	printf("%s", content.c_str());
	fflush(stdout);
}

Display::~Display()
{
}
