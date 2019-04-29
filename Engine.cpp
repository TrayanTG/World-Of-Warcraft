#include "Engine.h"
#include "Graphics.h"
#include "Data.h"
#include "HeadersAndConsts.h"
#include "PlayerClasses.h"

Engine Engine::s;

Engine::Engine()
{
	Data::getIstance().loadItems();
	Data::getIstance().loadAbilities();
	Data::getIstance().loadEnemies();

	srand((size_t)time(0));

	cci.dwSize = 25;
	//cci.bVisible = TRUE;
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

void Engine::initMouse()
{
	cci.dwSize = 25;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

void Engine::setCursorVisible(bool isVisible)
{
	if(!isVisible)cci.bVisible = FALSE;
	else cci.bVisible = TRUE;
}

void Engine::updateCursor()
{
	while (true)
	{
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (InputRecord.EventType == MOUSE_EVENT)
		{
			coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
			coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			/*if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) ///RIGHTMOST_BUTTON_PRESSED
			{
				SetConsoleCursorPosition(hout, coord);
				SetConsoleTextAttribute(hout, rand() % 7 + 9);

				//std::cout << "Hello world at " << InputRecord.Event.MouseEvent.dwMousePosition.X << " x " << InputRecord.Event.MouseEvent.dwMousePosition.Y << " ";
			}
			else if (InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				Graphics::getInstance().clearscreen();
			}*/
		}
		/*if (InputRecord.EventType == KEY_EVENT)
		{
			SetConsoleCursorPosition(hout, coord);
			SetConsoleTextAttribute(hout, rand() % 7 + 9);

			//std::cout << coord.X << ' ' << coord.Y << ' ' << InputRecord.Event.KeyEvent.wVirtualKeyCode << std::endl;
		}*/
		//FlushConsoleInputBuffer(hin);
	}
}

void Engine::logIn()
{
	char chooseClass, chooseNewExist, name[MAX_NAME_LENGHT];
// ---------------------------------------------
	Graphics::getInstance().drawClassChooseUI();
	do
	{
		chooseClass = _getch();
		if (chooseClass >= 'a')chooseClass -= ('a' - 'A');
	} while (chooseClass != 'W' && chooseClass != 'M' && chooseClass != 'P');
// ---------------------------------------------
	Graphics::getInstance().drawNewOldUI();
	do
	{
		chooseNewExist = _getch();
		if (chooseNewExist >= 'a')chooseNewExist -= ('a' - 'A');
	} while (chooseNewExist != 'N' && chooseNewExist != 'E');
// ---------------------------------------------
	Graphics::getInstance().drawEnterName();
	Graphics::getInstance().setcolor(White);
	std::cin >> name;
// ---------------------------------------------
	
	if (chooseClass == 'W')myPlayer = new Warrior(name);
	else if (chooseClass == 'P')myPlayer = new Paladin(name);
	else myPlayer = new Mage(name);

	char path[MAX_PATH_LENGHT];
	strcpy(directory, "Data/Logs/");
	if (chooseClass == 'W')strcat(directory, "Warriors/");
	else if (chooseClass == 'P')strcat(directory, "Paladins/");
	else strcat(directory, "Mages/");
	strcat(directory, name);

	if (chooseNewExist == 'N')
	{
		_mkdir(directory);
		myPlayer->savePlayer(strcat(directory, "/"));
	}
	else
	{
		strcpy(path, directory);
		std::ifstream iFile(strcat(path,"/CharacterInfo"));
		if (!iFile)
		{
			Graphics::getInstance().clearscreen();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			exit(-1);
		}
		myPlayer->loadPlayer(strcat(directory, "/"));
		iFile.close();
	}
	Graphics::getInstance().init();
}

void Engine::Home()
{
	Box *boxes[16];
	int cntBoxes = 0, currBox=-1;

	{
		for (int i = 0;i < 4;i++)
		{
			if (myPlayer->eqAbilities[i]->getID() >= 0)
			{
				boxes[cntBoxes++] = myPlayer->eqAbilities[i];
			}
		}
		if (myPlayer->weapon->getID() >= 0)boxes[cntBoxes++] = myPlayer->weapon;
		if (myPlayer->helmet->getID() >= 0)boxes[cntBoxes++] = myPlayer->helmet;
		if (myPlayer->chest->getID() >= 0)boxes[cntBoxes++] = myPlayer->chest;
		if (myPlayer->shoulders->getID() >= 0)boxes[cntBoxes++] = myPlayer->shoulders;
		if (myPlayer->gloves->getID() >= 0)boxes[cntBoxes++] = myPlayer->gloves;
		if (myPlayer->legs->getID() >= 0)boxes[cntBoxes++] = myPlayer->legs;
		if (myPlayer->feet->getID() >= 0)boxes[cntBoxes++] = myPlayer->feet;
	}
	
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	Graphics::getInstance().drawHomeUI(*myPlayer);
	while (true)
	{
		std::this_thread::sleep_until(tp);
		tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(250);
		myPlayer->savePlayer(directory);
		if (InputRecord.EventType == KEY_EVENT)
		{
			switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
			{
			case 'S': std::cout << "SHOP";break;
			case 'I': Inventory(); break;
			case 'P': std::cout << "PLAY";break;
			case 'A': std::cout << "ABILITIES";break;
			case 'E': return;
			}
		}
		else if (InputRecord.EventType == MOUSE_EVENT)
		{
			if (currBox == -1)
			{
				for (int i = 0;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(coord.X, coord.Y))
					{
						boxes[i]->toggleInfoBox();
						currBox = i;
					}
				}
			}
			else
			{
				if (!boxes[currBox]->isWithin(coord.X, coord.Y))
				{
					boxes[currBox]->hideInfoBox();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					currBox = -1;
				}
			}
		}
	}
}

void Engine::Inventory()
{
	Box *boxes[128];
	int cntBoxes = 0, currBox = -1, invBoxes = -1;

	{
		for (int i = 0;i < 4;i++)
		{
			if (myPlayer->eqAbilities[i]->getID() >= 0)
			{
				boxes[cntBoxes++] = myPlayer->eqAbilities[i];
			}
		}
		if (myPlayer->weapon->getID() >= 0)boxes[cntBoxes++] = myPlayer->weapon;
		if (myPlayer->helmet->getID() >= 0)boxes[cntBoxes++] = myPlayer->helmet;
		if (myPlayer->chest->getID() >= 0)boxes[cntBoxes++] = myPlayer->chest;
		if (myPlayer->shoulders->getID() >= 0)boxes[cntBoxes++] = myPlayer->shoulders;
		if (myPlayer->gloves->getID() >= 0)boxes[cntBoxes++] = myPlayer->gloves;
		if (myPlayer->legs->getID() >= 0)boxes[cntBoxes++] = myPlayer->legs;
		if (myPlayer->feet->getID() >= 0)boxes[cntBoxes++] = myPlayer->feet;
		invBoxes = cntBoxes;
		for (size_t i = 0;i < myPlayer->items.size();i++)
		{
			bool f = true;
			for (int k = 0;k < cntBoxes;k++)
			{
				if (boxes[k]->getID() == myPlayer->items[i]->getID()) { f = false;break; }
			}
			if(f) boxes[cntBoxes++] = myPlayer->items[i];
		}
	}

	
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer);
	Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes);
	
	while (true)
	{
		std::this_thread::sleep_until(tp);
		tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(250);
		myPlayer->savePlayer(directory);
		if (InputRecord.EventType == KEY_EVENT)
		{
			switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
			{
			case 'S': std::cout << "SHOP";break;
			case 'I': std::cout << "INVENTORY";break;
			case 'P': std::cout << "PLAY";break;
			case 'A': std::cout << "ABILITIES";break;
			case 'E': return;
			}
		}
		else if (InputRecord.EventType == MOUSE_EVENT)
		{
			if (currBox == -1)
			{
				for (int i = 0;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(coord.X, coord.Y))
					{
						boxes[i]->toggleInfoBox();
						currBox = i;
					}
				}
			}
			else
			{
				if (!boxes[currBox]->isWithin(coord.X, coord.Y))
				{
					boxes[currBox]->hideInfoBox();
					if (currBox < invBoxes) Graphics::getInstance().drawHomeUI(*myPlayer);
					else Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes);
					currBox = -1;
				}
			}
		}
	}
}