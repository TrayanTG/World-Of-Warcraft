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

void Engine::initHome(Box **boxes, int &cntBoxes, int &currBox)
{
	cntBoxes = 0;
	currBox = -1;
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

void Engine::initInventory(Box **boxes, Button &equipItem, Button &sellItem, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox)
{
	int totalLenX = 20 * (DEF_ITEM_SIZE + 1);
	int totalLenY = 5 * (DEF_ITEM_SIZE + 1);

	cntBoxes = 0;
	currBox = invBoxes = markedBox = -1;
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
			if (f) boxes[cntBoxes++] = myPlayer->items[i];
		}
	}
	
	equipItem.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1);
	sellItem.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2 - 12, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1);

	for (int i = invBoxes;i < cntBoxes;i++)
	{
		boxes[i]->setXY(DEF_FREE_BEG + 19 + 2 * ((i - invBoxes) % 10) * (DEF_ITEM_SIZE + 1), 10 + ((i - invBoxes) / 10) * (DEF_ITEM_SIZE + 1));
	}
}

void Engine::initShop(Box **boxes, Button &buyItem, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox)
{
	int totalLenX = 20 * (DEF_ITEM_SIZE + 1);
	int totalLenY = 5 * (DEF_ITEM_SIZE + 1);

	cntBoxes = 0;
	currBox = invBoxes = markedBox = -1;

	{
		for (int i = 0;i < 4;i++)
		{
			if (myPlayer->eqAbilities[i]->getID() >= 0)
			{
				boxes[cntBoxes++] = myPlayer->eqAbilities[i];
			}
		}
		for (size_t i = 0;i < myPlayer->items.size();i++)
		{
			boxes[cntBoxes++] = myPlayer->items[i];
		}
		invBoxes = cntBoxes;
		for (size_t i = 0;i < Data::getIstance().items.size();i++)
		{
			bool f = true;
			for (int k = 0; k < invBoxes; k++)
			{
				if (Data::getIstance().items[i]->getID() == boxes[k]->getID()) f = false;
			}
			if (f) boxes[cntBoxes++] = Data::getIstance().items[i];
		}
	}

	for (int i = invBoxes;i < cntBoxes;i++)
	{
		boxes[i]->setXY(DEF_FREE_BEG + 19 + 2 * ((i - invBoxes) % 10) * (DEF_ITEM_SIZE + 1), 10 + ((i - invBoxes) / 10) * (DEF_ITEM_SIZE + 1));
	}

	buyItem.setXY((DEF_CONSOLE_WIDTH + DEF_FREE_BEG - buyItem.getLen() - 2) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 3);
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
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
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
	char t = 0;
	Box *boxes[16];
	int cntBoxes, currBox;
	
	initHome(boxes, cntBoxes, currBox);
	
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	Graphics::getInstance().clearscreen();
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
			case 'S': t = 'S'; break;
			case 'I': t = 'I'; break;
			case 'P': t = 'P'; break;
			case 'A': t = 'A'; break;
			case 'E': exit(0);
			}
			if (t)break;
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
	for (int i = 0;i < cntBoxes;i++)boxes[i]->setXY(-1, -1);
	if (t == 'I')Inventory();
	if (t == 'S')Shop();
}

