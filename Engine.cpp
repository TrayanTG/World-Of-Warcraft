#include "Engine.h"
#include "Graphics.h"
#include "Data.h"
#include "HeadersAndConsts.h"
#include "PlayerClasses.h"

Engine Engine::s;

Engine::Engine()
{
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

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
	delete[] myPlayer;
}

Engine& Engine::getInstance()
{
	return s;
}

const COORD &Engine::getCoord()
{
	return { InputRecord[currEvent].Event.MouseEvent.dwMousePosition.X, InputRecord[currEvent].Event.MouseEvent.dwMousePosition.Y };
}

const WORD &Engine::getKey()
{
	return InputRecord[currEvent].Event.KeyEvent.wVirtualKeyCode;
}

void Engine::resetEvents()
{
	currEvent = Events;
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

void Engine::enemyMove(EnemyBox **enemies, int &enemyCnt, int &currEnemy)
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
//std::cout << enemies[i]->getHP().Curr;
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
//std::cout << enemies[i]->getHP().Curr;
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
//std::cout << enemies[i]->getHP().Curr;
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
//std::cout << enemies[i]->getHP().Curr;
				}
				break;
			}
		}
		
		for (DWORD i=0;i<Events;i++)
		{
			if (currEnemy >= 0 && enemies[currEnemy]->isWithin(InputRecord[i].Event.MouseEvent.dwMousePosition.X, InputRecord[i].Event.MouseEvent.dwMousePosition.Y))enemies[currEnemy]->toggleInfoBox(); // Not sure for the getCoord(???)
		}
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
				if (typeid(*boxes[k]) != typeid(Ability) && boxes[k]->getID() == myPlayer->items[i]->getID()) 
				{
					f = false;
					break; 
				}
			}
			if (f) boxes[cntBoxes++] = myPlayer->items[i];
		}
	}
	
	equipItem.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1 + TEMP_FIX);
	sellItem.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) + totalLenX) / 2 - 12, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1 + TEMP_FIX);

	for (int i = invBoxes;i < cntBoxes;i++)
	{
		boxes[i]->setXY(DEF_FREE_BEG + 19 + 2 * ((i - invBoxes) % 10) * (DEF_ITEM_SIZE + 1), 10 + ((i - invBoxes) / 10) * (DEF_ITEM_SIZE + 1) + TEMP_FIX);
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
				if (typeid(*boxes[k]) != typeid(Ability) && Data::getIstance().items[i]->getID() == boxes[k]->getID()) f = false;
			}
			if (f)
			{
				int id = Data::getIstance().items[i]->getID();
				if (id < 15) //[0;14] - Weapons
				{
					if (typeid(*myPlayer) == typeid(Mage) && id % 3 != 0)continue;
					if (typeid(*myPlayer) == typeid(Warrior) && id % 3 != 1)continue;
					if (typeid(*myPlayer) == typeid(Paladin) && id % 3 != 2)continue;
				}
				if (Data::getIstance().items[i]->getMinLevel() > myPlayer->getLevel()) continue;
				boxes[cntBoxes++] = Data::getIstance().items[i];
			}
		}
	}

	for (int i = invBoxes;i < cntBoxes;i++)
	{
		boxes[i]->setXY(DEF_FREE_BEG + 19 + 2 * ((i - invBoxes) % 10) * (DEF_ITEM_SIZE + 1), 10 + ((i - invBoxes) / 10) * (DEF_ITEM_SIZE + 1) + TEMP_FIX);
	}

	buyItem.setXY((DEF_CONSOLE_WIDTH + DEF_FREE_BEG - buyItem.getLen() - 2) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 4);
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

	eqSlot1.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 - 4 + 2 * TEMP_FIX + 1);
	eqSlot2.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2 + eqSlot2.getLen() + 5, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 - 4 + 2 * TEMP_FIX + 1);
	eqSlot3.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1 + 2 * TEMP_FIX + 1);
	eqSlot4.setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2 + eqSlot2.getLen() + 5, (DEF_CONSOLE_HEIGHT + totalLenY) / 2 + 1 + 2 * TEMP_FIX + 1);

	for (int i = 0;i < cntBoxes - invBoxes;i++)
	{
		boxes[i+invBoxes]->setXY(((DEF_CONSOLE_WIDTH + DEF_FREE_BEG) - totalLenX) / 2 + 1 + 2 * (i % 3) * (DEF_ABILITY_SIZE + 1), (DEF_CONSOLE_HEIGHT - totalLenY) / 2 - 5 + (i / 3)*(DEF_ABILITY_SIZE + 1) + 2 * TEMP_FIX + 1);
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
		if (Events && (currEvent < Events))continue;
		ReadConsoleInput(hin, InputRecord, 128, &Events);
		currEvent = 0;
	}
}

