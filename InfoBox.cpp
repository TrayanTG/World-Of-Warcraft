#include "InfoBox.h"
#include "Graphics.h"

InfoBox::InfoBox(const char *title, const char *description)
{
	if (strlen(title) + 2 >= MAX_INFOBOX_WIDTH)throw std::runtime_error("Invalid InfoBox title lenght!\n");
	this->title = new char[strlen(title) + 1];
	strcpy(this->title, title);
	this->description = new char[strlen(description) + 1];
	strcpy(this->description, description);
	if (this->description[strlen(this->description) - 1] == '\n') this->description[strlen(this->description) - 1] = 0;
}

InfoBox& InfoBox::operator= (const InfoBox &other)
{
	if (this == &other) return *this;
	delete[] title;
	delete[] description;
	title = new char[strlen(other.title) + 1];
	strcpy(title, other.title);
	description = new char[strlen(other.description) + 1];
	strcpy(description, other.description);
	return *this;
}

InfoBox::~InfoBox()
{
	delete[] title;
	delete[] description;
}

int InfoBox::getRows(int width)const
{
	int t = 0, rows = 1, i;
	for (i = 0;description[i];i++)
	{
		if (i - t >= width - 2 || description[i]=='\n')
		{
			rows++;
			t = i + 1;
		}
	}
	if (i == t)rows--;
	return rows;
}

bool InfoBox::showInfoBox(int tlx, int tly, int brx, int bry)const
{
	int width = brx - tlx;
	int rows = bry - tly - 2;
	if (width < MIN_INFOBOX_WIDTH || width>MAX_INFOBOX_WIDTH)return false;
	//int rows = getRows(width);
	Graphics::getInstance().clearBoarder(tlx, tly, brx, bry);//tlx + width - 1, tly + rows + 2);
	Graphics::getInstance().drawBoarder(tlx, tly, brx, bry);//tlx + width - 1, tly + rows + 2);
	Graphics::getInstance().gotoxy(tlx + 1, tly + 1);std::cout << title;
	int index = 0;
	for (int i = 0;i < rows;i++)
	{
		Graphics::getInstance().gotoxy(tlx + 1, tly + 2 + i);
		for (int j = 0;j < width - 1;j++)
		{
			if (!description[index] || description[index] == '\n') 
			{
				index++;
				break;
			}
			std::cout << description[index++];
		}
	}
	//Graphics::getInstance().gotoxy(20, 20);
	return true;
}

bool InfoBox::clearInfoBox(int tlx, int tly, int brx, int bry)const
{
	int width = brx - tlx;
	if (width < MIN_INFOBOX_WIDTH || width>MAX_INFOBOX_WIDTH)return false;
	Graphics::getInstance().clearBoarder(tlx, tly, brx, bry);
	return true;
}