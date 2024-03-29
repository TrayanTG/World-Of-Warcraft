#include "InfoBox.h"
#include "Graphics.h"

InfoBox::InfoBox(const string &title, const string &description)
{
	if (title.size() + 2 >= MAX_INFOBOX_WIDTH)throw std::runtime_error("Invalid InfoBox title lenght!\n");
	this->title = title;
	this->description = description;
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

bool InfoBox::showInfoBox(const Coord &topLeft, const Coord &botRight)const
{
	int width = botRight.X - topLeft.X;
	int rows = botRight.Y - topLeft.Y - 2;
	//std::cout << 123;
	if (width < MIN_INFOBOX_WIDTH || width>MAX_INFOBOX_WIDTH)return false;
	//std::cout << 456;system("pause");
	Graphics::clearBoarder(topLeft, botRight);
	Graphics::drawBoarder(topLeft, botRight);
	Graphics::gotoxy(topLeft.X + 1, topLeft.Y + 1);std::cout << title;
	int index = 0;
	for (int i = 0;i < rows;i++)
	{
		Graphics::gotoxy(topLeft.X + 1, topLeft.Y + 2 + i);
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
	return true;
}

bool InfoBox::clearInfoBox(const Coord &topLeft, const Coord &botRight)const
{
	int width = botRight.X - topLeft.X;
	if (width < MIN_INFOBOX_WIDTH || width>=MAX_INFOBOX_WIDTH)return false;
	Graphics::clearBoarder(topLeft, botRight);
	return true;
}
