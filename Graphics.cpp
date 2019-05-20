#include "Graphics.h"

#include "Engine.h"
#include "Data.h"
#include "PlayerClasses.h"

void Graphics::gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

void Graphics::gotoxy(const Coord &pos)
{
	COORD coord;
	coord.X = (SHORT)pos.X; coord.Y = (SHORT)pos.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

void Graphics::setcolor(Colour foreground, Colour background)
{
	/*
	Black, Blue, Green, Cyan, Red, Magenta, Yellow, LightGray, DarkGray,
	LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, LightYellow, White
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foreground + 16 * background);
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
	//wcscpy_s(cfi.FaceName, L"Lucida Console"); // Choose your font

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void Graphics::init()
{
	system("chcp 437"); //8217
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

std::vector<string> Graphics::fileToText(const string &directory, int &width)
{
	std::ifstream iFile(directory);
	if (!iFile) throw std::runtime_error("const std::vector<string> &fileToText(const string&) bad call!");
	std::vector<string> text;
	string temp;
	width = 0;
	while (iFile)
	{
		std::getline(iFile, temp);
		text.push_back(temp);
		if (text.back().size() > (size_t)width) width = text.back().size();
	}
	return text;
}

bool Graphics::drawPrecisely(const std::vector<string> &text, const Coord &topLeft, char filler)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || topLeft.Y + text.size() > DEF_CONSOLE_HEIGHT) return false;
	for (size_t i = 0;i < text.size();i++) if (topLeft.X + text[i].length() - 1 > DEF_CONSOLE_WIDTH)return false;

	for (size_t i = 0;i < text.size() - 1;i++)
	{
		for (int j = 0; text[i][j]; j++)
		{
			if (text[i][j] != filler)
			{
				gotoxy(j + topLeft.X, i + topLeft.Y);
				std::cout << text[i][j];
			}
		}
	}
	return true;
}

bool Graphics::drawBoxesInRange(Box **boxes, int cntBoxes, const Coord &topLeft, const Coord &botRight, const Damage &damage)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || botRight.X > DEF_CONSOLE_WIDTH || botRight.Y > DEF_CONSOLE_HEIGHT) return false;
	int fixed[128], cntFixed = 0;
	for (int i = topLeft.Y; i <= botRight.Y; i++)
	{
		for (int j = topLeft.X; j <= botRight.X; j++)
		{
			for (int k = 0;k < cntBoxes;k++)
			{
				if (boxes[k]->isWithin({ j, i }))
				{
					bool f = true;
					for (int p = 0; p < cntFixed; p++)
					{
						if (p == k)
						{
							f = false;
							break;
						}
					}
					if (f)
					{
						boxes[k]->showBox(damage);
						fixed[cntFixed++] = k;
					}
				}
			}
		}
	}
	return true;
}

bool Graphics::drawGrass(const Coord &topLeft, const Coord &botRight)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || botRight.X > DEF_CONSOLE_WIDTH || botRight.Y > DEF_CONSOLE_HEIGHT)return false;
	for (int i = topLeft.Y;i < botRight.Y;i++)
	{
		gotoxy(topLeft.X, i);
		for (int j = topLeft.X;j < botRight.X;j++)
		{
			int t = rand() % 80;
			if (t < 70)
			{
				std::cout << ' ';
			}
			else if (t < 73 && j < botRight.X - 2)
			{
				setcolor(LightGreen);
				std::cout << "\\/";j++;
			}
			else if (t < 75 && j < botRight.X - 6)
			{
				setcolor(Red);
				std::cout << "..--..";j += 5;
			}
			else if (t < 79)
			{
				setcolor(Red);
				std::cout << '.';
			}
			else
			{
				setcolor(Red);
				std::cout << '_';
			}
		}
	}
	setcolor(White);
	return true;
}

bool Graphics::drawCloud(const Coord &topLeft)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || topLeft.X + 36 >= DEF_CONSOLE_WIDTH || topLeft.Y + 6 >= DEF_CONSOLE_HEIGHT) return false;
	gotoxy(topLeft.X + 17, topLeft.Y + 0);std::cout << ",---.";
	gotoxy(topLeft.X + 16, topLeft.Y + 1);std::cout << "(     )";
	gotoxy(topLeft.X + 14, topLeft.Y + 2);std::cout << "_.-'  _'-. _";
	gotoxy(topLeft.X + 8, topLeft.Y + 3);std::cout << ",---.(     (    ) ),--.";
	gotoxy(topLeft.X + 1, topLeft.Y + 4);std::cout << "_.----(                       )-._";
	gotoxy(topLeft.X + 0, topLeft.Y + 5);std::cout << "(__________________________________)";
	return true;
}

bool Graphics::drawLogo(const Coord &topLeft)
{
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Logo", width);
	setcolor(Yellow);
	drawPrecisely(text, { topLeft.X - width / 2, topLeft.Y - (int)text.size() / 2 }, 'a');
	setcolor(White);
	return true;
}

bool Graphics::drawTextFromFile(const string &directory, const Coord &topLeft)
{
	int width;
	std::vector<string> text = fileToText(directory, width);
	setcolor(Yellow);
	drawPrecisely(text, {topLeft.X - (int)width / 2 + 3, topLeft.Y}, 'a');
	setcolor(White);
	return true;
}

bool Graphics::drawBoarder(const Coord &topLeft, const Coord &botRight)
{
	if (topLeft.X >= botRight.X || topLeft.Y >= botRight.Y) return false;
	if (topLeft.X < 0 || topLeft.Y < 0) return false;
	if (botRight.X >= DEF_CONSOLE_WIDTH || botRight.Y >= DEF_CONSOLE_HEIGHT)return false;
	gotoxy(topLeft);std::cout << (char)218;
	for (int i = topLeft.X + 1;i < botRight.X;i++)std::cout << (char)196;
	std::cout << (char)191;
	for (int i = topLeft.Y + 1;i < botRight.Y;i++)
	{
		gotoxy(topLeft.X, i);
		std::cout << (char)179;
		gotoxy(botRight.X, i);
		std::cout << (char)179;
	}
	gotoxy(topLeft.X, botRight.Y);std::cout << (char)192;
	for (int i = topLeft.X + 1;i < botRight.X;i++)std::cout << (char)196;
	std::cout << (char)217;

	return true;
}

bool Graphics::clearBoarder(const Coord &topLeft, const Coord &botRight)
{
	if (topLeft.X >= botRight.X || topLeft.Y >= botRight.Y) return false;
	if (topLeft.X < 0 || topLeft.Y < 0) return false;
	if (botRight.X >= DEF_CONSOLE_WIDTH || botRight.Y >= DEF_CONSOLE_HEIGHT)return false;
	for (int i = topLeft.Y;i <= botRight.Y;i++)
	{
		gotoxy(topLeft.X, i);
		for (int j = topLeft.X;j <= botRight.X;j++)std::cout << ' ';
	}
	return true;
}

bool Graphics::drawButton(const Button &button)
{
	if (button.getTopLeft().X < 0 || button.getTopLeft().Y < 0 || button.getTopLeft().X + button.getLen() + 3 >= DEF_CONSOLE_WIDTH || button.getTopLeft().Y + 4 >= DEF_CONSOLE_HEIGHT) return false;

	clearBoarder(button.getTopLeft(), { button.getTopLeft().X + button.getLen() + 3, button.getTopLeft().Y + 4 });
	drawBoarder(button.getTopLeft(), { button.getTopLeft().X + button.getLen() + 3, button.getTopLeft().Y + 4 });

	Graphics::gotoxy(button.getTopLeft().X + 2, button.getTopLeft().Y + 2);std::cout << button.getText();

	return true;
}

bool Graphics::drawFrame(const Character &character, const Coord &topLeft)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || topLeft.X + DEF_FRAME_WIDTH >= DEF_CONSOLE_WIDTH || topLeft.Y + DEF_FRAME_HEIGHT >= DEF_CONSOLE_HEIGHT)return false;
	clearBoarder(topLeft, { topLeft.X + DEF_FRAME_WIDTH, topLeft.Y + DEF_FRAME_HEIGHT });
	drawBoarder(topLeft, { topLeft.X + DEF_FRAME_WIDTH, topLeft.Y + DEF_FRAME_HEIGHT });
	gotoxy(topLeft.X + 1, topLeft.Y + 1);
	std::cout << character.getName() << " (" << character.getLevel() << ')';
	//-----
	gotoxy(topLeft.X + 1, topLeft.Y + 2);
	setcolor(LightRed);
	std::cout << character.getHP().Curr << "    ";
	gotoxy(topLeft.X + DEF_FRAME_WIDTH - 4, topLeft.Y + 2);
	if (character.getHP().Max < 1000)std::cout << ' ';
	if (character.getHP().Max < 100)std::cout << ' ';
	if (character.getHP().Max < 10)std::cout << ' ';
	std::cout << character.getHP().Max;
	//-----
	gotoxy(topLeft.X + 1, topLeft.Y + 3);
	
	if (typeid(character) == typeid(Warrior))setcolor(Yellow);
	else if (typeid(character) == typeid(Mage))setcolor(LightBlue);
	else if (typeid(character) == typeid(Paladin))setcolor(Magenta);
	else setcolor(Black);

	std::cout << character.getRes().Curr << "    ";
	gotoxy(topLeft.X + DEF_FRAME_WIDTH - 4, topLeft.Y + 3);
	if (character.getRes().Max < 1000)std::cout << ' ';
	if (character.getRes().Max < 100)std::cout << ' ';
	if (character.getRes().Max < 10)std::cout << ' ';
	std::cout << character.getRes().Max;

	setcolor(White);
	return true;
}

bool Graphics::drawPlayerUI(Player &player, const Coord &topLeft)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || topLeft.X + DEF_CHARACTER_WIDTH >= DEF_CONSOLE_WIDTH || topLeft.Y + DEF_CHARACTER_HEIGHT >= DEF_CONSOLE_HEIGHT) return false;
	//clearBoarder(tlx, tly, tlx + DEF_CHARACTER_WIDTH, tly + DEF_CHARACTER_HEIGHT);
	drawBoarder(topLeft, { topLeft.X + DEF_CHARACTER_WIDTH, topLeft.Y + DEF_CHARACTER_HEIGHT });
	drawPlayer(player, topLeft);

	player.setEqAbilityXY(0, { topLeft.X, topLeft.Y + DEF_CHARACTER_HEIGHT + 1 });
	player.getEqAbility(0).showBox(player.getTotalDamageStats());
	player.setEqAbilityXY(1, { topLeft.X + DEF_ABILITY_SIZE * 2 + 1, topLeft.Y + DEF_CHARACTER_HEIGHT + 1 });
	player.getEqAbility(1).showBox(player.getTotalDamageStats());
	player.setEqAbilityXY(2, { topLeft.X + 2 * (DEF_ABILITY_SIZE * 2 + 1) + 1, topLeft.Y + DEF_CHARACTER_HEIGHT + 1 });
	player.getEqAbility(2).showBox(player.getTotalDamageStats());
	player.setEqAbilityXY(3, { topLeft.X + 3 * (DEF_ABILITY_SIZE * 2 + 1) + 1, topLeft.Y + DEF_CHARACTER_HEIGHT + 1 });
	player.getEqAbility(3).showBox(player.getTotalDamageStats());

	player.setHelmetXY({ topLeft.X + DEF_CHARACTER_WIDTH + 1, topLeft.Y });
	player.getHelmet().showBox();
	player.setShouldersXY({ topLeft.X + DEF_CHARACTER_WIDTH + 1, topLeft.Y + DEF_ITEM_SIZE + 1 });
	player.getShoulders().showBox();
	player.setChestXY({ topLeft.X + DEF_CHARACTER_WIDTH + 1, topLeft.Y + 2 * (DEF_ITEM_SIZE + 1) });
	player.getChest().showBox();
	player.setGlovesXY({ topLeft.X + DEF_CHARACTER_WIDTH + 1, topLeft.Y + 3 * (DEF_ITEM_SIZE + 1) });
	player.getGloves().showBox();
	player.setLegsXY({ topLeft.X + DEF_CHARACTER_WIDTH + 1, topLeft.Y + 4 * (DEF_ITEM_SIZE + 1) });
	player.getLegs().showBox();
	player.setFeetXY({ topLeft.X + DEF_CHARACTER_WIDTH + 1, topLeft.Y + 5 * (DEF_ITEM_SIZE + 1) });
	player.getFeet().showBox();
	player.setWeaponXY({ topLeft.X + DEF_CHARACTER_WIDTH + 1, topLeft.Y + 6 * (DEF_ITEM_SIZE + 1) + 2 });
	player.getWeapon().showBox();
	return true;
}

bool Graphics::drawPlayer(const Player &player, const Coord &topLeft)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || topLeft.X + DEF_CHARACTER_WIDTH >= DEF_CONSOLE_WIDTH || topLeft.Y + DEF_CHARACTER_HEIGHT >= DEF_CONSOLE_HEIGHT) return false;
	bool hasWeapon = (player.getWeapon().getID() >= 0);
	if (!hasWeapon)
	{
		drawHelmet(player.getHelmet(), { topLeft.X + 14, topLeft.Y + 2 });
		drawShoulders(player.getShoulders(), { topLeft.X + 7, topLeft.Y + 7 });
		drawChest(player.getChest(), { topLeft.X + 12, topLeft.Y + 7 });
		drawGloves(player.getGloves(), { topLeft.X + 7, topLeft.Y + 10 });
		drawLegs(player.getLegs(), { topLeft.X + 12, topLeft.Y + 14 });
		drawFeet(player.getFeet(), { topLeft.X + 10, topLeft.Y + 20 });
	}
	else
	{
		drawHelmet(player.getHelmet(), { topLeft.X + 9, topLeft.Y + 2});
		drawShoulders(player.getShoulders(), { topLeft.X + 2, topLeft.Y + 7 });
		drawChest(player.getChest(), { topLeft.X + 7, topLeft.Y + 7 });
		drawGloves(player.getGloves(), { topLeft.X + 2, topLeft.Y + 10 });
		drawLegs(player.getLegs(), { topLeft.X + 7, topLeft.Y + 14 });
		drawFeet(player.getFeet(), { topLeft.X + 5, topLeft.Y + 20 });
		//std::cout << "here";system("pause");
		drawWeapon(player.getWeapon(), { topLeft.X + 27, topLeft.Y + 4 });
	}
	return true;
}

bool Graphics::drawEnemy(const Enemy &enemy, const Coord &topLeft)
{
	char id[MAX_ID_LENGHT];
	int width;
	string temp = "Data/Enemies/Graphics/";
	std::vector<string> text = fileToText(temp + _itoa(enemy.getID(), id, 10), width);
	if (text.size() > DEF_ENEMY_HEIGHT || width > DEF_ENEMY_WIDTH) return false;
	return drawPrecisely(text, {DEF_CONSOLE_WIDTH - 1 - width, DEF_CONSOLE_HEIGHT - (int)text.size() - 1});
}

bool Graphics::drawCharacterInfo(const Player &player, const Coord &topLeft)
{
	if (topLeft.X < 0 || topLeft.Y < 0 || false || false)return false; //и аз не знам
	clearBoarder({ topLeft.X, topLeft.Y }, { topLeft.X + DEF_CHAR_INFO_WIDTH, topLeft.Y + DEF_CHAR_INFO_HEIGH });
	drawBoarder({ topLeft.X, topLeft.Y }, { topLeft.X + DEF_CHAR_INFO_WIDTH, topLeft.Y + DEF_CHAR_INFO_HEIGH });

	setcolor(White);
	gotoxy(topLeft.X + 2, topLeft.Y + 1);std::cout << player.getName() << '(' << player.getLevel() << ')';
	if (player.getLevel() < MAX_LEVEL) { gotoxy(topLeft.X + 2, topLeft.Y + 2);std::cout << "XP: " << player.getXP();std::cout << '/' << DEF_LEVEL_EXP[player.getLevel() - 1]; }
	gotoxy(topLeft.X + 2, topLeft.Y + 3);std::cout << "Gold: " << player.getGold();
	gotoxy(topLeft.X + 2, topLeft.Y + 4);std::cout << "Health: " << player.getHP().Max;
	gotoxy(topLeft.X + 2, topLeft.Y + 5);std::cout << "Physical Damage: ";setcolor(LightRed);std::cout << player.getTotalDamageStats().Physical;setcolor(White);
	gotoxy(topLeft.X + 2, topLeft.Y + 6);std::cout << "Magical Damage: ";setcolor(LightBlue);std::cout << player.getTotalDamageStats().Magical;setcolor(White);
	gotoxy(topLeft.X + 2, topLeft.Y + 7);std::cout << "Armor: ";setcolor(LightRed);std::cout << player.getTotalDefenceStats().Armor;setcolor(White);
	gotoxy(topLeft.X + 2, topLeft.Y + 8);std::cout << "Magic Resist: ";setcolor(LightBlue);std::cout << player.getTotalDefenceStats().MagicResist;setcolor(White);

	return true;
}

void Graphics::drawWeapon(const Weapon &weapon, const Coord &topLeft)
{
	if (weapon.getID() < 0)setcolor(White);
	else if (weapon.getMinLevel() < 2)setcolor(Yellow);
	else if (weapon.getMinLevel() < 4)setcolor(LightGray);
	else if (weapon.getMinLevel() < 6)setcolor(LightYellow);
	else if (weapon.getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);

	if (weapon.getWeaponType() == Axe)drawAxe(topLeft);
	else drawStaff(topLeft);

	setcolor(White);
}

void Graphics::drawHelmet(const Armor &helmet, const Coord &topLeft)
{
	if (helmet.getID() < 1)setcolor(White);
	else if (helmet.getMinLevel() < 2)setcolor(Yellow);
	else if (helmet.getMinLevel() < 4)setcolor(LightGray);
	else if (helmet.getMinLevel() < 6)setcolor(LightYellow);
	else if (helmet.getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Helmet", width);
	drawPrecisely(text, topLeft);
	
	setcolor(White);
	gotoxy(topLeft.X + 2, topLeft.Y + 1);std::cout << '*';
	gotoxy(topLeft.X + 6, topLeft.Y + 1);std::cout << '*';
	gotoxy(topLeft.X + 3, topLeft.Y + 3);std::cout << "***";
}

void Graphics::drawShoulders(const Armor &shoulders, const Coord &topLeft)
{
	if (shoulders.getID() < 1)setcolor(White);
	else if (shoulders.getMinLevel() < 2)setcolor(Yellow);
	else if (shoulders.getMinLevel() < 4)setcolor(LightGray);
	else if (shoulders.getMinLevel() < 6)setcolor(LightYellow);
	else if (shoulders.getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Shoulders", width);
	drawPrecisely(text, topLeft);
	setcolor(White);
}

void Graphics::drawChest(const Armor &chest, const Coord &topLeft)
{
	if (chest.getID() < 1)setcolor(White);
	else if (chest.getMinLevel() < 2)setcolor(Yellow);
	else if (chest.getMinLevel() < 4)setcolor(LightGray);
	else if (chest.getMinLevel() < 6)setcolor(LightYellow);
	else if (chest.getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Chest", width);
	drawPrecisely(text, topLeft);
	setcolor(White);
}

void Graphics::drawGloves(const Armor &gloves, const Coord &topLeft)
{
	if (gloves.getID()<1)setcolor(White);
	else if (gloves.getMinLevel() < 2)setcolor(Yellow);
	else if (gloves.getMinLevel() < 4)setcolor(LightGray);
	else if (gloves.getMinLevel() < 6)setcolor(LightYellow);
	else if (gloves.getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Gloves", width);
	drawPrecisely(text, topLeft);
	setcolor(White);
}

void Graphics::drawLegs(const Armor &legs, const Coord &topLeft)
{
	if (legs.getID() < 1)setcolor(White);
	else if (legs.getMinLevel() < 2)setcolor(Yellow);
	else if (legs.getMinLevel() < 4)setcolor(LightGray);
	else if (legs.getMinLevel() < 6)setcolor(LightYellow);
	else if (legs.getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Legs", width);
	drawPrecisely(text, topLeft);
	setcolor(White);
}

void Graphics::drawFeet(const Armor &feet, const Coord &topLeft)
{
	if (feet.getID() < 1)setcolor(White);
	else if (feet.getMinLevel() < 2)setcolor(Yellow);
	else if (feet.getMinLevel() < 4)setcolor(LightGray);
	else if (feet.getMinLevel() < 6)setcolor(LightYellow);
	else if (feet.getMinLevel() < 8)setcolor(LightCyan);
	else setcolor(LightMagenta);
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Feet", width);
	drawPrecisely(text, topLeft);
	setcolor(White);
}

void Graphics::drawAxe(const Coord &topLeft)
{
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Sword", width);
	drawPrecisely(text, topLeft);
}

void Graphics::drawStaff(const Coord &topLeft)
{
	int width;
	std::vector<string> text = fileToText("Data/Graphics/Items/Staff", width);
	drawPrecisely(text, topLeft);
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

void Graphics::drawMapChooseUI()
{
	setFontSize(DEF_CONSOLE_SIZE * 2);
	SetConsoleWindowSize(DEF_CONSOLE_WIDTH / 2, DEF_CONSOLE_HEIGHT / 2);
	clearscreen();

	gotoxy((DEF_CONSOLE_WIDTH / 2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 - 2);std::cout << "Choose map:";
	gotoxy((DEF_CONSOLE_WIDTH / 2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 - 1);std::cout << "-Elwynn Forest <E>";
	gotoxy((DEF_CONSOLE_WIDTH / 2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 - 0);std::cout << "-Durotar <D>";
	gotoxy((DEF_CONSOLE_WIDTH / 2 - 18) / 2, DEF_CONSOLE_HEIGHT / 4 + 1);std::cout << "-Isle of Giants <I>";
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
	Engine::setCursorVisible(true);
	gotoxy(DEF_CONSOLE_WIDTH / 4 - 12 / 2, DEF_CONSOLE_HEIGHT / 4 - 1);
	std::cout << "Enter name: ";
}

void Graphics::drawHomeUI(Player &player, Box **boxes, int num, const Coord &topLeft, const Coord &botRight)
{
	//clearBoarder(0, 0, DEF_FREE_BEG, DEF_CONSOLE_HEIGHT - 1);
	drawPlayerUI(player, { 0, 10 });

	if (topLeft.X <= DEF_CHAR_INFO_WIDTH && topLeft.Y <= DEF_CHAR_INFO_HEIGH)
	{
		drawCharacterInfo(player, { 0, 0 });
	}
	if (botRight.X <= DEF_FREE_BEG && topLeft.Y <= DEF_CHAR_INFO_HEIGH)
	{
		clearBoarder({ DEF_CHAR_INFO_WIDTH + 1, 0 }, { DEF_CHAR_INFO_WIDTH + 19, DEF_CHAR_INFO_HEIGH });
		drawBoarder({ DEF_CHAR_INFO_WIDTH + 1, 0 }, { DEF_CHAR_INFO_WIDTH + 19, DEF_CHAR_INFO_HEIGH });

		gotoxy(DEF_CHAR_INFO_WIDTH + 3, 1);std::cout << "Menu:";
		gotoxy(DEF_CHAR_INFO_WIDTH + 3, 3);std::cout << "Home <H>";
		gotoxy(DEF_CHAR_INFO_WIDTH + 3, 4);std::cout << "Play <P>";
		gotoxy(DEF_CHAR_INFO_WIDTH + 3, 5);std::cout << "Shop <S>";
		gotoxy(DEF_CHAR_INFO_WIDTH + 3, 6);std::cout << "Inventory <I>";
		gotoxy(DEF_CHAR_INFO_WIDTH + 3, 7);std::cout << "AbilityBook <A>";
		gotoxy(DEF_CHAR_INFO_WIDTH + 3, 8);std::cout << "Exit <E>";
	}
}

void Graphics::drawInventoryUI(const Player &player, Box **boxes, int num, Button &equipItem, Button &sellItem, const Coord &topLeft, const Coord &botRight)
{
	int totalLenX = 20 * (DEF_ITEM_SIZE + 1);
	int totalLenY = 5 * (DEF_ITEM_SIZE + 1);

	drawBoarder({ ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 1 + TEMP_FIX }, 
		{ ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + TEMP_FIX });
	equipItem.showButton();
	sellItem.showButton();
	drawBoxesInRange(boxes, num, topLeft, botRight);

	if (topLeft.Y <= 10)drawTextFromFile("Data/Graphics/InventoryLogo");
}

void Graphics::drawShopUI(const Player &player, Box **boxes, int num, Button &buyItem, const Coord &topLeft, const Coord &botRight)
{
	int totalLenX = 20 * (DEF_ITEM_SIZE + 1);
	int totalLenY = 5 * (DEF_ITEM_SIZE + 1);

	drawBoarder({ ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 1 + TEMP_FIX }, 
		{ ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + TEMP_FIX });
	buyItem.showButton();
	drawBoxesInRange(boxes, num, topLeft, botRight);

	if (topLeft.Y <= 10)drawTextFromFile("Data/Graphics/ShopLogo");
}

void Graphics::drawAbilityBookUI(const Player &player, Box **boxes, int num, Button &eqSlot1, Button &eqSlot2, Button &eqSlot3, Button &eqSlot4, const Coord &topLeft, const Coord &botRight)
{
	int totalLenX = 6 * (DEF_ABILITY_SIZE + 1);
	int totalLenY = 3 * (DEF_ABILITY_SIZE + 1);

	drawBoarder({ ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 6 + 2 * TEMP_FIX + 1 }, 
		{ ((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 - 5 + 2 * TEMP_FIX + 1 });
	eqSlot1.showButton();
	eqSlot2.showButton();
	eqSlot3.showButton();
	eqSlot4.showButton();
	drawBoxesInRange(boxes, num, topLeft, botRight, player.getTotalDamageStats());

	if (topLeft.Y <= 10)drawTextFromFile("Data/Graphics/AbilityBookLogo");
}

void Graphics::drawMap(const std::pair<int, int> &pos, EnemyBox **enemies, int enemyCnt)
{
	clearscreen();
	gotoxy(0, 0);std::cout << "E";
	gotoxy(pos.first, pos.second);
	std::cout << 'P';
	for (int i = 0;i < enemyCnt;i++)
	{
		if(enemies[i]->getHP().Max>0)
		enemies[i]->showBox();//std::cout << enemies[i]->getHP().Curr;
	}
}

void Graphics::drawPlay(const Player &player, const Enemy &enemy)
{
	clearscreen();
	gotoxy(0, 22);for (int i = 0;i < DEF_CONSOLE_WIDTH;i++)std::cout << (char)126;//196;
	drawGrass({ 0, 23 }, { DEF_CONSOLE_WIDTH, DEF_CONSOLE_HEIGHT });
	drawCloud({ 40, 0 });
	drawCloud({ 94, 4 });
	
	drawPlayer(player, { 0, 15 });
	drawEnemy(enemy, { DEF_CONSOLE_WIDTH - 38 - 10, 12 - 3 });
	drawFrame(player, { 3, 3 });
	drawFrame(enemy, { DEF_CONSOLE_WIDTH - 25, 3 });
}

void Graphics::drawResultsUI(Player &player, const Enemy &enemy)
{
	Graphics::setFontSize(DEF_CONSOLE_SIZE * 2);
	Graphics::SetConsoleWindowSize(DEF_CONSOLE_WIDTH / 2, DEF_CONSOLE_HEIGHT / 2);
	Graphics::clearscreen();
	if (!player.isAlive())
	{
		std::cout << "You lost!\n";
	}
	else
	{
		std::cout << "You won!\n";
		std::cout << "You received " << enemy.getGold() << " gold\n";
		player.gainGold(enemy.getGold());
		std::cout << "You received " << enemy.getXP() << " XP\n";
		int currGold = player.getGold();
		if (player.gainXP(enemy.getXP()))
		{
			std::cout << "You reaced " << player.getLevel() << " level!\n";
			std::cout << "You received bonus " << player.getGold() - currGold << " gold!\n";
		}
	}
	std::cout << "Press <H> to return to the home page!";
}