void Engine::logIn()
{
	
	char chooseClass, chooseNewExist, name[MAX_NAME_LENGHT];
	bool cheatTemp = false;
// ---------------------------------------------
choosePlayer:
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
			std::cout << name << '(';
			if (chooseClass == 'W')std::cout << "Warrior)";
			else if (chooseClass == 'M')std::cout << "Mage)";
			else std::cout << "Paladin)";
			std::cout << " does not exist!";
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			goto choosePlayer;
		}
		myPlayer->loadPlayer(strcat(directory, "/"));
		iFile.close();
	}
	Graphics::getInstance().init();
}

void Engine::loadMap(EnemyBox **enemies, int &enemyCnt)
{
	resetEvents();
	char chooseMap, path[MAX_PATH_LENGHT], temp[4];
	int enemyIDs[MAX_ENEMY_CNT], index = 0;
	Graphics::getInstance().drawMapChooseUI();
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (currEvent < Events)
		{
			chooseMap = (char)InputRecord[currEvent++].Event.KeyEvent.wVirtualKeyCode;
			if (chooseMap >= 'a')chooseMap -= ('a' - 'A');
		}
	} while (chooseMap != 'E' && chooseMap != 'D' && chooseMap != 'I');
	// ---------------------------------------------
	std::ifstream iFile;
	strcpy(path, "Data/Maps/");
	if (chooseMap == 'E')strcat(path, "ElwynnForest");
	else if (chooseMap == 'D')strcat(path, "Durotar");
	else strcat(path, "IsleOfGiants");
	iFile.open(path);
	if (!iFile) exit(-1);
	iFile >> enemyCnt;
	if (enemyCnt > MAX_ENEMY_CNT) exit(-1);
	while (iFile) iFile >> enemyIDs[index++];index--;
	iFile.close();
	
	for (int i = index;i < enemyCnt;i++) enemyIDs[i] = enemyIDs[rand() % index];
	//for (int i = 0;i < enemyCnt;i++)std::cout << enemyIDs[i] << ' ';system("pause");
	for (int i = 0;i < enemyCnt;i++)
	{
		enemies[i] = new EnemyBox(temp, *Data::getIstance().getEnemyByID(enemyIDs[i]), myPlayer->getLevel() + rand() % MAX_LEVEL_DIF);
		//std::cout << enemies[i]->getHP().Max << ' ';system("pause");
	}
}

void Engine::Home()
{
	resetEvents();
	char t = 0;
	Box *boxes[16];
	int cntBoxes = 0, currBox = -1, oldBox = -1;
	initHome(boxes, cntBoxes, currBox);
	
	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes);
	Graphics::getInstance().drawLogo();
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now() + DEF_RENDER_TIME;
	while (true)
	{
		myPlayer->savePlayer(directory);
		for (; currEvent < Events; currEvent++)
		{
			if (InputRecord[currEvent].EventType == KEY_EVENT)
			{
				switch (InputRecord[currEvent].Event.KeyEvent.wVirtualKeyCode)
				{
				case 'S': t = 'S'; break;
				case 'I': t = 'I'; break;
				case 'P': t = 'P'; break;
				case 'A': t = 'A'; break;
				case 'E': exit(0);
				}
				if (t)goto ExitHome;
			}
			if (InputRecord[currEvent].EventType == MOUSE_EVENT)
			{
				bool f = false;
				for (int i = 0;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(getCoord().X, getCoord().Y))
					{
						currBox = i;
						f = true;
						break;
					}
				}
				if(!f) currBox = -1;
			}
		}
		if (std::chrono::steady_clock::now() >= animationCD)
		{
			animationCD += DEF_RENDER_TIME;
			if (oldBox >= 0 && oldBox != currBox)
			{
				boxes[oldBox]->hideInfoBox();
				Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes, boxes[oldBox]->ibTLX(), boxes[oldBox]->ibTLY(), boxes[oldBox]->ibBRX(), boxes[oldBox]->ibBRY());
			}
			if (currBox >= 0 && currBox != oldBox)
			{
				boxes[currBox]->toggleInfoBox();
			}
			oldBox = currBox;
		}
	}
