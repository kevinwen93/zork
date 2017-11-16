#ifndef BORDER_H_
#define BORDER_H_

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector> 
#include "rapidxml.hpp"

using namespace rapidxml; 
using namespace std;

class Border
{
public:

	Border(xml_node<> *b_node);
	~Border();

	/* data */
	string direction;
	string name;
};

#endif /*BORDER_H_*/