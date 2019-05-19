#pragma once

#pragma once

#include "HeadersAndConsts.h"
#include "Player.h"
#include "Button.h"
#include "EnemyBox.h"

class Graphics
{
	static Graphics s;

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

	void init();
	void gotoxy(int x, int y)const;
	void setcolor(Colour foreground, Colour background = Black);
	void clearscreen();
	void SetConsoleWindowSize(short x, short y);
	void setFontSize(int size);

	bool drawPrecisely(char **text, int tlx, int tly, int width, int height)const;
	bool drawBoxesInRange(Box **boxes, int cntBoxes, int tlx, int tly, int brx, int bry, const Damage &damage = { 0,0 })const;

	bool drawGrass(int tlx, int tly, int brx, int bry);
	bool drawCloud(int tlx, int tly);

	bool drawBoarder(int tlx, int tly, int brx, int bry);
	bool clearBoarder(int tlx, int tly, int brx, int bry);
	bool drawButton(const Button &button);
	bool drawFrame(const Character &character, int tlx, int tly);
	bool drawPlayer(const Player &player, int tlx, int tly);
	bool drawEnemy(const Enemy &enemy, int tlx, int tly)const;
	bool drawPlayerUI(const Player &player, int tlx, int tly);
	bool drawCharacterInfo(const Player &player, int tlx, int tly);
	void drawHomeUI(const Player &player, Box **boxes, int num, int tlx = 0, int tly = 0, int brx = DEF_FREE_BEG, int bry = DEF_CONSOLE_HEIGHT);
	void drawInventoryUI(const Player &player, Box **boxes, int num, Button &equipItem, Button &sellItem, int tlx = DEF_FREE_BEG, int tly = 0, int brx = DEF_CONSOLE_WIDTH, int bry = DEF_CONSOLE_HEIGHT);
	void drawShopUI(const Player &player, Box **boxes, int num, Button &buyItem, int tlx=DEF_FREE_BEG, int tly=0, int brx = DEF_CONSOLE_WIDTH, int bry = DEF_CONSOLE_HEIGHT);
	void drawAbilityBookUI(const Player &player, Box **boxes, int num, Button &eqSlot1, Button &eqSlot2, Button &eqSlot3, Button &eqSlot4, int tlx = DEF_FREE_BEG, int tly = 0, int brx = DEF_CONSOLE_WIDTH, int bry = DEF_CONSOLE_HEIGHT);
	void drawMap(const std::pair<int, int> &pos, EnemyBox **enemies, int enemyCnt);
	void drawPlay(const Player &player, const Enemy &enemy);

	void drawClassChooseUI();
	void drawMapChooseUI();
	void drawNewOldUI();
	void drawEnterName();

};