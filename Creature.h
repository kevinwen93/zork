#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector> 
#include "rapidxml.hpp"
#include "Trigger.h"

using namespace rapidxml; 
using namespace std;

struct Attack
{
	/* data */
	Condition cond;
	string print;
	vector<string> actions;
};

class Creature
{
public:
	Creature(xml_node<> *c_node);
	~Creature();

	/* data */
	string name;
	string status;
	string description;
	vector<string> vulnerabilities;
	Attack attack;
	vector<Trigger> triggers;

private:
	Attack SetAttack(xml_node<> *a_node);
	Condition CreateCondition(xml_node<> *c_node);

	void AddTrigger(xml_node<> *t_node);

};