#include "Border.h"

Border::Border(xml_node<> *b_node)
{

	for(xml_node<> *pNode=b_node->first_node();pNode; pNode=pNode->next_sibling())
	{
		//printf("%s\n", pNode->name());
		string n_name = pNode->name();
		if(n_name.compare("direction") == 0)
		{
			direction = pNode->value();
		}
		else if (n_name.compare("name") == 0)
		{
			name = pNode->value();
		}
	}
}

Border::~Border()
{
	
}