ExitHome:
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
	int cntBoxes = 0, currBox = -1, oldBox = -1, invBoxes = -1, markedBox = -1;

	initInventory(boxes, equipItem, sellItem, cntBoxes, currBox, invBoxes, markedBox);
	
	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
	Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes-invBoxes, equipItem, sellItem);
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	while (true)
	{
		myPlayer->savePlayer(directory);
		for (; currEvent < Events; currEvent++)
		{
			if (InputRecord[currEvent].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				if (markedBox == -1)
				{
					if (currBox >= invBoxes)
					{
						boxes[currBox]->setMarked(true);
						boxes[currBox]->showBox();
					}
					markedBox = currBox;
				}
				else
				{
					boxes[markedBox]->setMarked(false);
					boxes[markedBox]->showBox();
					if (equipItem.isWithin(getCoord().X, getCoord().Y))
					{
						myPlayer->equipItem((Item*)boxes[markedBox]);
						initInventory(boxes, equipItem, sellItem, cntBoxes, currBox, invBoxes, markedBox);
						Graphics::getInstance().clearscreen();
						Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
						Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, equipItem, sellItem);
						markedBox = -1;
					}
					else if (sellItem.isWithin(getCoord().X, getCoord().Y))
					{
						myPlayer->sellItem((Item*)boxes[markedBox]);
						for (int i = markedBox + 1;i < cntBoxes;i++)
						{
							boxes[i - 1] = boxes[i];
						}
						cntBoxes--;
						Graphics::getInstance().clearscreen();
						Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
						Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, equipItem, sellItem);
						markedBox = -1;
					}
					else
					{
						if (markedBox != currBox)
						{
							if (currBox >= invBoxes)
							{
								boxes[currBox]->setMarked(true);
								boxes[currBox]->showBox();
								boxes[currBox]->toggleInfoBox();
							}
							markedBox = currBox;
						}
						else markedBox = -1;
					}
				}
			}
			if (InputRecord[currEvent].EventType == KEY_EVENT)
			{
				switch (InputRecord[currEvent].Event.KeyEvent.wVirtualKeyCode)
				{
				case 'S': t = 'S'; break;
				case 'H': t = 'H'; break;
				case 'P': t = 'P'; break;
				case 'A': t = 'A'; break;
				case 'E': exit(0);
				}
				if (t)goto exitInventory;
			}
			if (InputRecord[currEvent].EventType == MOUSE_EVENT)
			{
				bool f = false;
				for (int i = 0;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(getCoord().X, getCoord().Y))
					{
						currBox = i;
						f = true;
						break;
					}
				}
				if (!f) currBox = -1;
			}
		}
		if (std::chrono::steady_clock::now() >= animationCD)
		{
			animationCD += DEF_RENDER_TIME;
			if (oldBox >= 0 && oldBox != currBox)
			{
				boxes[oldBox]->hideInfoBox();
				if (oldBox < invBoxes) Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes, boxes[oldBox]->ibTLX(), boxes[oldBox]->ibTLY(), boxes[oldBox]->ibBRX(), boxes[oldBox]->ibBRY());
				else Graphics::getInstance().drawInventoryUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, equipItem, sellItem, boxes[oldBox]->ibTLX(), boxes[oldBox]->ibTLY(), boxes[oldBox]->ibBRX(), boxes[oldBox]->ibBRY());
			}
			if (currBox >= 0 && currBox != oldBox)
			{
				boxes[currBox]->toggleInfoBox();
			}
			oldBox = currBox;
		}
	}
