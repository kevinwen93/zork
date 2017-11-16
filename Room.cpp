#include "Room.h"



Room::Room(xml_node<> *r_node)
{
	for(xml_node<> *pNode=r_node->first_node();pNode; pNode=pNode->next_sibling())
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
		else if (n_name.compare("type") == 0)
		{
			type = pNode->value();
		}
		else if (n_name.compare("description") == 0 )
		{
			description = pNode->value();;
		}
		else if (n_name.compare("border") == 0 )
		{
			AddBorder(pNode);
		}
		else if (n_name.compare("container") == 0 )
		{
			string v = pNode->value();
			containers.push_back(v);
		}
		else if (n_name.compare("item") == 0 )
		{
			string v = pNode->value();
			items.push_back(v);
		}
		else if (n_name.compare("creature") == 0 )
		{
			string v = pNode->value();
			creatures.push_back(v);
		}
		else if (n_name.compare("trigger") == 0 )
		{
			AddTrigger(pNode);
		}
	}
}

bool Room::have(string name)
{
	for (int i = 0; i < items.size(); ++i)
	{
		string s = items[i];
		if (s.compare(name) == 0)
		{
			return true;
		}
	}

	for (int i = 0; i < containers.size(); ++i)
	{
		string s = containers[i];
		if (s.compare(name) == 0)
		{
			return true;
		}
	}

	return false;
}

bool Room::remove(string name)
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


	for (it = containers.begin(); it!=containers.end();it++)
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
		containers.erase(it);
		return true;
	}

	for (it = creatures.begin(); it!=creatures.end();it++)
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
		creatures.erase(it);
		return true;
	}

	return false;
}

void Room::add(string name, bool isItem)
{
	if (isItem)
	{
		items.push_back(name);
	}
	else {
		containers.push_back(name);
	}
}

void Room::add(string name, int type)
{

	if (type == 0)
	{
		items.push_back(name);
	}
	else if (type == 1){
		containers.push_back(name);
	}
	else if(type == 2)
	{
		creatures.push_back(name);
	}
}


void Room::AddBorder(xml_node<> *b_node)
{
	Border border(b_node);
	borders.push_back(border);
}

void Room::AddTrigger(xml_node<> *t_node)
{
	Trigger trigger(t_node);
	triggers.push_back(t_node);
}

Room::~Room()
{

}