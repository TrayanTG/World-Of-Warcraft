#include "Graphics.h"

#include "Engine.h"
#include "Data.h"

Graphics Graphics::s;

Graphics& Graphics::getInstance()
{
	return s;
}

// --------------------------------------------------------------------------------

void Graphics::gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

void Graphics::setcolor(Colour colour)
{
	/*
	Black, Blue, Green, Cyan, Red, Magenta, Yellow, LightGray, DarkGray,
	LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, LightYellow, White
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
	return;
}

void Graphics::clearscreen()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
	return;
}

void Graphics::SetConsoleWindowSize(short x, short y)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	if (h == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Unable to get stdout handle.");

	// If either dimension is greater than the largest console window we can have,
	// there is no point in attempting the change.
	{
		COORD largestSize = GetLargestConsoleWindowSize(h);
		if (x > largestSize.X)
			throw std::invalid_argument("The x dimension is too large.");
		if (y > largestSize.Y)
			throw std::invalid_argument("The y dimension is too large.");
	}


	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(h, &bufferInfo))
		throw std::runtime_error("Unable to retrieve screen buffer info.");

	SMALL_RECT& winInfo = bufferInfo.srWindow;
	COORD windowSize = { winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1 };

	if (windowSize.X > x || windowSize.Y > y)
	{
		// window size needs to be adjusted before the buffer size can be reduced.
		SMALL_RECT info =
		{
			0,
			0,
			x < windowSize.X ? x - 1 : windowSize.X - 1,
			y < windowSize.Y ? y - 1 : windowSize.Y - 1
		};

		if (!SetConsoleWindowInfo(h, TRUE, &info))
			throw std::runtime_error("Unable to resize window before resizing buffer.");
	}

	COORD size = { x, y };
	if (!SetConsoleScreenBufferSize(h, size))
		throw std::runtime_error("Unable to resize screen buffer.");


	SMALL_RECT info = { 0, 0, x - 1, y - 1 };
	if (!SetConsoleWindowInfo(h, TRUE, &info))
		throw std::runtime_error("Unable to resize window after resizing buffer.");
}

void Graphics::setFontSize(int size)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = size;                // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void Graphics::init()
{
	system("chcp 437");
	//Engine::getInstance().initMouse();
	setFontSize(DEF_CONSOLE_SIZE);
	SetConsoleWindowSize(DEF_CONSOLE_WIDTH, DEF_CONSOLE_HEIGHT);
	clearscreen();
}

// --------------------------------------------------------------------------------

Graphics::Graphics()
{
	init();
}

Graphics::~Graphics()
{

}

bool Graphics::drawBoarder(int tlx, int tly, int brx, int bry)
{
	if (tlx >= brx || tly >= bry) return false;
	if (tlx < 0 || tly < 0) return false;
	if (brx >= DEF_CONSOLE_WIDTH || bry >= DEF_CONSOLE_HEIGHT)return false;
	gotoxy(tlx, tly);std::cout << (char)218;
	for (int i = tlx + 1;i < brx;i++)std::cout << (char)196;
	std::cout << (char)191;
	for (int i = tly + 1;i < bry;i++)
	{
		gotoxy(tlx, i);
		std::cout << (char)179;
		gotoxy(brx, i);
		std::cout << (char)179;
	}
	gotoxy(tlx, bry);std::cout << (char)192;
	for (int i = tlx + 1;i < brx;i++)std::cout << (char)196;
	std::cout << (char)217;

	return true;
}

bool Graphics::clearBoarder(int tlx, int tly, int brx, int bry)
{
	if (tlx > brx || tly > bry) return false;
	if (tlx < 0 || tly < 0) return false;
	if (brx >= DEF_CONSOLE_WIDTH || bry >= DEF_CONSOLE_HEIGHT)return false;
	for (int i = tly;i <= bry;i++)
	{
		gotoxy(tlx, i);
		for (int j = tlx;j <= brx;j++)std::cout << ' ';
	}
	return true;
}

bool Graphics::drawButton(const Button &button)
{
	if (button.getTLX() < 0 || button.getTLY() < 0 || button.getTLX() + button.getLen() + 3 >= DEF_CONSOLE_WIDTH || button.getTLY() + 4 >= DEF_CONSOLE_HEIGHT) return false;

	clearBoarder(button.getTLX(), button.getTLY(), button.getTLX() + button.getLen() + 3, button.getTLY() + 4);
	drawBoarder(button.getTLX(), button.getTLY(), button.getTLX() + button.getLen() + 3, button.getTLY() + 4);

	Graphics::getInstance().gotoxy(button.getTLX() + 2, button.getTLY() + 2);std::cout << button.getText();

	return true;
}

bool Graphics::drawFrame(const char *name, int level, const Bar &HP, const Bar &res, Colour col, int tlx, int tly)
{
	if (tlx < 0 || tly < 0 || tlx + DEF_FRAME_WIDTH >= DEF_CONSOLE_WIDTH || tly + DEF_FRAME_HEIGHT >= DEF_CONSOLE_HEIGHT)return false;
	clearBoarder(tlx, tly, tlx + DEF_FRAME_WIDTH, tly + DEF_FRAME_HEIGHT);
	drawBoarder(tlx, tly, tlx + DEF_FRAME_WIDTH, tly + DEF_FRAME_HEIGHT);
	gotoxy(tlx + 1, tly + 1);
	std::cout << name << " (" << level << ')';
	//-----
	gotoxy(tlx + 1, tly + 2);
	setcolor(LightRed);
	std::cout << HP.Curr << "    ";
	gotoxy(tlx + DEF_FRAME_WIDTH - 4, tly+2);
	if (HP.Max < 1000)std::cout << ' ';
	if (HP.Max < 100)std::cout << ' ';
	if (HP.Max < 10)std::cout << ' ';
	std::cout << HP.Max;
	//-----
	gotoxy(tlx + 1, tly + 3);
	setcolor(col);
	std::cout << res.Curr << "    ";
	gotoxy(tlx + DEF_FRAME_WIDTH - 4, tly + 3);
	if (res.Max < 1000)std::cout << ' ';
	if (res.Max < 100)std::cout << ' ';
	if (res.Max < 10)std::cout << ' ';
	std::cout << res.Max;

	setcolor(White);
	return true;
}

bool Graphics::drawPlayerUI(const Player &player, int tlx, int tly)
{
	if (tlx < 0 || tly < 0 || tlx + DEF_CHARACTER_WIDTH >= DEF_CONSOLE_WIDTH || tly + DEF_CHARACTER_HEIGHT >= DEF_CONSOLE_HEIGHT) return false;
	clearBoarder(tlx, tly, tlx + DEF_CHARACTER_WIDTH, tly + DEF_CHARACTER_HEIGHT);
	drawBoarder(tlx, tly, tlx + DEF_CHARACTER_WIDTH, tly + DEF_CHARACTER_HEIGHT);
	drawPlayer(player, tlx, tly);

	player.eqAbilities[0]->setXY(tlx, tly + DEF_CHARACTER_HEIGHT + 1);
	player.eqAbilities[0]->showBox(player.getTotalDamageStats());
	player.eqAbilities[1]->setXY(tlx + DEF_ABILITY_SIZE * 2 + 1, tly + DEF_CHARACTER_HEIGHT + 1);
	player.eqAbilities[1]->showBox(player.getTotalDamageStats());
	player.eqAbilities[2]->setXY(tlx + 2 * (DEF_ABILITY_SIZE*2 + 1) + 1, tly + DEF_CHARACTER_HEIGHT + 1);
	player.eqAbilities[2]->showBox(player.getTotalDamageStats());
	player.eqAbilities[3]->setXY(tlx + 3 * (DEF_ABILITY_SIZE*2 + 1) + 1, tly + DEF_CHARACTER_HEIGHT + 1 );
	player.eqAbilities[3]->showBox(player.getTotalDamageStats());

	player.helmet->setXY(tlx + DEF_CHARACTER_WIDTH + 1, tly);
	player.helmet->showBox();
	player.shoulders->setXY(tlx + DEF_CHARACTER_WIDTH + 1, tly + DEF_ITEM_SIZE + 1);
	player.shoulders->showBox();
	player.chest->setXY(tlx + DEF_CHARACTER_WIDTH + 1, tly + 2 * (DEF_ITEM_SIZE + 1));
	player.chest->showBox();
	player.gloves->setXY(tlx + DEF_CHARACTER_WIDTH + 1, tly + 3 * (DEF_ITEM_SIZE + 1));
	player.gloves->showBox();
	player.legs->setXY(tlx + DEF_CHARACTER_WIDTH + 1, tly + 4 * (DEF_ITEM_SIZE + 1));
	player.legs->showBox();
	player.feet->setXY(tlx + DEF_CHARACTER_WIDTH + 1, tly + 5 * (DEF_ITEM_SIZE + 1));
	player.feet->showBox();
	player.weapon->setXY(tlx + DEF_CHARACTER_WIDTH + 1, tly + 6 * (DEF_ITEM_SIZE + 1) + 2);
	player.weapon->showBox();
	return true;
}

bool Graphics::drawPlayer(const Player &player, int tlx, int tly)
{
	if (tlx < 0 || tly < 0 || tlx + DEF_CHARACTER_WIDTH >= DEF_CONSOLE_WIDTH || tly + DEF_CHARACTER_HEIGHT >= DEF_CONSOLE_HEIGHT) return false;
	bool hasWeapon = (player.weapon->getID() >= 0);
	if (!hasWeapon)
	{
		drawHelmet(player.helmet, tlx + 14, tly + 2);
		drawShoulders(player.shoulders, tlx + 7, tly + 7);
		drawChest(player.chest, tlx + 12, tly + 7);
		drawGloves(player.gloves, tlx + 7, tly + 10);
		drawLegs(player.legs, tlx + 12, tly + 14);
		drawFeet(player.feet, tlx + 10, tly + 20);
	}
	else
	{
		drawHelmet(player.helmet, tlx + 9, tly + 2);
		drawShoulders(player.shoulders, tlx + 2, tly + 7);
		drawChest(player.chest, tlx + 7, tly + 7);
		drawGloves(player.gloves, tlx + 2, tly + 10);
		drawLegs(player.legs, tlx + 7, tly + 14);
		drawFeet(player.feet, tlx + 5, tly + 20);
		drawWeapon(player.weapon, tlx + 27, tly + 4);
	}
	return true;
}

bool Graphics::drawCharacterInfo(const Player &player, int tlx, int tly)
{
	if (tlx < 0 || tly < 0 || false || false)return false;
	clearBoarder(tlx, tly, tlx + DEF_CHAR_INFO_WIDTH, tly + DEF_CHAR_INFO_HEIGH);
	drawBoarder(tlx, tly, tlx + DEF_CHAR_INFO_WIDTH, tly + DEF_CHAR_INFO_HEIGH);

	setcolor(White);
	gotoxy(tlx+2, tly + 1);std::cout << player.getName()<<'('<<player.getLevel()<<')';
	if (player.getLevel() < MAX_LEVEL) { gotoxy(tlx + 2, tly + 2);std::cout << "XP: " << player.getXP();std::cout << '/' << DEF_LEVEL_EXP[player.getLevel() - 1]; }
	gotoxy(tlx + 2, tly + 3);std::cout << "Gold: " << player.getGold();
	gotoxy(tlx + 2, tly + 4);std::cout << "Health: " << player.getHP().Max;
	gotoxy(tlx + 2, tly + 5);std::cout << "Physical Damage: ";setcolor(LightRed);std::cout << player.getTotalDamageStats().Physical;setcolor(White);
	gotoxy(tlx + 2, tly + 6);std::cout << "Magical Damage: ";setcolor(LightBlue);std::cout << player.getTotalDamageStats().Magical;setcolor(White);
	gotoxy(tlx + 2, tly + 7);std::cout << "Armor: ";setcolor(LightRed);std::cout << player.getTotalDefenceStats().Armor;setcolor(White);
	gotoxy(tlx + 2, tly + 8);std::cout << "Magic Resist: ";setcolor(LightBlue);std::cout << player.getTotalDefenceStats().MagicResist;setcolor(White);

	return true;
}

void Graphics::drawWeapon(const Weapon *weapon, int tlx, int tly)
{
	if (weapon->getID() < 0)setcolor(White);
	else if (weapon->getMinLevel() < 2)setcolor(Yellow);
	else if (weapon->getMinLevel() < 4)setcolor(LightGray);
	else if (weapon->getMinLevel() < 6)setcolor(LightYellow);
	else if (weapon->getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);

	if (weapon->getWeaponType() == Axe)drawAxe(tlx, tly);
	else drawStaff(tlx, tly);

	setcolor(White);
}

void Graphics::drawHelmet(const Armor *helmet, int tlx, int tly)
{
	if (helmet->getID() < 1)setcolor(White);
	else if (helmet->getMinLevel() < 2)setcolor(Yellow);
	else if (helmet->getMinLevel() < 4)setcolor(LightGray);
	else if (helmet->getMinLevel() < 6)setcolor(LightYellow);
	else if (helmet->getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	gotoxy(tlx, tly + 0);std::cout << " ******* ";
	gotoxy(tlx, tly + 1);std::cout << "*       *";
	gotoxy(tlx, tly + 2);std::cout << "*       *";
	gotoxy(tlx, tly + 3);std::cout << "*       *";
	gotoxy(tlx, tly + 4);std::cout << " ******* ";

	setcolor(White);
	gotoxy(tlx + 2, tly + 1);std::cout << "*   *";
	gotoxy(tlx + 3, tly + 3);std::cout << "***";
}

void Graphics::drawShoulders(const Armor *shoulders, int tlx, int tly)
{
	if (shoulders->getID() < 1)setcolor(White);
	else if (shoulders->getMinLevel() < 2)setcolor(Yellow);
	else if (shoulders->getMinLevel() < 4)setcolor(LightGray);
	else if (shoulders->getMinLevel() < 6)setcolor(LightYellow);
	else if (shoulders->getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	gotoxy(tlx, tly + 0);std::cout << " *****";
	gotoxy(tlx, tly + 1);std::cout << "******";
	gotoxy(tlx, tly + 2);std::cout << "****  ";
	gotoxy(tlx + 17, tly + 0);std::cout << "***** ";
	gotoxy(tlx + 17, tly + 1);std::cout << "******";
	gotoxy(tlx + 17, tly + 2);std::cout << "  ****";
	setcolor(White);
}

void Graphics::drawChest(const Armor *chest, int tlx, int tly)
{
	if (chest->getID() < 1)setcolor(White);
	else if (chest->getMinLevel() < 2)setcolor(Yellow);
	else if (chest->getMinLevel() < 4)setcolor(LightGray);
	else if (chest->getMinLevel() < 6)setcolor(LightYellow);
	else if (chest->getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	gotoxy(tlx + 1, tly + 0);std::cout <<  "***********";
	gotoxy(tlx + 1, tly + 1);std::cout <<  "***********";
	gotoxy(tlx + 0, tly + 2);std::cout << "*************";
	gotoxy(tlx + 0, tly + 3);std::cout << "*************";
	gotoxy(tlx + 0, tly + 4);std::cout << "*************";
	gotoxy(tlx + 0, tly + 5);std::cout << "*************";
	gotoxy(tlx + 0, tly + 6);std::cout << "*************";
	setcolor(White);
}

void Graphics::drawGloves(const Armor *gloves, int tlx, int tly)
{
	if (gloves->getID()<1)setcolor(White);
	else if (gloves->getMinLevel() < 2)setcolor(Yellow);
	else if (gloves->getMinLevel() < 4)setcolor(LightGray);
	else if (gloves->getMinLevel() < 6)setcolor(LightYellow);
	else if (gloves->getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	gotoxy(tlx, tly + 0);std::cout << "****";
	gotoxy(tlx, tly + 1);std::cout << "****";
	gotoxy(tlx, tly + 2);std::cout << "****";
	gotoxy(tlx, tly + 3);std::cout << "****";
	gotoxy(tlx, tly + 4);std::cout << "||||";
	gotoxy(tlx + 19, tly + 0);std::cout << "****";
	gotoxy(tlx + 19, tly + 1);std::cout << "****";
	gotoxy(tlx + 19, tly + 2);std::cout << "****";
	gotoxy(tlx + 19, tly + 3);std::cout << "****";
	gotoxy(tlx + 19, tly + 4);std::cout << "||||";
	setcolor(White);
}

void Graphics::drawLegs(const Armor *legs, int tlx, int tly)
{
	if (legs->getID() < 1)setcolor(White);
	else if (legs->getMinLevel() < 2)setcolor(Yellow);
	else if (legs->getMinLevel() < 4)setcolor(LightGray);
	else if (legs->getMinLevel() < 6)setcolor(LightYellow);
	else if (legs->getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	gotoxy(tlx, tly + 0);std::cout << " =========== ";
	gotoxy(tlx, tly + 1);std::cout << "*************";
	gotoxy(tlx, tly + 2);std::cout << "*************";
	gotoxy(tlx, tly + 3);std::cout << "****** ******";
	gotoxy(tlx, tly + 4);std::cout << "*****   *****";
	gotoxy(tlx, tly + 5);std::cout << "*****   *****";
	setcolor(White);
}

void Graphics::drawFeet(const Armor *feet, int tlx, int tly)
{
	if (feet->getID() < 1)setcolor(White);
	else if (feet->getMinLevel() < 2)setcolor(Yellow);
	else if (feet->getMinLevel() < 4)setcolor(LightGray);
	else if (feet->getMinLevel() < 6)setcolor(LightYellow);
	else if (feet->getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	gotoxy(tlx, tly + 0);std::cout << "  *****   *****  ";
	gotoxy(tlx, tly + 1);std::cout << "  *****   *****  ";
	gotoxy(tlx, tly + 2);std::cout << "*******   *******";
	gotoxy(tlx, tly + 3);std::cout << "*******   *******";
	setcolor(White);
}

void Graphics::drawAxe(int tlx, int tly)
{
	gotoxy(tlx + 1, tly + 0);std::cout << "  /\\  ";
	gotoxy(tlx + 1, tly + 1);std::cout << " /  \\ ";
	gotoxy(tlx + 1, tly + 2);std::cout << "/    \\";
	gotoxy(tlx + 1, tly + 3);std::cout << "|    |";
	gotoxy(tlx + 1, tly + 4);std::cout << "|    |";
	gotoxy(tlx + 1, tly + 5);std::cout << "|    |";
	gotoxy(tlx + 1, tly + 6);std::cout << "|    |";
	gotoxy(tlx + 1, tly + 7);std::cout << "|    |";
	gotoxy(tlx + 1, tly + 8);std::cout << "|    |";
	gotoxy(tlx + 0, tly + 9);for (int i = 0;i < 8;i++)std::cout << (char)240;
	gotoxy(tlx + 3, tly + 10);std::cout << (char)186 << (char)186;
	gotoxy(tlx + 3, tly + 11);std::cout << (char)186 << (char)186;
	gotoxy(tlx + 3, tly + 12);std::cout << (char)186 << (char)186;
	gotoxy(tlx + 3, tly + 13);std::cout << (char)186 << (char)186;
}

void Graphics::drawStaff(int tlx, int tly)
{
	gotoxy(tlx + 0, tly + 0);std::cout << "  **** ";
	gotoxy(tlx + 0, tly + 1);std::cout << " *    *";
	gotoxy(tlx + 0, tly + 2);std::cout << "*     *";
	gotoxy(tlx + 0, tly + 3);std::cout << "*     *";
	gotoxy(tlx + 0, tly + 4);std::cout << " *    *";
	gotoxy(tlx + 0, tly + 5);std::cout << "      *";
	gotoxy(tlx + 0, tly + 6);std::cout << "      *";
	gotoxy(tlx + 0, tly + 7);std::cout << "      *";
	gotoxy(tlx + 0, tly + 8);std::cout << "      *";
	gotoxy(tlx + 0, tly + 8);std::cout << "      *";
	gotoxy(tlx + 0, tly + 9);std::cout << "      *";
	gotoxy(tlx + 0, tly + 10);std::cout << "      *";
	gotoxy(tlx + 0, tly + 11);std::cout << "      *";
	gotoxy(tlx + 0, tly + 12);std::cout << "      *";
}

void Graphics::drawClassChooseUI()
{
	setFontSize(DEF_CONSOLE_SIZE*2);
	SetConsoleWindowSize(DEF_CONSOLE_WIDTH/2, DEF_CONSOLE_HEIGHT/2);
	clearscreen();

	gotoxy((DEF_CONSOLE_WIDTH/2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 - 2);std::cout << "Choose your class:";
	gotoxy((DEF_CONSOLE_WIDTH/2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 - 1);std::cout << "-Warrior <W>";
	gotoxy((DEF_CONSOLE_WIDTH/2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 - 0);std::cout << "-Mage <M>";
	gotoxy((DEF_CONSOLE_WIDTH/2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 + 1);std::cout << "-Paladin <P>";
}

void Graphics::drawNewOldUI()
{
	clearscreen();
	gotoxy(DEF_CONSOLE_WIDTH / 4 - 54/2, DEF_CONSOLE_HEIGHT / 4 - 1);
	std::cout << "Start a new character or log into existing one? <N/E>";
}

void Graphics::drawEnterName()
{
	clearscreen();
	Engine::getInstance().setCursorVisible(true);
	gotoxy(DEF_CONSOLE_WIDTH / 4 - 12 / 2, DEF_CONSOLE_HEIGHT / 4 - 1);
	std::cout << "Enter name: ";
}

void Graphics::drawHomeUI(const Player &player)
{
	clearBoarder(0, 0, DEF_FREE_BEG, DEF_CONSOLE_HEIGHT - 1);
	drawPlayerUI(player, 0, 10);
	drawCharacterInfo(player, 0, 0);
	
	clearBoarder(DEF_CHAR_INFO_WIDTH + 1, 0, DEF_CHAR_INFO_WIDTH + 19, DEF_CHAR_INFO_HEIGH);
	drawBoarder(DEF_CHAR_INFO_WIDTH + 1, 0, DEF_CHAR_INFO_WIDTH + 19, DEF_CHAR_INFO_HEIGH);

	gotoxy(DEF_CHAR_INFO_WIDTH + 3, 1);std::cout << "Menu:";
	gotoxy(DEF_CHAR_INFO_WIDTH + 3, 3);std::cout << "Home <H>";
	gotoxy(DEF_CHAR_INFO_WIDTH + 3, 4);std::cout << "Play <P>";
	gotoxy(DEF_CHAR_INFO_WIDTH + 3, 5);std::cout << "Shop <S>";
	gotoxy(DEF_CHAR_INFO_WIDTH + 3, 6);std::cout << "Inventory <I>";
	gotoxy(DEF_CHAR_INFO_WIDTH + 3, 7);std::cout << "AbilityBook <A>";
	gotoxy(DEF_CHAR_INFO_WIDTH + 3, 8);std::cout << "Exit <E>";
}

void Graphics::drawInventoryUI(const Player &player, Box **boxes, int num, Button &equipItem, Button &sellItem)
{
	int totalLenX = 20 * (DEF_ITEM_SIZE + 1);
	int totalLenY = 5 * (DEF_ITEM_SIZE + 1);

	Graphics::getInstance().clearBoarder(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 1, ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2);
	Graphics::getInstance().drawBoarder(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 1, ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2);
	equipItem.showButton();
	sellItem.showButton();
	for (int i = 0;i < num;i++) boxes[i]->showBox();
}

void Graphics::drawShopUI(const Player &player, Box **boxes, int num, Button &buyItem)
{
	int totalLenX = 20 * (DEF_ITEM_SIZE + 1);
	int totalLenY = 5 * (DEF_ITEM_SIZE + 1);

	Graphics::getInstance().clearBoarder(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 1, ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2);
	Graphics::getInstance().drawBoarder(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 1, ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2);
	buyItem.showButton();
	for (int i = 0;i < num;i++) boxes[i]->showBox();
}

void Graphics::drawAbilityBookUI(const Player &player, Box **boxes, int num, Button &eqSlot1, Button &eqSlot2, Button &eqSlot3, Button &eqSlot4)
{
	int totalLenX = 6 * (DEF_ABILITY_SIZE + 1);
	int totalLenY = 3 * (DEF_ABILITY_SIZE + 1);

	Graphics::getInstance().clearBoarder(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 6, ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 - 5);
	Graphics::getInstance().drawBoarder(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 6, ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 - 5);
	eqSlot1.showButton();
	eqSlot2.showButton();
	eqSlot3.showButton();
	eqSlot4.showButton();
	for (int i = 0;i < num;i++) (Ability*)boxes[i]->showBox(player.getTotalDamageStats());
}