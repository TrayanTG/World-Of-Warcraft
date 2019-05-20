#pragma once

#pragma once

#include "HeadersAndConsts.h"
#include "Player.h"
#include "Button.h"
#include "EnemyBox.h"

class Graphics
{
	Graphics();

	static void drawStaff(const Coord &topLeft);
	static void drawAxe(const Coord &topLeft);
	static void drawHelmet(const Armor &helmet, const Coord &topLeft);
	static void drawShoulders(const Armor &shoulders, const Coord &topLeft);
	static void drawChest(const Armor &chest, const Coord &topLeft);
	static void drawGloves(const Armor &gloves, const Coord &topLeft);
	static void drawLegs(const Armor &legs, const Coord &topLeft);
	static void drawFeet(const Armor &feet, const Coord &topLeft);
	static void drawWeapon(const Weapon &weapon, const Coord &topLeft);

public:

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	static std::vector<string> fileToText(const string &directory, int &width);

	~Graphics();

	static void init();
	static void gotoxy(int x, int y);
	static void gotoxy(const Coord &pos);
	static void setcolor(Colour foreground, Colour background = Black);
	static void clearscreen();
	static void SetConsoleWindowSize(short x, short y);
	static void setFontSize(int size);

	static bool drawPrecisely(const std::vector<string> &text, const Coord &topLeft, char filler = '%');
	static bool drawBoxesInRange(Box **boxes, int cntBoxes, const Coord &topLeft, const Coord &botRight, const Damage &damage = { 0,0 });

	static bool drawGrass(const Coord &topLeft, const Coord &botRight);
	static bool drawCloud(const Coord &topLeft);
	static bool drawLogo(const Coord &topLeft = { DEF_CONSOLE_WIDTH / 2 + DEF_FREE_BEG / 2, DEF_CONSOLE_HEIGHT / 2 });
	static bool drawTextFromFile(const string &directory, const Coord &topLeft = { DEF_CONSOLE_WIDTH / 2 + DEF_FREE_BEG / 2,1 });

	static bool drawBoarder(const Coord &topLeft, const Coord &botRight);
	static bool clearBoarder(const Coord &topLeft, const Coord &botRight);
	static bool drawButton(const Button &button); // VOID!!!!
	static bool drawFrame(const Character &character, const Coord &topLeft);
	static bool drawPlayer(const Player &player, const Coord &topLeft);
	static bool drawEnemy(const Enemy &enemy, const Coord &topLeft);
	static bool drawPlayerUI(Player &player, const Coord &topLeft);
	static bool drawCharacterInfo(const Player &player, const Coord &topLeft);
	static void drawHomeUI(Player &player, Box **boxes, int num, const Coord &topLeft = { 0,0 }, const Coord &botRight = { DEF_FREE_BEG,DEF_CONSOLE_HEIGHT });
	static void drawInventoryUI(const Player &player, Box **boxes, int num, Button &equipItem, Button &sellItem, const Coord &topLeft = { DEF_FREE_BEG,0 }, const Coord &botRight = { DEF_CONSOLE_WIDTH, DEF_CONSOLE_HEIGHT });
	static void drawShopUI(const Player &player, Box **boxes, int num, Button &buyItem, const Coord &topLeft = { DEF_FREE_BEG, 0 }, const Coord &botRight = { DEF_CONSOLE_WIDTH,DEF_CONSOLE_HEIGHT });
	static void drawAbilityBookUI(const Player &player, Box **boxes, int num, Button &eqSlot1, Button &eqSlot2, Button &eqSlot3, Button &eqSlot4, const Coord &topLeft = { DEF_FREE_BEG,0 }, const Coord &botRight = { DEF_CONSOLE_WIDTH, DEF_CONSOLE_HEIGHT });
	static void drawMap(const std::pair<int, int> &pos, EnemyBox **enemies, int enemyCnt);
	static void drawPlay(const Player &player, const Enemy &enemy);
	static void drawResultsUI(Player &player, const Enemy &enemy);

	static void drawClassChooseUI();
	static void drawMapChooseUI();
	static void drawNewOldUI();
	static void drawEnterName();
};
