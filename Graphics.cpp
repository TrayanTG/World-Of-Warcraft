#include "Graphics.h"

#include "Engine.h"

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

// --------------------------------------------------------------------------------

Graphics::Graphics()
{
	system("chcp 437");
	Engine::getInstance().init();
	setFontSize(DEF_CONSOLE_SIZE);
	SetConsoleWindowSize(DEF_CONSOLE_WIDTH, DEF_CONSOLE_HEIGHT);
	clearscreen();
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
		for (int j = tlx;j <= brx;j++)std::cout << '.';
	}
	return true;
}

bool Graphics::drawFrame(const char *name, int level, int currHP, int maxHP, int currRes, int maxRes, Colour col, int tlx, int tly)
{
	if (tlx < 0 || tly < 0 || tlx + DEF_FRAME_WIDTH >= DEF_CONSOLE_WIDTH || tly + DEF_FRAME_HEIGHT >= DEF_CONSOLE_HEIGHT)return false;
	clearBoarder(tlx, tly, tlx + DEF_FRAME_WIDTH, tly + DEF_FRAME_HEIGHT);
	drawBoarder(tlx, tly, tlx + DEF_FRAME_WIDTH, tly + DEF_FRAME_HEIGHT);
	gotoxy(tlx + 1, tly + 1);
	std::cout << name << " (" << level << ')';
	//-----
	gotoxy(tlx + 1, tly + 2);
	setcolor(LightRed);
	std::cout << currHP << "    ";
	gotoxy(tlx + DEF_FRAME_WIDTH - 4, tly+2);
	if (maxHP < 1000)std::cout << ' ';
	if (maxHP < 100)std::cout << ' ';
	if (maxHP < 10)std::cout << ' ';
	std::cout << maxHP;
	//-----
	gotoxy(tlx + 1, tly + 3);
	setcolor(col);
	std::cout << currRes << "    ";
	gotoxy(tlx + DEF_FRAME_WIDTH - 4, tly + 3);
	if (maxRes < 1000)std::cout << ' ';
	if (maxRes < 100)std::cout << ' ';
	if (maxRes < 10)std::cout << ' ';
	std::cout << maxRes;

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
	bool hasWeapon = (player.weapon != nullptr);
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

void Graphics::drawWeapon(const Weapon *weapon, int tlx, int tly)
{
	if (weapon->getID() < 1)setcolor(White);
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
