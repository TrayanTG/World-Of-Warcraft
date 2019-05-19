#include "Box.h"
#include "Graphics.h"

Box::Box(const char *title, const char *description, const char *name, int size, bool marked, int tlx, int tly): InfoBox(title, description)
{
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->size = size;
	this->marked = marked;
	this->tlx = tlx;
	this->tly = tly;
}


Box& Box::operator= (const Box &other)
{
	if (this == &other)return *this;
	InfoBox::operator=(other);
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	size = other.size;
	marked = other.marked;
	tlx = other.tlx;
	tly = other.tly;
	return *this;
}

Box::~Box()
{
	delete[] name;
}

int Box::getQuadrant()const
{
	if (tlx < 0 || tly < 0)return 0;
	int x = tlx + size;
	int y = tly + size / 2;
	if (x >= DEF_CONSOLE_WIDTH / 2 && y <= DEF_CONSOLE_HEIGHT / 2)return 1;
	if (x <= DEF_CONSOLE_WIDTH / 2 && y <= DEF_CONSOLE_HEIGHT / 2)return 2;
	if (x <= DEF_CONSOLE_WIDTH / 2) return 3;
	return 4;
}

int Box::getTLX()const
{
	return tlx;
}

int Box::getTLY()const
{
	return tly;
}

int Box::ibTLX()const
{
	int ans;
	int quadrant = getQuadrant();
	switch (quadrant)
	{
		case 1:
			ans = tlx - (DEF_INFOBOX_WIDTH - 2 * size);
			break;
		case 2:
			ans = tlx;
			break;
		case 3:
			ans = tlx;
			break;
		case 4:
			ans = tlx - (DEF_INFOBOX_WIDTH - 2 * size);
			break;
	}
	return ans;
}

int Box::ibTLY()const
{
	int ans;
	int quadrant = getQuadrant();
	switch (quadrant)
	{
		case 1:
			ans = tly + size + 1;
			break;
		case 2:
			ans = tly + size + 1;
			break;
		case 3:
			ans = tly - getRows(DEF_INFOBOX_WIDTH) - 3;
			break;
		case 4:
			ans = tly - getRows(DEF_INFOBOX_WIDTH) - 3;
			break;
	}
	return ans;
}

int Box::ibBRX()const
{
	int ans;
	int quadrant = getQuadrant();
	switch (quadrant)
	{
		case 1:
			ans = tlx + 2 * size;
			break;
		case 2:
			ans = tlx + DEF_INFOBOX_WIDTH;
			break;
		case 3:
			ans = tlx + DEF_INFOBOX_WIDTH;
			break;
		case 4:
			ans = tlx + 2 * size;
			break;
	}
	return ans;
}

int Box::ibBRY()const
{
	int ans;
	int quadrant = getQuadrant();
	switch (quadrant)
	{
		case 1:
			ans = tly + size + 3 + getRows(DEF_INFOBOX_WIDTH);
			break;
		case 2:
			ans = tly + size + getRows(DEF_INFOBOX_WIDTH) + 3;
			break;
		case 3:
			ans = tly - 1;
			break;
		case 4:
			ans = tly - 1;
			break;
	}
	return ans;
}

bool Box::isWithin(int x, int y, int range)const //not tested
{
	if (x < 0 || y < 0 || x >= DEF_CONSOLE_WIDTH || y >= DEF_CONSOLE_HEIGHT)return false;
	return x >= tlx - 2 * range && x <= tlx + 2 * size + 2 * range && y >= tly - range && y <= tly + size + range;
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

bool Box::setXY(int x, int y)
{
	if (x < -1 || y < -1 || x >= DEF_CONSOLE_WIDTH - 2 * size  || y >= DEF_CONSOLE_HEIGHT - size)return false;
	tlx = x;
	tly = y;
	return true;
}

int Box::getID()const
{
	return -1;
}

bool Box::showBox(const Damage &damage)const
{
	if (tlx < 0 || tly < 0 || tlx + 2 * size >= DEF_CONSOLE_WIDTH || tly + size >= DEF_CONSOLE_HEIGHT)return false;
	Graphics::getInstance().clearBoarder(tlx, tly, tlx + 2 * size, tly + size);
	if (marked)Graphics::getInstance().setcolor(LightYellow);
	Graphics::getInstance().drawBoarder(tlx, tly, tlx + 2 * size, tly + size);
	Graphics::getInstance().setcolor(White);
	Graphics::getInstance().gotoxy(tlx + 1 + (size * 2 - strlen(name) - 1) / 2, tly + 1);
	std::cout << name;
	Graphics::getInstance().gotoxy(tlx + 1, tly + 2);
	return true;
}

bool Box::hideBox()const
{
	if (tlx < 0 || tly < 0 || tlx + 2 * size >= DEF_CONSOLE_WIDTH || tly + size >= DEF_CONSOLE_HEIGHT)return false;
	Graphics::getInstance().clearBoarder(tlx, tly, tlx + 2 * size, tly + size);
	return true;
}

bool Box::toggleInfoBox()const
{
	/*int quadrant = getQuadrant();
	switch (quadrant)
	{
	case 1:
		showInfoBox(tlx - (DEF_INFOBOX_WIDTH - 2 * size), tly + size + 1, tlx + 2 * size, tly + size + 3 + getRows(DEF_INFOBOX_WIDTH));
		break;
	case 2:
		showInfoBox(tlx, tly + size + 1, tlx + DEF_INFOBOX_WIDTH, tly + size + getRows(DEF_INFOBOX_WIDTH) + 3);
		break;
	case 3:
		showInfoBox(tlx, tly - getRows(DEF_INFOBOX_WIDTH) - 3, tlx + DEF_INFOBOX_WIDTH, tly - 1);
		break;
	case 4:
		showInfoBox(tlx - (DEF_INFOBOX_WIDTH - 2 * size), tly - getRows(DEF_INFOBOX_WIDTH) - 3, tlx + 2 * size, tly - 1);
		break;
	default:
		return false;
	}*/
	showInfoBox(ibTLX(), ibTLY(), ibBRX(), ibBRY());
	return true;
}

bool Box::hideInfoBox()const
{
	int quadrant = getQuadrant();
	switch (quadrant)
	{
	case 1:
		Graphics::getInstance().clearBoarder(tlx - (DEF_INFOBOX_WIDTH - 2 * size), tly + size + 1, tlx + 2 * size, tly + size + 3 + getRows(DEF_INFOBOX_WIDTH));
		break;
	case 2:
		Graphics::getInstance().clearBoarder(tlx, tly + size + 1, tlx + DEF_INFOBOX_WIDTH, tly + size + getRows(DEF_INFOBOX_WIDTH) + 3);
		break;
	case 3:
		Graphics::getInstance().clearBoarder(tlx, tly - getRows(DEF_INFOBOX_WIDTH) - 3, tlx + DEF_INFOBOX_WIDTH, tly - 1);
		break;
	case 4:
		Graphics::getInstance().clearBoarder(tlx - (DEF_INFOBOX_WIDTH - 2 * size), tly - getRows(DEF_INFOBOX_WIDTH) - 3, tlx + 2 * size, tly - 1);
		break;
	default:
		return false;
	}
	return true;
}