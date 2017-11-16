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

struct TurnOn
{
	/* data */
	string print;
	vector<string> actions;
};

class Item
{
public:
	Item(xml_node<> *i_node);
	~Item();

	/* data */
	string name;
	string status;
	string description;
	string writing;
	TurnOn turnOn;
	vector<Trigger> triggers;

private:
	TurnOn SetTurnOn(xml_node<> *t_node);
	void AddTrigger(xml_node<> *t_node);
};