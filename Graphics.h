#pragma once

#pragma once

#include "HeadersAndConsts.h"

class Graphics
{
	static Graphics s;

	void SetConsoleWindowSize(short x, short y);
	void setFontSize(int size);

	Graphics();

public:

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	~Graphics();

	static Graphics& getInstance();

	void gotoxy(int x, int y);
	void setcolor(Colour colour);
	void clearscreen();

	bool drawBoarder(int tlx, int tly, int brx, int bry);
	bool clearBoarder(int tlx, int tly, int brx, int bry);
	bool drawFrame(const char *name, int level, int currHP, int maxHP, int currRes, int maxRes, Colour col, int tlx, int tly);
	

};