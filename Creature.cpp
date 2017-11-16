#include "Creature.h"

Creature::Creature(xml_node<> *i_node)
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
			description = pNode->value();
		}
		else if (n_name.compare("vulnerability") == 0)
		{
			vulnerabilities.push_back(pNode->value());
		}
		else if (n_name.compare("attack") == 0)
		{
			attack = SetAttack(pNode);
		} 
		else if (n_name.compare("trigger") == 0 )
		{
			AddTrigger(pNode);
		}
	}
}




void Creature::AddTrigger(xml_node<> *t_node)
{
	Trigger trigger(t_node);
	triggers.push_back(t_node);
}

Attack Creature::SetAttack(xml_node<> *a_node)
{
	Attack att;
	for(xml_node<> *pNode=a_node->first_node();pNode; pNode=pNode->next_sibling())
	{
		string n_name = pNode->name();
		string v = pNode->value();
		if(n_name.compare("condition") == 0)
		{
			att.cond = CreateCondition(pNode);
		}
		else if(n_name.compare("print") == 0)
		{
			att.print = v;
		}
		else if(n_name.compare("action") == 0)
		{
			att.actions.push_back(v);
		}
		
	}

	return att;
}

Condition Creature::CreateCondition(xml_node<> *c_node)
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

Creature::~Creature()
{

}