exitInventory:
	equipItem.setXY(-1, -1);
	sellItem.setXY(-1, -1);
	for (int i = 0;i < cntBoxes;i++)
	{
		boxes[i]->setMarked(false);
		boxes[i]->setXY(-1, -1);
	}
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
	int cntBoxes = 0, currBox = -1, oldBox = -1, invBoxes = -1, markedBox = -1;
	initShop(boxes, buyItem, cntBoxes, currBox, invBoxes, markedBox);

	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
	Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem);
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now() + DEF_RENDER_TIME;

	while (true)
	{
		myPlayer->savePlayer(directory);
		for (; currEvent < Events;currEvent++)
		{
			if (InputRecord[currEvent].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				if (markedBox == -1)
				{
					if (currBox >= invBoxes)
					{
						boxes[currBox]->setMarked(true);
						boxes[currBox]->showBox();
					}
					markedBox = currBox;
				}
				else
				{
					boxes[markedBox]->setMarked(false);
					boxes[markedBox]->showBox();
					if (buyItem.isWithin(getCoord().X, getCoord().Y))
					{
						myPlayer->buyItem((Item*)boxes[markedBox]);
						initShop(boxes, buyItem, cntBoxes, currBox, invBoxes, markedBox);
						Graphics::getInstance().clearscreen();
						Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
						Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem);
						markedBox = -1;
					}
					else
					{
						if(markedBox != currBox)
						{
							if (currBox >= invBoxes)
							{
								boxes[currBox]->setMarked(true);
								boxes[currBox]->showBox();
								boxes[currBox]->toggleInfoBox();
							}
							markedBox = currBox;
						}
						else markedBox = -1;
					}
				}
			}
			if (InputRecord[currEvent].EventType == KEY_EVENT)
			{
				switch (InputRecord[currEvent].Event.KeyEvent.wVirtualKeyCode)
				{
				case 'L': if (cheatOn) myPlayer->gainXP(10); break;
				case 'M': if (cheatOn) myPlayer->gainGold(1000); break;
				case 'H': t = 'H'; break;
				case 'I': t = 'I'; break;
				case 'P': t = 'P'; break;
				case 'A': t = 'A'; break;
				case 'E': exit(0);
				}
				if (t)goto exitShop;
			}
			if (InputRecord[currEvent].EventType == MOUSE_EVENT)
			{
				bool f = false;
				for (int i = 0;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(getCoord().X, getCoord().Y))
					{
						currBox = i;
						f = true;
						break;
					}
				}
				if (!f) currBox = -1;
			}
		}
		if (std::chrono::steady_clock::now() >= animationCD)
		{
			animationCD += DEF_RENDER_TIME;
			if (oldBox >= 0 && oldBox != currBox)
			{
				boxes[oldBox]->hideInfoBox();
				if (oldBox < invBoxes) Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes, boxes[oldBox]->ibTLX(), boxes[oldBox]->ibTLY(), boxes[oldBox]->ibBRX(), boxes[oldBox]->ibBRY());
				else Graphics::getInstance().drawShopUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, buyItem, boxes[oldBox]->ibTLX(), boxes[oldBox]->ibTLY(), boxes[oldBox]->ibBRX(), boxes[oldBox]->ibBRY());
			}
			if (currBox >= 0 && currBox != oldBox)
			{
				boxes[currBox]->toggleInfoBox();
			}
			oldBox = currBox;
		}
	}
