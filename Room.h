#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector> 
#include "rapidxml.hpp"
#include "Trigger.h"
#include "Border.h"

using namespace rapidxml; 
using namespace std;

class Room
{
public:
	Room(xml_node<> *r_node);
	//Room* CreateRoom(xml_node<> *r_node);
	//void InitRoom(xml_node<> *r_node);
	~Room();
	/* data */

	string name;
	string status;
	string type;
	string description;
	vector<Border> borders; 
	vector<string> containers;
	vector<string> items;
	vector<string> creatures;
	vector<Trigger> triggers;

	bool have(string name);
	bool remove(string name);
	void add(string name, bool isItem);
	void add(string name, int type);

private:
	void AddBorder(xml_node<> *b_node);
	void AddTrigger(xml_node<> *t_node);
};