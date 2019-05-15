#include "Engine.h"
#include "Graphics.h"
#include "Data.h"
#include "HeadersAndConsts.h"
#include "PlayerClasses.h"

Engine Engine::s;

Engine::Engine()
{
	cheatOn = false;
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

void Engine::randomizeEnemies(EnemyBox **enemies, int enemyCnt)
{
	bool f;
	int x, y;
	for (int i = 0;i < enemyCnt;i++)
	{
		f = true;
		x = 10 + rand() % (DEF_CONSOLE_WIDTH - 10 - 2 * (DEF_ENEMY_SIZE + 1));
		y = 5 + rand() % (DEF_CONSOLE_HEIGHT - 5 - (DEF_ENEMY_SIZE + 1));
		for (int j = 0;j < i;j++)
		{

			if (enemies[j]->isWithin(x, y, 2 * DEF_ENEMY_SIZE)){		
				i--;
				f = false;
				break;
			}
		}
		if (f) enemies[i]->setXY(x, y);
	}
}

void Engine::enemyMove(EnemyBox **enemies, int &enemyCnt, int currEnemy)
{
	for (int i = 0;i < enemyCnt;i++)
	{
		int t = rand() % 5;
		
		switch (t)
		{
			case 0: break;
			case 1:
			{
				if (enemies[i]->getTLX() < 5) break;
				bool f = true;
				for (int j = 0;j < enemyCnt;j++)
				{
					if (j == i)continue;
					if (enemies[j]->isWithin(enemies[i]->getTLX() - 4, enemies[i]->getTLY(), DEF_ENEMY_SIZE))
					{
						f = false;
						break;
					}
				}
				if (f)
				{
					enemies[i]->hideBox();
					if (i == currEnemy)
					{
						enemies[i]->hideInfoBox();
						for (int j = 0;j < i;j++)enemies[j]->showBox();
					}
					enemies[i]->setXY(enemies[i]->getTLX() - 4, enemies[i]->getTLY());
					enemies[i]->showBox();
				}
				break;
			}
			case 2:
			{
				if (enemies[i]->getTLX() >= DEF_CONSOLE_WIDTH - 2 * (DEF_ENEMY_SIZE + 1)) break;
				bool f = true;
				for (int j = 0;j < enemyCnt;j++)
				{
					if (j == i)continue;
					if (enemies[j]->isWithin(enemies[i]->getTLX() + 4, enemies[i]->getTLY(), DEF_ENEMY_SIZE))
					{
						f = false;
						break;
					}
				}
				if (f)
				{
					enemies[i]->hideBox();
					if (i == currEnemy)
					{
						enemies[i]->hideInfoBox();
						for (int j = 0;j < i;j++)enemies[j]->showBox();
					}
					enemies[i]->setXY(enemies[i]->getTLX() + 4, enemies[i]->getTLY());
					enemies[i]->showBox();
				}
				break;
			}
			case 3:
			{
				if (enemies[i]->getTLY() <= 0) break;
				bool f = true;
				for (int j = 0;j < enemyCnt;j++)
				{
					if (j == i)continue;
					if (enemies[j]->isWithin(enemies[i]->getTLX(), enemies[i]->getTLY() - 1, DEF_ENEMY_SIZE))
					{
						f = false;
						break;
					}
				}
				if (f)
				{
					enemies[i]->hideBox();
					if (i == currEnemy)
					{
						enemies[i]->hideInfoBox();
						for (int j = 0;j < i;j++)enemies[j]->showBox();
					}
					enemies[i]->setXY(enemies[i]->getTLX(), enemies[i]->getTLY() - 1);
					enemies[i]->showBox();
				}
				break;
			}
			case 4:
			{
				if (enemies[i]->getTLY() >= DEF_CONSOLE_WIDTH - (DEF_ENEMY_SIZE + 1)) break;
				bool f = true;
				for (int j = 0;j < enemyCnt;j++)
				{
					if (j == i)continue;
					if (enemies[j]->isWithin(enemies[i]->getTLX(), enemies[i]->getTLY() + 1, DEF_ENEMY_SIZE))
					{
						f = false;
						break;
					}
				}
				if (f)
				{
					enemies[i]->hideBox();
					if (i == currEnemy)
					{
						enemies[i]->hideInfoBox();
						for (int j = 0;j < i;j++)enemies[j]->showBox();
					}
					enemies[i]->setXY(enemies[i]->getTLX(), enemies[i]->getTLY() + 1);
					enemies[i]->showBox();
				}
				break;
			}
		}

		if (currEnemy>=0 && enemies[currEnemy]->isWithin(coord.X, coord.Y))enemies[currEnemy]->toggleInfoBox();
	}
}

void Engine::initHome(Box **boxes, int &cntBoxes, int &currBox)
{
	for (int i = 0;i < cntBoxes;i++) boxes[i]->setXY(-1, -1);

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

	for (int i = 0;i < cntBoxes;i++) boxes[i]->setXY(-1, -1);

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
				if (typeid(boxes[k]) != typeid(Ability*) && boxes[k]->getID() == myPlayer->items[i]->getID()) { f = false;break; }
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

	for (int i = 0;i < cntBoxes;i++) boxes[i]->setXY(-1, -1);

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
				if (typeid(boxes[k]) != typeid(Ability*) && Data::getIstance().items[i]->getID() == boxes[k]->getID()) f = false;
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

void Engine::initAilityBook(Box **boxes, Button &eqSlot1, Button &eqSlot2, Button &eqSlot3, Button &eqSlot4, int &cntBoxes, int &currBox, int &invBoxes, int &markedBox)
{
	int totalLenX = 6 * (DEF_ABILITY_SIZE + 1);
	int totalLenY = 3 * (DEF_ABILITY_SIZE + 1);

	for (int i = 0;i < cntBoxes;i++) boxes[i]->setXY(-1, -1);

	cntBoxes = 0;
	currBox = invBoxes = markedBox = -1;

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

	for (size_t i = 0;i < myPlayer->abilities.size();i++)
	{
		bool f = true;
		for (int k = 0;k < invBoxes;k++)
		{
			if (typeid(*boxes[k]) == typeid(Ability) && boxes[k]->getID() == myPlayer->abilities[i]->getID()) { f = false;break; }
		}
		if (f) boxes[cntBoxes++] = myPlayer->abilities[i];
	}

	eqSlot1.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 - 4);
	eqSlot2.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2 + eqSlot2.getLen() + 5, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 - 4);
	eqSlot3.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1);
	eqSlot4.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2 + eqSlot2.getLen() + 5, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1);

	for (int i = 0;i < cntBoxes - invBoxes;i++)
	{
		boxes[i+invBoxes]->setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2 + 1 + 2 * (i % 3) * (DEF_ABILITY_SIZE + 1), (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 5 + (i / 3)*(DEF_ABILITY_SIZE + 1));
	}
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
	bool cheatTemp = false;
