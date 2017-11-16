#include "Item.h"

Item::Item(xml_node<> *i_node)
{
	for(xml_node<> *pNode=i_node->first_node();pNode; pNode=pNode->next_sibling())
	{
		//printf("%s\n", pNode->name());
		string n_name = pNode->name();
		if(n_name.compare("name") == 0)
		{
			name = pNode->value();
		}
		else if (n_name.compare("status") == 0)
		{
			status = pNode->value();
		}
		else if (n_name.compare("description") == 0 )
		{
			description = pNode->value();;
		}
		else if (n_name.compare("writing") == 0)
		{
			writing = pNode->value();
		}
		else if (n_name.compare("turnon") == 0)
		{
			turnOn = SetTurnOn(pNode);
		}
		else if (n_name.compare("trigger") == 0 )
		{
			AddTrigger(pNode);
		}
	}
}


TurnOn Item::SetTurnOn(xml_node<> *t_node)
{
	TurnOn to;

	for(xml_node<> *pNode=t_node->first_node();pNode; pNode=pNode->next_sibling())
	{
		string n_name = pNode->name();
		string v = pNode->value();
		if(n_name.compare("print") == 0)
		{
			to.print = v;
		}
		else if(n_name.compare("action") == 0)
		{
			to.actions.push_back(v);
		}
	}

	return to;
}

void Item::AddTrigger(xml_node<> *t_node)
{
	Trigger trigger(t_node);
	triggers.push_back(t_node);
}

Item::~Item()
{

}