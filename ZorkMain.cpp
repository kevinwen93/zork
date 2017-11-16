#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector> 
#include <map>
#include "rapidxml.hpp"

#include "Room.h"
#include "Item.h"
#include "Container.h"
#include "Creature.h"

using namespace rapidxml; 
using namespace std;

bool ExitGame;

map<string,Room> rooms;
map<string,Item> items;
map<string,Container> containers;
map<string,Creature> creatures;

vector<string> Inventory;
vector<string>Ava_Items;
vector<string>Ava_Containers;
vector<string>Ava_Creatures;
Room* cur_room;

void DomTraverse(xml_node<>*);

void SpliteBySpace(const std::string& str, std::vector<std::string>& v)
{
	std::stringstream ss(str);
  	ss >> std::noskipws;
  	std::string field;
  	char ws_delim;
  	while(1) {
    	if( ss >> field )
      		v.push_back(field);
    	else if (ss.eof())
      		break;
    	else
      		v.push_back(std::string());

    	ss.clear();
    	ss >> ws_delim;
  }
}

bool findObject(vector<string>objs, string name);

void MovementCommand(string);
bool TriggerAtrigger(Trigger&, string);
bool checkCondition(Condition&);
bool checkConditions(vector<Condition> conds);

void excuteActions(vector<string> actions);
void runAction(string);


Item* getItemByName(string name)
{
	std::map<string,Item>::iterator it;
	it = items.find(name);
	if(it==items.end())
	{
		return NULL;
	}

	return &(it->second);
}

Container* getContainerByName(string name)
{
	std::map<string,Container>::iterator it;
	it = containers.find(name);
	if(it==containers.end())
	{
		return NULL;
	}

	return &(it->second);	
}

Creature* getCreatureByName(string name)
{
	std::map<string,Creature>::iterator it;
	it = creatures.find(name);
	if(it==creatures.end())
	{
		return NULL;
	}

	return &(it->second);	
}

Room* getRoomByName(string name)
{
	std::map<string,Room>::iterator it;
	it = rooms.find(name);
	if(it==rooms.end())
	{
		return NULL;
	}

	return &(it->second);
}

bool itemAccessable(string name)
{
	for (int i = 0; i < Ava_Items.size(); ++i)
	{
		string s = Ava_Items[i];
		if (s.compare(name) == 0)
		{
			return true;
		}
	}

	return false;
}

bool containerAccessable(string name)
{
	for (int i = 0; i < Ava_Containers.size(); ++i)
	{
		string s = Ava_Containers[i];
		if (s.compare(name) == 0)
		{
			return true;
		}
	}

	return false;
}


void changeRoom(string name)
{
	Ava_Items.clear();
	Ava_Containers.clear();
	Ava_Creatures.clear();

	std::map<string,Room>::iterator it;
	it = rooms.find(name);
	cur_room = &(it->second);

	printf("%s\n", cur_room->description.c_str());

	//add items to ava_items
	Ava_Items.insert(Ava_Items.end(), Inventory.begin(), Inventory.end());
	Ava_Items.insert(Ava_Items.end(), cur_room->items.begin(), cur_room->items.end());
	//add containers to ava_containers
	Ava_Containers.insert(Ava_Containers.end(), cur_room->containers.begin(), cur_room->containers.end());
	//add creatures to ava_creatures
	Ava_Creatures.insert(Ava_Creatures.end(), cur_room->creatures.begin(), cur_room->creatures.end());

	for (int i = 0; i < Ava_Containers.size(); ++i)
	{
		string s = Ava_Containers[i];
		Container *c = getContainerByName(s);
		if (c->opened == true)
		{
			Ava_Items.insert(Ava_Items.end(), c->items.begin(), c->items.end());
		}
	}
}