exitShop:
	buyItem.setXY(-1, -1);
	for (int i = 0;i < cntBoxes;i++)
	{
		boxes[i]->setMarked(false);
		boxes[i]->setXY(-1, -1);
	}
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
	int cntBoxes = 0, currBox = -1, oldBox = -1, invBoxes = -1, markedBox = -1;
	initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);

	Graphics::getInstance().clearscreen();
	Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
	Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now() + DEF_RENDER_TIME;
	while (true)
	{
		myPlayer->savePlayer(directory);
		for (; currEvent < Events;currEvent++)
		{
			if (InputRecord[currEvent].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				if (markedBox == -1)
				{
					if (currBox >= invBoxes)
					{
						boxes[currBox]->setMarked(true);
						boxes[currBox]->showBox(myPlayer->getTotalDamageStats());
					}
					markedBox = currBox;
				}
				else
				{
					boxes[markedBox]->setMarked(false);
					boxes[markedBox]->showBox(myPlayer->getTotalDamageStats());
					if (eqSlot1.isWithin(getCoord().X, getCoord().Y))
					{
						myPlayer->equipAbility((Ability*)boxes[markedBox], 0);
						initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
						Graphics::getInstance().clearscreen();
						Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
						Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
						markedBox = -1;
					}
					else if (eqSlot2.isWithin(getCoord().X, getCoord().Y))
					{
						myPlayer->equipAbility((Ability*)boxes[markedBox], 1);
						initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
						Graphics::getInstance().clearscreen();
						Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
						Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
						markedBox = -1;
					}
					else if (eqSlot3.isWithin(getCoord().X, getCoord().Y))
					{
						myPlayer->equipAbility((Ability*)boxes[markedBox], 2);
						initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
						Graphics::getInstance().clearscreen();
						Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
						Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
						markedBox = -1;
					}
					else if (eqSlot4.isWithin(getCoord().X, getCoord().Y))
					{
						myPlayer->equipAbility((Ability*)boxes[markedBox], 3);
						initAilityBook(boxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, cntBoxes, currBox, invBoxes, markedBox);
						Graphics::getInstance().clearscreen();
						Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes);
						Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4);
						markedBox = -1;
					}
					else
					{
						if (markedBox != currBox)
						{
							if (currBox >= invBoxes)
							{
								boxes[currBox]->setMarked(true);
								boxes[currBox]->showBox(myPlayer->getTotalDamageStats());
								boxes[currBox]->toggleInfoBox();
							}
							markedBox = currBox;
						}
						else markedBox = -1;
					}
				}
			}
			if (InputRecord[currEvent].EventType == KEY_EVENT)
			{
				switch (InputRecord[currEvent].Event.KeyEvent.wVirtualKeyCode)
				{
				case 'S': t = 'S'; break;
				case 'H': t = 'H'; break;
				case 'P': t = 'P'; break;
				case 'I': t = 'I'; break;
				case 'E': exit(0);
				}
				if (t)goto exitAbilityBook;
			}
			if (InputRecord[currEvent].EventType == MOUSE_EVENT)
			{
				bool f = false;
				for (int i = 0;i < cntBoxes;i++)
				{
					if (boxes[i]->isWithin(getCoord().X, getCoord().Y))
					{
						currBox = i;
						f = true;
						break;
					}
				}
				if (!f) currBox = -1;
			}
		}
		if (std::chrono::steady_clock::now() >= animationCD)
		{
			animationCD += DEF_RENDER_TIME;
			if (oldBox >= 0 && oldBox != currBox)
			{
				boxes[oldBox]->hideInfoBox();
				if (oldBox < invBoxes) Graphics::getInstance().drawHomeUI(*myPlayer, boxes, cntBoxes - invBoxes, boxes[oldBox]->ibTLX(), boxes[oldBox]->ibTLY(), boxes[oldBox]->ibBRX(), boxes[oldBox]->ibBRY());
				else Graphics::getInstance().drawAbilityBookUI(*myPlayer, &boxes[invBoxes], cntBoxes - invBoxes, eqSlot1, eqSlot2, eqSlot3, eqSlot4, boxes[oldBox]->ibTLX(), boxes[oldBox]->ibTLY(), boxes[oldBox]->ibBRX(), boxes[oldBox]->ibBRY());
			}
			if (currBox >= 0 && currBox != oldBox)
			{
				boxes[currBox]->toggleInfoBox();
			}
			oldBox = currBox;
		}
	}
exitAbilityBook:
	eqSlot1.setXY(-1, -1);
	eqSlot2.setXY(-1, -1);
	eqSlot3.setXY(-1, -1);
	eqSlot4.setXY(-1, -1);
	for (int i = 0;i < cntBoxes;i++)
	{
		boxes[i]->setMarked(false);
		boxes[i]->setXY(-1, -1);
	}
	if (t == 'H')Home();
	if (t == 'S')Shop();
	if (t == 'I')Inventory();
	if (t == 'P')Map();
}

