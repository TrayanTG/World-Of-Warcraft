#include "HeadersAndConsts.h"
#include "Data.h"
#include "Player.h"
#include "Graphics.h"
#include "Engine.h"
using namespace std;


int main()
{
	srand(time(0));

	Data::getIstance().loadItems();
	Data::getIstance().loadAbilities();

	Player myPlayer;
	myPlayer.loadPlayer("Data/Logs/Petkan/");
	Graphics::getInstance().drawPlayerUI(myPlayer, 5, 5);
	//myPlayer.savePlayer("Data/Logs/Petkan/");

	//myPlayer.helmet = (Armor *)Data::getIstance().getItemByID(2);
	//myPlayer.helmet->setXY(70, 0);
	//myPlayer.helmet->showBox();
	

	/*
	Data::getIstance().loadItems();
	Data::getIstance().loadAbilities();
	
	Item *temp = Data::getIstance().getItemByID(3);
	temp->setXY(10, 10);
	temp->showBox();
	temp->toggleInfoBox();
	
	Player myPlayer;
	//myPlayer.setCharacter({ 10,0 }, { 1,1,0 }, "Ivan", { 100, 100 }, { 100, 100 }, 1, 0);
	cout<<myPlayer.loadPlayer("Data/Logs/Ivan/");
	cout << myPlayer.getName();
	myPlayer.eqAbilities[1]->setXY(10, 10);
	myPlayer.eqAbilities[1]->showBox({ 10,10 });
	myPlayer.eqAbilities[1]->toggleInfoBox();*/


	/*cout<<myPlayer.buyItem(Data::getIstance().items[0]);
	cout << myPlayer.equipItem(Data::getIstance().items[0]);
	cout << myPlayer.buyItem(Data::getIstance().items[1]);
	cout << myPlayer.buyItem(Data::getIstance().items[3]);
	cout << myPlayer.equipItem(Data::getIstance().items[2]);
	cout << myPlayer.addAbility(Data::getIstance().abilities[0]);*/
	//cout << myPlayer.equipAbility(Data::getIstance().abilities[0], 1);
	//cout << myPlayer.equipAbility(Data::getIstance().abilities[0], 0);

	//cout << myPlayer.savePlayer("Data/Logs/Ivan/");

	
	//Graphics::getInstance().drawFrame("Ivan", 5, 50, 9, 50, 100, LightBlue, 10, 10);

	
	//Data::getIstance().loadItems();
	//Data::getIstance().items[3]->setXY(10,10);
	//Data::getIstance().items[3]->showBox();
	//Data::getIstance().items[3]->toggleInfoBox();
	
	
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