void Engine::Inventory()
{
	char t = 0;
	Box *boxes[128];
	Button equipItem("  Equip  ");
	Button sellItem("Sell(50%)");
	int cntBoxes = 0, currBox = -1, invBoxes = -1, markedBox = -1;

	initInventory(boxes, equipItem, sellItem, cntBoxes, currBox, invBoxes, markedBox);
	
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer);
	Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes-invBoxes, equipItem, sellItem);
	
	while (true)
	{
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			if (markedBox == -1)
			{
				for (int i = invBoxes;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(coord.X, coord.Y))
					{
						markedBox = i;
						boxes[markedBox]->setMarked(true);
						Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, equipItem, sellItem);
						boxes[markedBox]->toggleInfoBox();
					}
				}
			}
			else
			{
				boxes[markedBox]->setMarked(false);
				if (equipItem.isWithin(coord.X, coord.Y))
				{
					myPlayer->equipItem((Item*)boxes[markedBox]);
					initInventory(boxes, equipItem, sellItem, cntBoxes, currBox, invBoxes, markedBox);
					Graphics::getInstance().clearscreen();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, equipItem, sellItem);
					markedBox = -1;
				}
				else if (sellItem.isWithin(coord.X, coord.Y))
				{
					myPlayer->sellItem((Item*)boxes[markedBox]);
					for (int i = markedBox + 1;i < cntBoxes;i++)
					{
						boxes[i - 1] = boxes[i];
					}
					cntBoxes--;
					Graphics::getInstance().clearscreen();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, equipItem, sellItem);
					markedBox = -1;
				}
				else if (boxes[markedBox]->isWithin(coord.X, coord.Y))
				{
					markedBox = -1;
				}
				else
				{
					for (int i = invBoxes;i < cntBoxes;i++)
					{
						if (boxes[i]->isWithin(coord.X, coord.Y))
						{
							markedBox = i;
							boxes[markedBox]->setMarked(true);
							Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, equipItem, sellItem);
							boxes[markedBox]->toggleInfoBox();
						}
					}
				}
			}
		}
		//std::this_thread::sleep_until(tp);
		if (tp > std::chrono::steady_clock::now())continue;
		tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(25);
		myPlayer->savePlayer(directory);
		
		if (InputRecord.EventType == KEY_EVENT)
		{
			switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
			{
			case 'S': t = 'S'; break;
			case 'H': t = 'H'; break;
			case 'P': t = 'P'; break;
			case 'A': t = 'A'; break;
			case 'E': exit(0);
			}
			if (t)break;
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
					else Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes-invBoxes, equipItem, sellItem);
					currBox = -1;
				}
			}
		}
	}
	equipItem.setXY(-1, -1);
	sellItem.setXY(-1, -1);
	for (int i = 0;i < cntBoxes;i++)boxes[i]->setXY(-1, -1);
	if (t == 'H')Home();
	if (t == 'S')Shop();
}

void Engine::Shop()
{
	char t = 0;
	Box *boxes[128];
	Button buyItem("   Buy   ");
	int cntBoxes, currBox, invBoxes, markedBox;

	initShop(boxes, buyItem, cntBoxes, currBox, invBoxes, markedBox);

	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer);
	Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem);
	
	while (true)
	{
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			if (markedBox == -1)
			{
				for (int i = invBoxes;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(coord.X, coord.Y))
					{
						markedBox = i;
						boxes[markedBox]->setMarked(true);
						Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem);
						boxes[markedBox]->toggleInfoBox();
					}
				}
			}
			else
			{
				boxes[markedBox]->setMarked(false);
				if (buyItem.isWithin(coord.X, coord.Y))
				{
					myPlayer->buyItem((Item*)boxes[markedBox]);
					initShop(boxes, buyItem, cntBoxes, currBox, invBoxes, markedBox);
					Graphics::getInstance().clearscreen();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem);
					markedBox = -1;
				}
				else if (boxes[markedBox]->isWithin(coord.X, coord.Y))
				{
					markedBox = -1;
				}
				else
				{
					for (int i = invBoxes;i < cntBoxes;i++)
					{
						if (boxes[i]->isWithin(coord.X, coord.Y))
						{
							markedBox = i;
							boxes[markedBox]->setMarked(true);
							Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem);
							boxes[markedBox]->toggleInfoBox();
						}
					}
				}
			}
		}
		if (tp > std::chrono::steady_clock::now())continue;
		tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(250);
		myPlayer->savePlayer(directory);

		if (InputRecord.EventType == KEY_EVENT)
		{
			switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
			{
			case 'H': t = 'H'; break;
			case 'I': t = 'I'; break;
			case 'P': t = 'P'; break;
			case 'A': t = 'A'; break;
			case 'E': exit(0);
			}
			if (t)break;
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
					else Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem);
					currBox = -1;
				}
			}
		}
	}
	buyItem.setXY(-1, -1);
	for (int i = 0;i < cntBoxes;i++)boxes[i]->setXY(-1, -1);
	if (t == 'H')Home();
	if (t == 'I')Inventory();
}