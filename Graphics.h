#pragma once

#pragma once

#include "HeadersAndConsts.h"
#include "Player.h"

class Graphics
{
	static Graphics s;

	void SetConsoleWindowSize(short x, short y);
	void setFontSize(int size);

	Graphics();

	void drawHelmet(const Armor *helmet, int tlx, int tly);
	void drawShoulders(const Armor *shoulders, int tlx, int tly);
	void drawChest(const Armor *chest, int tlx, int tly);
	void drawGloves(const Armor *gloves, int tlx, int tly);
	void drawLegs(const Armor *legs, int tlx, int tly);
	void drawFeet(const Armor *feet, int tlx, int tly);
	void drawStaff(int tlx, int tly);
	void drawAxe(int tlx, int tly);
	void drawWeapon(const Weapon *weapon, int tlx, int tly);


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
	bool drawPlayer(const Player &player, int tlx, int tly);
	bool drawPlayerUI(const Player &player, int tlx, int tly);

};