// ---------------------------------------------
	Graphics::getInstance().drawClassChooseUI();
	do
	{
		chooseClass = _getch();
		if (chooseClass >= 'a')chooseClass -= ('a' - 'A');
		if (chooseClass == 'C') cheatTemp = true;
	} while (chooseClass != 'W' && chooseClass != 'M' && chooseClass != 'P');
// ---------------------------------------------
	Graphics::getInstance().drawNewOldUI();
	do
	{
		chooseNewExist = _getch();
		if (chooseNewExist >= 'a')chooseNewExist -= ('a' - 'A');
		if (cheatTemp && chooseNewExist == 'H') cheatOn = true;
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

void Engine::loadMap(EnemyBox **enemies, int &enemyCnt)
{
	char chooseMap, path[MAX_PATH_LENGHT], temp[4];
	int enemyIDs[MAX_ENEMY_CNT], index = 0;
	Graphics::getInstance().drawMapChooseUI();
	do
	{
		chooseMap = (char)InputRecord.Event.KeyEvent.wVirtualKeyCode;
		if (chooseMap >= 'a')chooseMap -= ('a' - 'A');
	} while (chooseMap != 'E' && chooseMap != 'D' && chooseMap != 'B');
	// ---------------------------------------------
	std::ifstream iFile;
	strcpy(path, "Data/Maps/");
	if (chooseMap == 'E')strcat(path, "ElwynnForest");
	else if (chooseMap == 'D')strcat(path, "Durotar");
	else strcat(path, "BossLands");
	iFile.open(path);
	if (!iFile) exit(-1);
	iFile >> enemyCnt;
	if (enemyCnt > MAX_ENEMY_CNT) exit(-1);
	while (iFile) iFile >> enemyIDs[index++];index--;
	iFile.close();
	for (int i = index;i < enemyCnt;i++) enemyIDs[i] = enemyIDs[rand() % index];
	for (int i = 0;i < enemyCnt;i++)
	{
		enemies[i] = new EnemyBox(temp, *Data::getIstance().getEnemyByID(enemyIDs[i]), myPlayer->getLevel() + rand() % MAX_LEVEL_DIF);
	}
}

void Engine::Home()
{
	char t = 0;
	Box *boxes[16];
	int cntBoxes = 0, currBox = -1;
	
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
	if (t == 'A')AbilityBook();
	if (t == 'P')Map();
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
						for (int i = invBoxes;i < cntBoxes;i++) boxes[i]->setMarked(false);
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
					boxes[markedBox]->showBox();
					markedBox = -1;
				}
				else
				{
					for (int i = invBoxes;i < cntBoxes;i++)
					{
						if (boxes[i]->isWithin(coord.X, coord.Y))
						{
							for (int i = invBoxes;i < cntBoxes;i++) boxes[i]->setMarked(false);
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
	if (t == 'A')AbilityBook();
	if (t == 'P')Map();
}

void Engine::Shop()
{
	char t = 0;
	Box *boxes[128];
	Button buyItem("   Buy   ");
	int cntBoxes = 0, currBox = -1, invBoxes = -1, markedBox = -1;

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
						for (int i = invBoxes;i < cntBoxes;i++) boxes[i]->setMarked(false);
						currBox = markedBox = i;
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
					boxes[markedBox]->showBox();
					markedBox = -1;
				}
				else
				{
					for (int i = invBoxes;i < cntBoxes;i++)
					{
						if (boxes[i]->isWithin(coord.X, coord.Y))
						{
							for (int i = invBoxes;i < cntBoxes;i++) boxes[i]->setMarked(false);
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
			case 'L': if(cheatOn) myPlayer->gainXP(10); break;
			case 'M': if(cheatOn) myPlayer->gainGold(10); break;
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
	if (t == 'A')AbilityBook();
	if (t == 'P')Map();
}

void Engine::AbilityBook()
{
	char t = 0;
	Box *boxes[16];
	Button eqSlot1("Equip slot1");
	Button eqSlot2("Equip slot2");
	Button eqSlot3("Equip slot3");
	Button eqSlot4("Equip slot4");
	int cntBoxes = 0, currBox = -1, invBoxes = -1, markedBox = -1;

	initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);

	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer);
	Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);

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
						for (int i = invBoxes;i < cntBoxes;i++) boxes[i]->setMarked(false);
						markedBox = i;
						boxes[markedBox]->setMarked(true);
						Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
						boxes[markedBox]->toggleInfoBox();
					}
				}
			}
			else
			{
				boxes[markedBox]->setMarked(false);
				if (eqSlot1.isWithin(coord.X, coord.Y))
				{
					myPlayer->equipAbility((Ability*)boxes[markedBox], 0);
					initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
					Graphics::getInstance().clearscreen();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
					markedBox = -1;
				}
				else if (eqSlot2.isWithin(coord.X, coord.Y))
				{
					myPlayer->equipAbility((Ability*)boxes[markedBox], 1);
					initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
					Graphics::getInstance().clearscreen();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
					markedBox = -1;
				}
				else if (eqSlot3.isWithin(coord.X, coord.Y))
				{
					myPlayer->equipAbility((Ability*)boxes[markedBox], 2);
					initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
					Graphics::getInstance().clearscreen();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
					markedBox = -1;
				}
				else if (eqSlot4.isWithin(coord.X, coord.Y))
				{
					myPlayer->equipAbility((Ability*)boxes[markedBox], 3);
					initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
					Graphics::getInstance().clearscreen();
					Graphics::getInstance().drawHomeUI(*myPlayer);
					Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
					markedBox = -1;
				}
				else if (boxes[markedBox]->isWithin(coord.X, coord.Y))
				{
					boxes[markedBox]->showBox(myPlayer->getTotalDamageStats());
					markedBox = -1;
				}
				else
				{
					for (int i = invBoxes;i < cntBoxes;i++)
					{
						if (boxes[i]->isWithin(coord.X, coord.Y))
						{
							for (int i = invBoxes;i < cntBoxes;i++) boxes[i]->setMarked(false);
							markedBox = i;
							boxes[markedBox]->setMarked(true);
							Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
							boxes[markedBox]->toggleInfoBox();
						}
					}
				}
			}
		}
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
			case 'I': t = 'I'; break;
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
					else Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
					currBox = -1;
				}
			}
		}
	}

	eqSlot1.setXY(-1, -1);
	eqSlot2.setXY(-1, -1);
	eqSlot3.setXY(-1, -1);
	eqSlot4.setXY(-1, -1);
	for (int i = 0;i < cntBoxes;i++)boxes[i]->setXY(-1, -1);
	if (t == 'H')Home();
	if (t == 'S')Shop();
	if (t == 'I')Inventory();
	if (t == 'P')Map();
}

