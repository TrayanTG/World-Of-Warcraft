#include "HeadersAndConsts.h"
#include "Data.h"
using namespace std;


int main()
{
	srand(time(0));
	
	Graphics::getInstance().drawFrame("Ivan", 5, 50, 9, 50, 100, LightBlue, 10, 10);

	/*
	Data::getIstance().loadItems();
	Data::getIstance().items[3]->setXY(10,10);
	Data::getIstance().items[3]->showBox();
	Data::getIstance().items[3]->toggleInfoBox();
	*/	
	
	/*Ability a(1, 6, 9, 10, 1, 20, "Fireball", "Deals nice amount of damage.", "Ability", 4, false, 10, 10);
	a.showBox({ 100,100 });
	a.toggleInfoBox();
	*/

	//Engine::getIstance().updateCursor();
	/*Weapon a({ 10,0 }, Axe, "Axe1", "Melee weapon, which deals 10 physical damage.", "WPN", 3, false, 100, 30);
	Armor b({ 1,0 }, Head, "Head1", "Common helmet, which reduced physical damage by 1.", "HEAD", 3, false, 50, 30);

	a.showBox();
	a.toggleInfoBox();
	a.hideInfoBox();
	b.showBox();
	b.toggleInfoBox();
	b.hideInfoBox();
	*/
	while (true);
	return 0;
}