void Engine::Map()
{
	EnemyBox *enemies[MAX_ENEMY_CNT];
	int cntEnemies, currEnemy = -1, oldEnemy = -1;
	std::pair<int, int> playerPos(1,1);
	
	loadMap(enemies, cntEnemies);
	randomizeEnemies(enemies, cntEnemies);
	Graphics::getInstance().init();
	Graphics::getInstance().drawMap(playerPos, enemies, cntEnemies);
	initMouse();
	std::chrono::steady_clock::time_point moveCD = std::chrono::steady_clock::now() + DEF_ENEMY_MOVE_TIME;
	while (true)
	{
		if (playerPos.first == 0 && playerPos.second == 0) Home();
		for (int i = 0;i < cntEnemies;i++)//Check for battle
		{
			if (enemies[i]->isWithin(playerPos.first, playerPos.second))
			{
				Enemy attackedEnemy = (Enemy)(*enemies[i]);
				for (int j = 0;j < cntEnemies;j++)
				{
					delete[] enemies[j];
				}
				Play(attackedEnemy);
			}
		}
		for (; currEvent < Events;currEvent++)
		{
			if (InputRecord[currEvent].EventType == KEY_EVENT)
			{
				Graphics::getInstance().gotoxy(playerPos.first, playerPos.second);std::cout << ' ';
				switch (InputRecord[currEvent].Event.KeyEvent.wVirtualKeyCode)
				{
				case KEY_ARROW_UP:
					if (playerPos.second > 0)playerPos.second--; break;
				case KEY_ARROW_DOWN:
					if (playerPos.second < DEF_CONSOLE_HEIGHT - 1)playerPos.second++; break;
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
			}
			if (InputRecord[currEvent].EventType == MOUSE_EVENT)
			{
				bool f = false;
				for (int i = 0;i < cntEnemies;i++)
				{
					if (enemies[i]->isWithin(getCoord().X, getCoord().Y))
					{
						currEnemy = i;
						f = true;
						break;
					}
				}
				if (!f) currEnemy = -1;
			}
		}

		{//Nameplate render
			if (oldEnemy >= 0 && oldEnemy != currEnemy)
			{
				enemies[oldEnemy]->hideInfoBox();
			}
			if (currEnemy >= 0 && currEnemy != oldEnemy)
			{
				enemies[currEnemy]->toggleInfoBox();
			}
			oldEnemy = currEnemy;
		}

		if (std::chrono::steady_clock::now() >= moveCD)
		{
			moveCD = std::chrono::steady_clock::now() + DEF_ENEMY_MOVE_TIME;
			if (currEnemy >= 0)enemies[currEnemy]->hideInfoBox();
			enemyMove(enemies, cntEnemies, currEnemy);
		}
	}
}

void Engine::Play(Enemy enemy)
{
	char *text = new char[8];
	char *temp = new char[7];
	FloatingTexts playerFloatingDamageText(7, 6, 12, 10);
	FloatingTexts enemyFloatingDamageText(7, 6, DEF_CONSOLE_WIDTH - 15, 10);
	Damage playerDamage;
	Damage enemyDamage;
	myPlayer->setMaxHP();
	myPlayer->resetRes();
	enemy.setMaxHP();
	Graphics::getInstance().drawPlay(*myPlayer, enemy);
	for (int i = 0;i < 4;i++)
	{
		myPlayer->eqAbilities[i]->reduceCD(myPlayer->eqAbilities[i]->getCD());
		myPlayer->eqAbilities[i]->setXY(DEF_CONSOLE_WIDTH - 2 * DEF_FREE_BEG + (i - 1) * (2 * DEF_ABILITY_SIZE + 2), DEF_CONSOLE_HEIGHT - DEF_ABILITY_SIZE - 1);
		myPlayer->eqAbilities[i]->showBox(myPlayer->getTotalDamageStats());
	}
	std::chrono::steady_clock::time_point animationCD = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point enemyMove = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point frameUpdate = std::chrono::steady_clock::now();


	resetEvents();
	// Battle
	while (myPlayer->isAlive() && enemy.isAlive())
	{
		for (; currEvent < Events; currEvent++)
		{
			if (InputRecord[currEvent].Event.KeyEvent.bKeyDown)
			{
				switch (InputRecord[currEvent].Event.KeyEvent.wVirtualKeyCode)
				{
				case '1':
				{
					if (myPlayer->eqAbilities[0] == &Data::getIstance().emptyAbility)break;
					playerDamage = myPlayer->dealDamage(0);
					break;
				}
				case '2':
				{
					if (myPlayer->eqAbilities[1] == &Data::getIstance().emptyAbility)break;
					playerDamage = myPlayer->dealDamage(1);
					break;
				}
				case '3':
				{
					if (myPlayer->eqAbilities[2] == &Data::getIstance().emptyAbility)break;
					playerDamage = myPlayer->dealDamage(2);
					break;
				}
				case '4':
				{
					if (myPlayer->eqAbilities[3] == &Data::getIstance().emptyAbility)break;
					playerDamage = myPlayer->dealDamage(3);
					break;
				}
				default: break;
				}
				if (playerDamage.Physical + playerDamage.Magical <= 0)break;
				enemy.gainDamage(playerDamage);
				text[0] = '-';text[1] = 0;temp[0] = 0;
				enemyFloatingDamageText.addText(strcat(text, _itoa(enemy.calcDamage(playerDamage), temp, 10)));
			}
		}
		if (std::chrono::steady_clock::now() >= frameUpdate)
		{
			frameUpdate += std::chrono::milliseconds(100);
			for (int i = 0;i < 4;i++)
			{
				if (myPlayer->eqAbilities[i]->getCDRem() > 0)
				{
					myPlayer->eqAbilities[i]->reduceCD(100);
					myPlayer->eqAbilities[i]->showCD();
				}
			}
			myPlayer->regenRes();
			Graphics::getInstance().drawFrame(*myPlayer, 3, 3);
			resetEvents();
		}

		if (std::chrono::steady_clock::now() >= animationCD)
		{
			animationCD += std::chrono::milliseconds(1000);
			playerFloatingDamageText.update();
			playerFloatingDamageText.show(LightRed);
			enemyFloatingDamageText.update();
			enemyFloatingDamageText.show(LightRed);
			Graphics::getInstance().drawFrame(*myPlayer, 3, 3);
			Graphics::getInstance().drawFrame(enemy, DEF_CONSOLE_WIDTH - 25, 3);
		}

		if (std::chrono::steady_clock::now() >= enemyMove)
		{
			enemyMove += std::chrono::milliseconds(3000);
			enemyDamage = enemy.dealDamage();
			myPlayer->gainDamage(enemyDamage);
			text[0] = '-';text[1] = 0;temp[0] = 0;
			playerFloatingDamageText.addText(strcat(text, _itoa(myPlayer->calcDamage(enemyDamage), temp, 10)));
		}
	}
	
	// Results
	Graphics::getInstance().setFontSize(DEF_CONSOLE_SIZE * 2);
	Graphics::getInstance().SetConsoleWindowSize(DEF_CONSOLE_WIDTH / 2, DEF_CONSOLE_HEIGHT / 2);
	Graphics::getInstance().clearscreen();
	if (!myPlayer->isAlive())
	{
		std::cout << "You lost!\n";
	}
	else
	{
		std::cout << "You won!\n";
		std::cout << "You received " << enemy.getGold() << " gold\n";
		myPlayer->gainGold(enemy.getGold());
		std::cout << "You received " << enemy.getXP() << " XP\n";
		int currGold = myPlayer->getGold();
		if (myPlayer->gainXP(enemy.getXP()))
		{
			std::cout << "You reaced " << myPlayer->getLevel() << " level!\n";
			std::cout << "You received bonus " << myPlayer->getGold() - currGold << " gold!\n";
		}
	}
	std::cout << "Press <H> to return to the home page!";
	std::chrono::steady_clock::time_point skipTime = std::chrono::steady_clock::now() + std::chrono::seconds(30);
	resetEvents();
	
	while (true)
	{
		for (; currEvent < Events;currEvent++)
		{
			if (getKey() == 'h' || getKey() == 'H')goto exitResultScreen;
			if (std::chrono::steady_clock::now() >= skipTime)goto exitResultScreen;
		}
	}
exitResultScreen:
	delete[] temp;
	delete[] text;
	for (int i = 0;i < 4;i++)
	{
		myPlayer->eqAbilities[i]->reduceCD(myPlayer->eqAbilities[i]->getCD());
		myPlayer->eqAbilities[i]->hideBox();
		myPlayer->eqAbilities[i]->setXY(-1, -1);
	}
	Graphics::getInstance().setFontSize(DEF_CONSOLE_SIZE);
	Graphics::getInstance().SetConsoleWindowSize(DEF_CONSOLE_WIDTH, DEF_CONSOLE_HEIGHT);
	Home();
}