void updateRoom()
{
	Ava_Items.clear();
	Ava_Containers.clear();
	Ava_Creatures.clear();

	//add items to ava_items
	Ava_Items.insert(Ava_Items.end(), Inventory.begin(), Inventory.end());
	Ava_Items.insert(Ava_Items.end(), cur_room->items.begin(), cur_room->items.end());
	//add containers to ava_containers
	Ava_Containers.insert(Ava_Containers.end(), cur_room->containers.begin(), cur_room->containers.end());
	//add creatures to ava_creatures
	Ava_Creatures.insert(Ava_Creatures.end(), cur_room->creatures.begin(), cur_room->creatures.end());

	for (int i = 0; i < Ava_Containers.size(); ++i)
	{
		string s = Ava_Containers[i];
		Container *c = getContainerByName(s);
		if (c->opened == true)
		{
			Ava_Items.insert(Ava_Items.end(), c->items.begin(), c->items.end());
		}
	}

}

bool removeFromCurrentSpace(string name)
{
	for (int i = 0; i < Ava_Containers.size(); ++i)
	{
		Container *c = getContainerByName(Ava_Containers[i]);
		if (c->remove(name))
		{
			return true;
		}
	}

	if(cur_room->remove(name))
	{
		return true;
	}

	return false;
}

void addToVector(std::vector<string> &vec, string name)
{
	vec.push_back(name);
}

bool removeFromVector(std::vector<string> &vec, string name)
{
	bool should = false;
	std::vector<string>::iterator it;
	for (it = vec.begin(); it!=vec.end();it++)
	{
		string s = *it;
		if (s.compare(name) == 0)
		{
			should = true;
			break;
		}
	}

	if (should)
	{
		vec.erase(it);
		return true;
	}

	return false;
}

bool removeFromInventory(string name)
{
	return removeFromVector(Inventory, name);
}



bool addTo(string object, string target)
{
	bool isItem = true;
	if (getItemByName(object))
	{
		isItem = true;
	}
	else if (getContainerByName(object))
	{
		isItem = false;
	}
	

	Room *r = getRoomByName(target);
	if (r)
	{
		r->add(object, isItem);
		return true;
	}

	Container *c = getContainerByName(target);
	if (c)
	{
		c->add(object);
		return true;
	}

	if (target.compare("inventory") == 0)
	{
		Inventory.push_back(object);
		return true;
	}

	return false;
}

void MovementCommand(string input)
{
	for (int i = 0; i < cur_room->borders.size(); ++i)
	{
		/* code */
		//try move to next room
		Border &border = cur_room->borders[i];
		if(input[0] == border.direction[0])
		{
			changeRoom(border.name);
			return;
		}
	}

		printf("Wrong Way!\n");
}

void TakeCommand(string name)
{
	if (findObject(Inventory, name))
	{
		printf("already have %s\n", name.c_str());
		return;
	}
	if (!itemAccessable(name))
	{
		printf("can't take that\n");
		return;
	}
	if (removeFromCurrentSpace(name))
	{
		addTo(name, "inventory");
		printf("took %s\n", name.c_str());
	}

}

void InvCommand()
{
	printf("Inventory: ");
	for (int i = 0; i < Inventory.size(); ++i)
	{
		string s = Inventory[i];
		printf("%s   ", s.c_str());
	}
	printf("\n");
}

void OpenCommand(string name)
{
	if (name.compare("exit") == 0)
	{
		if (cur_room->type.compare("exit") == 0)
		{
			printf("Game Over, you win\n");
			ExitGame = true;
			return;
		}
		else {
			printf("this is not an exit\n");
			return;
		}
	}

	Container *c = getContainerByName(name);
	if (c == NULL)
	{
		printf("no such container\n");
		return;
	}

	if (!containerAccessable(name))
	{
		printf("cant find this container\n");
		return;
	}

	if (c->opened)
	{
		printf("%s is already opened\n", name.c_str());
		return;
	}

	c->opened = true;
	Ava_Items.insert(Ava_Items.end(), c->items.begin(), c->items.end());
	printf("you opened %s\n", name.c_str());
	printf("container contains: ");
	for (int i = 0; i < c->items.size(); ++i)
	{
		printf("%s   ", c->items[i].c_str());
	}
	printf("\n");

}

void ReadCommand(string name)
{
	Item *item = getItemByName(name);
	if (item == NULL)
	{
		printf("no such item\n");
		return;
	}

	if (findObject(Inventory, name))
	{
		string writing = item->writing;
		if (!writing.empty())
		{
			printf("%s\n", writing.c_str());
			return;
		}

		printf("Nothing written\n");
	}
	else {
		printf("you dont have it\n");
	}
}

