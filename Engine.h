#pragma once

#include "HeadersAndConsts.h"

class Engine
{
	static Engine s;
	
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	COORD coord = { 0, 0 };
	CONSOLE_CURSOR_INFO cci;

	Engine();

public:
	void updateCursor();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	~Engine();

	static Engine& getInstance();

	void init();

};