
#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector> 
#include "rapidxml.hpp"


using namespace rapidxml; 
using namespace std;

struct Condition
{
	/* data */
	string has;
	string object;
	string owner;
	string status;
};

class Trigger {
public:
	Trigger(xml_node<> *t_node);
	~Trigger();

	bool triggered;

	string type;
	string command;
	string print;
	vector<Condition> conds;
	vector<string> actions;

private:

	Condition CreateCondition(xml_node<> *c_node);

};

#endif /*TRIGGER_H_*/