void DropCommand(string name)
{
	if (!getItemByName(name))
	{
		printf("no such item\n");
		return;
	}

	if (!findObject(Inventory, name))
	{
		printf("you dont even have %s\n", name.c_str());
		return;
	}

	removeFromInventory(name);
	addTo(name, cur_room->name);
	printf("you droped %s in %s\n", name.c_str(), cur_room->name.c_str());
}

void PutCommand(string name1, string name2)
{
	if (!getItemByName(name1))
	{
		printf("no such item\n");
		return;
	}

	if (!findObject(Inventory, name1))
	{
		printf("you dont even have %s\n", name1.c_str());
		return;
	}

	if (containerAccessable(name2))
	{
		printf("Item %s added to %s\n", name1.c_str(), name2.c_str());
		removeFromInventory(name1);
		addTo(name1, name2);
	}
}

void TurnonCommand(string name)
{
	if (!getItemByName(name))
	{
		printf("no such item\n");
		return;
	}

	if (!findObject(Inventory, name))
	{
		printf("you dont even have %s\n", name.c_str());
		return;
	}

	Item* item = getItemByName(name);
	printf("you turned on %s\n", name.c_str());
	if (!item->turnOn.print.empty())
	{
		printf("%s\n", item->turnOn.print.c_str());
	}

	for (int i = 0; i < item->turnOn.actions.size(); ++i)
	{
		string action = item->turnOn.actions[i];
		runAction(action);
	}

}

void AttackCommand(string name1, string name2)
{
	Creature *c = NULL;
	Item *item = NULL;
	c = getCreatureByName(name1);
	item = getItemByName(name2);

	if (c == NULL)
	{
		printf("no such creature\n");
		return;
	}

	if(!findObject(Ava_Creatures, name1))
	{
		printf("the creature is not here\n");
		return;
	}

	if (item == NULL)
	{
		printf("no such item\n");
		return;
	}

	if (!findObject(Inventory, name2))
	{
		printf("you dont even have %s\n", name2.c_str());
		return;
	}

	if (!findObject(c->vulnerabilities, name2))
	{
		printf("you attack is not working\n");
		return;
	}
	
	if(!checkCondition(c->attack.cond))
	{
		printf("you attack method is not right\n");
		return;
	}

	if (!c->attack.print.empty())
	{
		printf("%s\n", c->attack.print.c_str());
	}

	for (int i = 0; i < c->attack.actions.size(); ++i)
	{
		string action = c->attack.actions[i];
		runAction(action);
	}
}

void AddCommand(string object, string target)
{

	int objectType = 0;
	if (getItemByName(object))
	{
		objectType = 0;
	}
	else if (getContainerByName(object))
	{
		objectType = 1;
	}
	else if (getCreatureByName(object))
	{
		objectType = 2;
	}
	else {
		return;
	}
	
	bool isRoom = true;

	Room *r = getRoomByName(target);
	if (r)
	{
		isRoom = true;
		r->add(object, objectType);
		updateRoom();
		return;
	}

	Container *c = getContainerByName(target);
	if (c)
	{
		isRoom = false;
		c->add(object);
		updateRoom();
		return;
	}

}

void DeleteCommand(string name)
{
	removeFromCurrentSpace(name);
	removeFromInventory(name);
	removeFromVector(Ava_Items, name);
	removeFromVector(Ava_Containers, name);
	removeFromVector(Ava_Creatures, name);
}

void UpdateCommand(string object, string status)
{
	Room *r = getRoomByName(object);
	if (r)
	{
		r->status = status;
		return;
	}

	Item *item = getItemByName(object);
	if (item)
	{
		item->status = status;
		return;
	}

	Container *contain = getContainerByName(object);
	if (contain)
	{
		contain->status = status;
		return;
	}

	Creature *crea = getCreatureByName(object);
	if(crea)
	{
		crea->status = status;
		return;
	}
}

void GameOverCommand()
{
	printf("Victory!\n");
	ExitGame = true;
}

