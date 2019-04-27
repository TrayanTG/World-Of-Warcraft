#include "Engine.h"
#include "Graphics.h"

Engine Engine::s;

Engine::Engine()
{
	cci.dwSize = 25;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

Engine::~Engine()
{

}

Engine& Engine::getInstance()
{
	return s;
}

void Engine::init()
{
	cci.dwSize = 25;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

void Engine::updateCursor()
{
	while (true)
	{
		ReadConsoleInput(hin, &InputRecord, 1, &Events);

		if (InputRecord.EventType == MOUSE_EVENT)
		{
			coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
			coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) ///RIGHTMOST_BUTTON_PRESSED
			{
				SetConsoleCursorPosition(hout, coord);
				SetConsoleTextAttribute(hout, rand() % 7 + 9);

				std::cout << "Hello world at " << InputRecord.Event.MouseEvent.dwMousePosition.X << " x " << InputRecord.Event.MouseEvent.dwMousePosition.Y << " ";
			}
			else if (InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				Graphics::getInstance().clearscreen();
			}
		}
		if (InputRecord.EventType == KEY_EVENT)
		{
			SetConsoleCursorPosition(hout, coord);
			SetConsoleTextAttribute(hout, rand() % 7 + 9);

			std::cout << coord.X << ' ' << coord.Y << ' ' << InputRecord.Event.KeyEvent.wVirtualKeyCode << std::endl;
		}
		//FlushConsoleInputBuffer(hin);
	}
}
