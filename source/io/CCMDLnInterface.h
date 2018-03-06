#pragma once

#include <string>
#include <vector>


#define NOT_TERMINATOR(x) x != '\r' && x != '\n' && x != '\0'
#define NOT_WHITESPACE(x) x != ' ' && x != '\n' && x != '\t'

#define CMD_ARGLIST std::vector<std::string>

#define CmdFunc(nme) bool nme(CMD_ARGLIST args, void* object)

class CCMDLnInterfaceBase
{
private:
	struct SCmdFunc
	{
		SCmdFunc(std::string cmd = "", bool(*func)(std::vector<std::string>, void*) = nullptr, void* object = nullptr, std::string menu = "root") :
			command(cmd),
			func(func),
			pObject(object)
		{
			valid = (func != nullptr);
		}

		//What string the command is identified by
		std::string command;
		std::string menu;
		//If you want to pass an object or data to the function set this variable to that data.
		void* pObject;
		//bool CommandFunction(arguments, object_passed)
		bool (*func)(std::vector<std::string>, void*);
		bool valid = false;

		bool doFunc(std::vector<std::string> args)
		{
			if (valid)
				return func(args, pObject);
			else
			{
				printf("Invalid Command Function\n");
				return false;
			}
		}
	};

	struct SCmdMenu
	{
		std::string				menu_name;
		SCmdMenu*				parent;
		std::vector<SCmdMenu*>	child;
		std::vector<SCmdFunc>	m_cmdList;

		SCmdFunc get_cmdFunc(std::string cmd);
	};
	class SCmdMenuTree
	{
	public:
		SCmdMenuTree(bool& exit_flag);

		void NavUp();
		bool NavDown(std::string child_name);
		SCmdMenu* GetMenu(std::string name);

		SCmdMenu* active_menu;
		void AddMenu(std::string name, std::string parent = "root");
	private:
		SCmdMenu root;
		std::vector<SCmdMenu*> m_menuIndex;

		bool& __exit_flag;

	};

public:
	CCMDLnInterfaceBase()
	:
		m_menuTree(__exit_flag)
	{}
	~CCMDLnInterfaceBase(){}

	//Overide this function to add new Commands to the interface
	virtual void initialise() = 0;

	void do_uInput();

	void set_exitflag();
	bool exitflag();
protected:
	bool add_commandFunc(std::string command, bool(*func)(std::vector<std::string>, void*), void* obj = nullptr, std::string menu = "root");
	bool add_commandMenu(std::string menu_name, std::string parent_menu = "root");

	std::string get_input();
	std::vector<std::string> get_args(std::string cmdln);

	SCmdMenuTree m_menuTree;

	bool __exit_flag;
};