bool TriggerAtrigger(Trigger& trigger, string input)
{
	bool inputOverride = false;
	bool shouldExcute = false;
	if (checkConditions(trigger.conds))
	{
		string commd = trigger.command;
		if (!commd.empty())
		{
			if(input.compare(commd) == 0)
			{
				inputOverride = true;
				shouldExcute = true;
			}

		}else {
			shouldExcute = true;

		}
	}

	if(trigger.type.compare("single") == 0)
	{
		if (trigger.triggered == true)
		{
			inputOverride = false;
			shouldExcute = false;
		}
	}

	if (shouldExcute)
	{
		if (!trigger.print.empty())
		{
			printf("%s\n", trigger.print.c_str());
		}

		trigger.triggered = true;
		excuteActions(trigger.actions);

	}

	return inputOverride;
}

void excuteActions(vector<string> actions)
{
	for (int i = 0; i < actions.size(); ++i)
	{
		string action = actions[i];
		runAction(action);
	}
}

bool TriggerTriggers(vector<Trigger> &triggers, string input)
{
	bool inputOverride = false;
	for (int i = 0; i < triggers.size(); ++i)
	{
		Trigger& trigger = triggers[i];

		if(TriggerAtrigger(trigger, input))
		{
			inputOverride = true;
		}
	}

	return inputOverride;
}

bool findObject(vector<string>objs, string name)
{
	for (int i = 0; i < objs.size(); ++i)
	{
		/* code */
		string s = objs[i];
		if(s.compare(name) == 0)
		{
			return true;
		}
	}

	return false;
}

bool checkConditions(vector<Condition> conds)
{
	bool passed = true;

	for (int i = 0; i < conds.size() ;++i)
	{
		Condition &cond = conds[i];
		if(!checkCondition(cond))
		{
			passed = false;
		}
	}
	return passed;
}

bool checkCondition(Condition &cond)
{
	if(cond.has.empty())
	{
		bool haveObject = false;
		string object = cond.object;
		string status = cond.status;
		Item *item = getItemByName(object);
		if (item)
		{
			if (status.compare(item->status) == 0)
			{
				return true;
			}	
		}

		Container *c = getContainerByName(object);
		if (c)
		{
			if (status.compare(c->status) == 0)
			{
				return true;
			}
		}

		return false;
	}
	else {

		string owner = cond.owner;
		string object = cond.object;
		string has = cond.has;
		bool haveObject = false;
		if(owner.compare("inventory") == 0)
		{
			haveObject = findObject(Inventory, object);
		}
		else 
		{
			Room* r = getRoomByName(owner);
			if(r)
			{
				haveObject = r->have(object);
			}

			Container* c= getContainerByName(owner);
			if(c)
			{
				haveObject = c->have(object);
			}
		}

		bool shouldhave = true;
		if(has.compare("no") == 0)
		{
			shouldhave = false;
		}

		if (haveObject)
		{
			if (shouldhave)
			{
				return true;
			}

			return false;
		}
		else
		{
			if (shouldhave)
			{
				return false;
			}

			return true;
		}

		return false;
	}

	return false;
}

bool checkTriggers(string input)
{
	bool inputOverride = false;

	//check trigger for cur_room
	if(TriggerTriggers(cur_room->triggers, input))
	{
		inputOverride = true;
	}



	//check triggers for containers
	for (int i = 0; i < Ava_Containers.size(); ++i)
	{
		string c_name = Ava_Containers[i];
		Container *c = getContainerByName(c_name);
		if(TriggerTriggers(c->triggers, input))
		{
			inputOverride = true;
		}
		//printf("triger for %s\n", c_name.c_str());
	}


	//check triggers for items
	for (int i = 0; i < Ava_Items.size(); ++i)
	{
		string i_name = Ava_Items[i];
		Item *item = getItemByName(i_name);
		if(TriggerTriggers(item->triggers, input))
		{
			inputOverride = true;
		}
		//printf("triger for %s\n", i_name.c_str());
	}

	//check triggers for creatures
	for (int i = 0; i < Ava_Creatures.size(); ++i)
	{
		string c_name = Ava_Creatures[i];
		Creature *c = getCreatureByName(c_name);
		if(TriggerTriggers(c->triggers, input))
		{
			inputOverride = true;
		}
		//printf("triger for %s\n", c_name.c_str());
	}


	/*if (inputOverride)
	{
		printf("should blocked\n");
	}*/
	return inputOverride;

}

