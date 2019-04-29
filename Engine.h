#pragma once

#include "HeadersAndConsts.h"
#include "Graphics.h"


class Engine
{public:
	static Engine s;
	
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	COORD coord = { 0, 0 };
	CONSOLE_CURSOR_INFO cci;

	Engine();

	Player *myPlayer;
	char directory[MAX_PATH_LENGHT];

public:
	void updateCursor();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	~Engine();
	
	static Engine& getInstance();

	void initMouse();
	void setCursorVisible(bool isVisible);

	void logIn();


	void Home();
	void Inventory();
};