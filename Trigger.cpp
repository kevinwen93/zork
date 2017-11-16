#include "Trigger.h"

Trigger::Trigger(xml_node<> *t_node)
{
	triggered = false;

	for(xml_node<> *pNode=t_node->first_node();pNode; pNode=pNode->next_sibling())
	{
		//printf("%s\n", pNode->name());
		string n_name = pNode->name();
		if(n_name.compare("type") == 0)
		{
			type = pNode->value();
		}
		else if (n_name.compare("command") == 0)
		{
			command = pNode->value();
		}
		else if (n_name.compare("condition") == 0 )
		{
			Condition cond = CreateCondition(pNode);
			conds.push_back(cond);
		}
		else if (n_name.compare("print") == 0 )
		{
			print = pNode->value();
		}
		else if(n_name.compare("action")==0)
		{
			actions.push_back(pNode->value());
		}
	}
}


Condition Trigger::CreateCondition(xml_node<> *c_node)
{
	Condition cond;

	for(xml_node<> *pNode=c_node->first_node();pNode; pNode=pNode->next_sibling())
	{
		string n_name = pNode->name();
		string v = pNode->value();
		if(n_name.compare("has") == 0)
		{
			cond.has = v;
		}
		else if(n_name.compare("object") == 0)
		{
			cond.object = v;
		}
		else if(n_name.compare("owner") == 0)
		{
			cond.owner = v;
		}
		else if(n_name.compare("status")==0)
		{
			cond.status = v;
		}
	}

	return cond;
}

Trigger::~Trigger()
{

}