void Engine::Map()
{
	EnemyBox *enemies[MAX_ENEMY_CNT];
	int cntEnemies, currEnemy = -1;
	std::pair<int, int> playerPos(1,1);
	
	loadMap(enemies, cntEnemies);
	randomizeEnemies(enemies, cntEnemies);
	Graphics::getInstance().init();
	Graphics::getInstance().drawMap(playerPos, enemies, cntEnemies);

	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point moveCD = std::chrono::steady_clock::now();
	initMouse();
	coord.X = 0;
	coord.Y = 0;
	
	while (true)
	{
		//Graphics::getInstance().gotoxy(0, 0);std::cout << coord.X << ' ' << coord.Y;
		if (playerPos.first == 0 && playerPos.second == 0) Home();
		for (int i = 0;i < cntEnemies;i++) if (enemies[i]->isWithin(playerPos.first, playerPos.second)) Play((Enemy&)enemies[i]);
		
		{//Mouse Hover
			if (currEnemy == -1)
			{
				for (int i = 0;i < cntEnemies;i++)
				{
					if (enemies[i]->isWithin(coord.X, coord.Y))
					{
						enemies[i]->toggleInfoBox();
						currEnemy = i;
					}
				}
			}
			else
			{
				if (!enemies[currEnemy]->isWithin(coord.X, coord.Y))
				{
					enemies[currEnemy]->hideInfoBox();
					for (int i = 0;i < cntEnemies;i++)enemies[i]->showBox();
					currEnemy = -1;
				}
			}
		}
		if (InputRecord.EventType == KEY_EVENT)
		{
			Graphics::getInstance().gotoxy(playerPos.first, playerPos.second);std::cout << ' ';
			switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
			{
			case KEY_ARROW_UP: if (playerPos.second > 0)playerPos.second--; break;
			case KEY_ARROW_DOWN: if (playerPos.second < DEF_CONSOLE_HEIGHT - 1)playerPos.second++; break;
			case KEY_ARROW_LEFT: 
				if (playerPos.first == 1)playerPos.first--;
				if (playerPos.first > 1)playerPos.first -= 2;
				break;
			case KEY_ARROW_RIGHT: 
				if (playerPos.first == DEF_CONSOLE_WIDTH - 2)playerPos.first++;
				if (playerPos.first < DEF_CONSOLE_WIDTH - 2)playerPos.first += 2;

				break;
			}
			Graphics::getInstance().gotoxy(playerPos.first, playerPos.second);std::cout << 'P';
			InputRecord.Event.KeyEvent.wVirtualKeyCode = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(25));

		if (moveCD > std::chrono::steady_clock::now())continue;
		moveCD = std::chrono::steady_clock::now() + std::chrono::milliseconds(1500);
		if (currEnemy >= 0)enemies[currEnemy]->hideInfoBox();
		enemyMove(enemies, cntEnemies, currEnemy);
	}
}

void Engine::Play(Enemy &enemy)
{
	Graphics::getInstance().clearscreen();
	std::cout << "BISH YOU GAY!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	exit(0);
}