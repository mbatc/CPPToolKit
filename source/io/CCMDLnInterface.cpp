#include "CCMDLnInterface.h"
#include <stdio.h>

char get_special_char(char c) {
	switch (c)
	{
	case 'n':
		return '\n';
	case '0':
		return '\0';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	default:
		return c;
	}
}

void CCMDLnInterfaceBase::do_uInput()
{
	printf("%s |> ", m_menuTree.active_menu->menu_name.c_str());
	std::string input = get_input();
	auto args = get_args(input);

	SCmdMenu* curMenu = m_menuTree.active_menu;
	if (args.size() <= 1)
		curMenu = nullptr;
	bool res = true;
	
	if (args[0] != "exit")
	{
		while (res)
		{
			if (args.size() == 0)
				break;

			res = m_menuTree.NavDown(args[0]);
			if (res)
				args.erase(args.begin());
		}

		if (args.size() > 0)
		{
			SCmdFunc cmdFunc = m_menuTree.active_menu->get_cmdFunc(args[0]);
			//Remove command name from argument list
			args.erase(args.begin());
			cmdFunc.doFunc(args);

			if (curMenu)
				m_menuTree.active_menu = curMenu;
		}
	}
	else
	{
		m_menuTree.NavUp();
	}
}

bool CCMDLnInterfaceBase::exitflag()
{
	return __exit_flag;
}

bool CCMDLnInterfaceBase::add_commandFunc(std::string command, bool(*func)(std::vector<std::string>, void *), void * obj, std::string menu)
{
	SCmdMenu* pMenu = m_menuTree.GetMenu(menu);
	if (pMenu == nullptr)
	{
		pMenu = m_menuTree.GetMenu("root");
	}
	
	pMenu->m_cmdList.push_back(SCmdFunc(command, func, obj));
	return pMenu->m_cmdList[pMenu->m_cmdList.size() - 1].valid;
}

bool CCMDLnInterfaceBase::add_commandMenu(std::string menu_name, std::string parent_menu)
{
	m_menuTree.AddMenu(menu_name, parent_menu);
	return true;
}

std::string CCMDLnInterfaceBase::get_input()
{
	char c = '\0';
	std::string cmdln = "";
	do
	{
		c = getchar();
		if(c)
		cmdln += c;
	} while (NOT_TERMINATOR(c));
	cmdln.pop_back();

	return cmdln;
}

std::vector<std::string> CCMDLnInterfaceBase::get_args(std::string cmdln)
{
	std::vector<std::string> args;
	std::string curArg = "";
	bool is_quote = false;
	char quote_type ='\0';

	for (int i = 0; i < cmdln.size(); i++)
	{
		char c = cmdln[i];

		if (NOT_WHITESPACE(c) || is_quote == true)
		{
			if (c == '\'' || c == '"')
			{
				if (!is_quote)
				{
					quote_type = c;
					is_quote = true;
				}
				else if (quote_type == c)
				{
					is_quote = false;
				}
				else
				{
					curArg += c;
				}
			}
			else
			{
				if (is_quote)
				{
					if (c == '\\')
					{
						c = get_special_char(cmdln[i + 1]);
						i++;
					}
				}
				curArg += c;
			}
		}
		else
		{
			if (curArg.size() > 0)
			{
				args.push_back(curArg);
				curArg = "";
			}
		}
	}

	if(curArg.size() > 0)
		args.push_back(curArg);

	return args;
}

//Default interface initialisation()

CmdFunc(exit_func)
{
	*(bool*)object = true;
	printf("Exiting command line interface!\n");

	return true;
}

CCMDLnInterfaceBase::SCmdMenuTree::SCmdMenuTree(bool& exit_flag)
	:
	__exit_flag(exit_flag)
{
	m_menuIndex.push_back(&root);
	root.menu_name = "root";
	root.parent = nullptr;

	active_menu = &root;
}

void CCMDLnInterfaceBase::SCmdMenuTree::NavUp()
{
	if (active_menu->parent)
	{
		active_menu = active_menu->parent;
	}
	else
	{
		printf("At root menu: Setting exit flag\n");
		__exit_flag = true;
	}
}

bool CCMDLnInterfaceBase::SCmdMenuTree::NavDown(std::string child_name)
{
	for (int i = 0; i < active_menu->child.size(); i++)
	{
		if (active_menu->child[i]->menu_name == child_name)
		{
			active_menu = active_menu->child[i];
			return true;
		}
	}

	return false;
}

CCMDLnInterfaceBase::SCmdMenu * CCMDLnInterfaceBase::SCmdMenuTree::GetMenu(std::string name)
{
	for (int i = 0; i < m_menuIndex.size(); i++)
	{
		if (name == m_menuIndex[i]->menu_name)
		{
			return m_menuIndex[i];
		}
	}
}

void CCMDLnInterfaceBase::SCmdMenuTree::AddMenu(std::string name, std::string parent)
{
	for (int i = 0; i < m_menuIndex.size(); i++)
	{
		if (name == m_menuIndex[i]->menu_name)
		{
			return;
		}
	}
	SCmdMenu* newMenu = new SCmdMenu();
	newMenu->parent = GetMenu(parent);
	if (newMenu->parent == nullptr)
	{
		newMenu->parent = &root;
		printf("Unable to set %s parent to %s: does not exist - using root instead", name.c_str(), parent.c_str());
	}
	newMenu->menu_name = name;
	newMenu->parent->child.push_back(newMenu);
	m_menuIndex.push_back(newMenu);
}

CCMDLnInterfaceBase::SCmdFunc CCMDLnInterfaceBase::SCmdMenu::get_cmdFunc(std::string cmd)
{
	for (int i = 0; i < m_cmdList.size(); i++)
	{
		if (cmd.compare(m_cmdList[i].command) == 0)
		{
			return m_cmdList[i];
		}
	}

	return SCmdFunc();
}
