#include "Box.h"
#include "Graphics.h"

Box::Box(const string &title, const string &description, const string &name, int size, const Coord &topLeft, bool marked):
	InfoBox(title, description), name(name), topLeft(topLeft)
{
	this->size = size;
	this->marked = marked;
}

int Box::getQuadrant()const
{
	if (topLeft.X < 0 || topLeft.Y < 0)return 0;
	int x = topLeft.X + size;
	int y = topLeft.Y + size / 2;
	if (x >= DEF_CONSOLE_WIDTH / 2 && y <= DEF_CONSOLE_HEIGHT / 2)return 1;
	if (x <= DEF_CONSOLE_WIDTH / 2 && y <= DEF_CONSOLE_HEIGHT / 2)return 2;
	if (x <= DEF_CONSOLE_WIDTH / 2) return 3;
	return 4;
}

const Coord &Box::getTopLeft()const
{
	return topLeft;
}

Coord Box::getInfoBoxTopLeft()const
{
	int quadrant = getQuadrant();
	switch (quadrant)
	{
	case 1:
		return { topLeft.X - (DEF_INFOBOX_WIDTH - 2 * size),topLeft.Y + size + 1 };
		break;
	case 2:
		return { topLeft.X,topLeft.Y + size + 1 };
		break;
	case 3:
		return { topLeft.X,topLeft.Y - getRows(DEF_INFOBOX_WIDTH) - 3 };
		break;
	case 4:
		return { topLeft.X - (DEF_INFOBOX_WIDTH - 2 * size), topLeft.Y - getRows(DEF_INFOBOX_WIDTH) - 3 };
		break;
	}
	return { -1,-1 };
}

Coord Box::getInfoBoxBotRight()const
{
	int quadrant = getQuadrant();
	switch (quadrant)
	{
	case 1:
		return { topLeft.X + 2 * size,topLeft.Y + size + 3 + getRows(DEF_INFOBOX_WIDTH) };
		break;
	case 2:
		return { topLeft.X + DEF_INFOBOX_WIDTH,topLeft.Y + size + getRows(DEF_INFOBOX_WIDTH) + 3 };
		break;
	case 3:
		return { topLeft.X + DEF_INFOBOX_WIDTH, topLeft.Y - 1 };
		break;
	case 4:
		return { topLeft.X + 2 * size, topLeft.Y - 1 };
		break;
	}
	return { -1,-1 };
}

bool Box::isWithin(const Coord &pos, int range)const //not tested
{
	if (pos.X < 0 || pos.Y < 0 || pos.X >= DEF_CONSOLE_WIDTH || pos.Y >= DEF_CONSOLE_HEIGHT)return false;
	return pos.X >= topLeft.X - 2 * range && pos.X <= topLeft.X + 2 * size + 2 * range && 
		   pos.Y >= topLeft.Y - range && pos.Y <= topLeft.Y + size + range;
}

bool Box::isMarked()const
{
	return marked;
}

bool Box::setMarked(bool marked)
{
	if (this->marked == marked) return false;
	this->marked = marked;
	return true;
}

bool Box::setXY(const Coord &topLeft)
{
	if (topLeft.X < -1 || topLeft.Y < -1 || topLeft.X >= DEF_CONSOLE_WIDTH - 2 * size  || topLeft.Y >= DEF_CONSOLE_HEIGHT - size)return false;
	this->topLeft = topLeft;
	return true;
}

int Box::getID()const
{
	return -1;
}

void Box::showBox(const Damage &damage)const
{
	Graphics::clearBoarder(topLeft, { topLeft.X + 2 * size, topLeft.Y + size });
	if (marked)Graphics::setcolor(LightYellow);
	Graphics::drawBoarder(topLeft, { topLeft.X + 2 * size, topLeft.Y + size });
	Graphics::setcolor(White);
	Graphics::gotoxy(topLeft.X + 1 + (2 * size - name.size() - 1) / 2, topLeft.Y + 1);
	std::cout << name;
	Graphics::gotoxy(topLeft.X + 1, topLeft.Y + 2);
}

void Box::hideBox()const
{
	Graphics::clearBoarder(topLeft, { topLeft.X + 2 * size, topLeft.Y + size });
}

void Box::toggleInfoBox()const
{
	showInfoBox(getInfoBoxTopLeft(), getInfoBoxBotRight());
}

void Box::hideInfoBox()const
{
	Graphics::clearBoarder(getInfoBoxTopLeft(), getInfoBoxBotRight());
}