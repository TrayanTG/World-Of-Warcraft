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