void runAction(string input)
{

	if(input.compare("n") == 0)
		{
			MovementCommand(input);
		}
		else if(input.compare("s") == 0)
		{
			MovementCommand(input);

		}
		else if(input.compare("e") == 0)
		{
			MovementCommand(input);

		}
		else if(input.compare("w") == 0)
		{
			MovementCommand(input);
		}
		else if(input.find("take")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 2)
			{
				printf("ERROR\n");
			}
			else {
				TakeCommand(elements[1]);
			}
		}
		else if(input.compare("i") == 0)
		{
			InvCommand();
		}
		else if(input.find("open")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 2)
			{
				printf("open error\n");
			}
			else {
				OpenCommand(elements[1]);
			}
		}
		else if(input.find("read")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 2)
			{
				printf("read error\n");
			}
			else {
				ReadCommand(elements[1]);
			}
		}
		else if(input.find("drop")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 2)
			{
				printf("drop error\n");
			}
			else {
				DropCommand(elements[1]);	
			}
		}
		else if(input.find("put")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 4)
			{
				printf("put error\n");
			}
			else {
				if(elements[2].compare("in") == 0)
				{
					PutCommand(elements[1], elements[3]);
				}
				else {
					printf("put error\n");
				}
			}
		}
		else if(input.find("turn on")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 3)
			{
				printf("turnon error\n");
			}
			else {
				TurnonCommand(elements[2]);
			}
		}
		else if(input.find("attack")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 4)
			{
				printf("atack error\n");
			}
			else {
				if(!elements[2].compare("with") == 0)
				{
					printf("atack error\n");
				}
				else {
					AttackCommand(elements[1], elements[3]);
				}
			}  
		}
		else if(input.find("Add")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 4)
			{
				printf("add error\n");
			}
			else {
				if(elements[2].compare("to") == 0)
				{
					AddCommand(elements[1], elements[3]);
				}
				else {
					printf("add error\n");
				}
			}
		}
		else if(input.find("Delete")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 2)
			{
				printf("delete error\n");
			}
			else {
				DeleteCommand(elements[1]);
			}
		}
		else if(input.find("Update")!= std::string::npos)
		{
			vector<string> elements;
			SpliteBySpace(input, elements);
			if(elements.size() != 4)
			{
				printf("update error\n");
			}
			else {
				if(elements[2].compare("to") == 0)
				{
					UpdateCommand(elements[1], elements[3]);
				}
				else {
					printf("update error\n");
				}
			}
		}
		else if(input.compare("Game Over")==0)
		{
			GameOverCommand();
		}
		else{
			printf("ERROR\n");
		}

}



int main()
{
	ExitGame = false;

  	xml_document<> doc;
	xml_node<> * root_node;
	// Read the xml file into a vector
	ifstream theFile("sample.xml");
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc
	doc.parse<0>(&buffer[0]); 

	xml_node<> *pRoot = doc.first_node();
	DomTraverse(pRoot);


	changeRoom("Entrance");
	

	while(!ExitGame) 
	{
		char r_input[80];
		printf(">");
		cin.getline(r_input,80);
		string input = r_input;

		if(!checkTriggers(input))
		{
			runAction(input);
			checkTriggers("null");
		}
		else {

		}

	}
}

void DomTraverse(xml_node<> *root)
{
	
	for(xml_node<> *pNode=root->first_node();pNode; pNode=pNode->next_sibling())
	{
		string n_name = pNode->name();
		if(n_name.compare("room") == 0)
		{
			//printf("find a room\n");
			Room room(pNode);
			rooms.insert(make_pair(room.name, room));
		}
		else if(n_name.compare("item") == 0)
		{
			//printf("find an item\n");
			Item item(pNode);
			items.insert(make_pair(item.name, item));
		}
		else if(n_name.compare("container") == 0)
		{
			//printf("find a container\n");
			Container container(pNode);
			containers.insert(make_pair(container.name, container));
		}
		else if(n_name.compare("creature") == 0)
		{
			//printf("find a creature\n");
			Creature creature(pNode);
			creatures.insert(make_pair(creature.name, creature));
		}

	}
}