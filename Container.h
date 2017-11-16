#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector> 
#include "rapidxml.hpp"
#include "Trigger.h"

using namespace rapidxml; 
using namespace std;

class Container
{
public:
	Container(xml_node<> *c_node);
	~Container();

	/* data */
	bool opened;
	string name;
	string status;
	string description;
	vector<string> accepts;
	vector<string> items;
	vector<Trigger> triggers;

	bool have(string name);
	bool remove(string name);
	void add(string name);

private:
	void AddTrigger(xml_node<> *t_node);
};