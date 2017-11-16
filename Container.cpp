#include "Container.h"

Container::Container(xml_node<> *i_node)
{
	opened = false;

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
		else if (n_name.compare("accept") == 0)
		{
			accepts.push_back(pNode->value());
		}
		else if (n_name.compare("item") == 0)
		{
			items.push_back(pNode->value());
		} 
		else if (n_name.compare("trigger") == 0 )
		{
			AddTrigger(pNode);
		}
	}
}

bool Container::have(string name)
{
	for (int i = 0; i < items.size(); ++i)
	{
		string s = items[i];
		if (s.compare(name) == 0)
		{
			return true;
		}
	}

	return false;
}

bool Container::remove(string name)
{
	bool should = false;
	std::vector<string>::iterator it;
	for (it = items.begin(); it!=items.end();it++)
	{
		string s = *it;
		if (s.compare(name) == 0)
		{
			should = true;
			break;
		}
	}

	if (should)
	{
		items.erase(it);
		return true;
	}

	return false;
}

void Container::add(string name)
{
	items.push_back(name);
}

void Container::AddTrigger(xml_node<> *t_node)
{
	Trigger trigger(t_node);
	triggers.push_back(t_node);
}

Container::~Container()
{

}