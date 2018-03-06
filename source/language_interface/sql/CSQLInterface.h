#pragma once

#include <string>
#include <windows.h>
#include "sqlite3.h"

#include "../../tools/CLog.h"

#include <vector>

#define VALID_INDEX(i, arr) (i >= 0 && i < arr.size())

struct SQLFILE_DATA
{
	std::vector<string> keywords;
	std::vector<string>	replace;
};

class CSQLInterface
{
private:
	CSQLInterface() : m_curConn(-1), m_retCode(0) {}
	static CSQLInterface* _instance;
public:
	static CSQLInterface* get_instance();
	static void destroy_instance() {
		if (_instance)
			delete _instance;
		_instance = nullptr;
	}

	struct DBTABLE
	{
		int nRows;
		int nCol;

		std::vector<std::string> results;
		std::vector<std::string> headers;

		std::vector<std::string> get_row(int row) {
			std::vector<std::string> ret;
			if (row >= 0 && row < nRows)
				for (int i = 0; i < nCol; i++)
					ret.push_back(results[(nCol * row) + i]);
			return ret;
		}
		std::vector<std::string> get_col(int col) {
			std::vector<std::string> ret;
			if (col >= 0 && col < nCol)
				for (int i = 0; i < nRows; i++)
					ret.push_back(results[(i * nCol) + col]);
				return ret;
		}
		std::string get_cell(int row, int col) { if (row < 0 || row >= nRows || col < 0 || col >= nCol) return "";
			return results[(nCol* row) + col];
		}
		int get_headerCol(std::string col) {
			for (int i = 0; i < headers.size(); i++)
				if (headers[i] == col)
					return i;
			return -1;
		}
	
		//The query which returned these results
		std::string query;
	};

	struct DBCONNECTION
	{
		std::string		filename;
		sqlite3*		_handle_;

		std::vector<DBTABLE> results_table;
	};

public:
	// Open a connection to an sqlite3 database.
	//   
	//   Must be called before executing a query
	//   on a database.
	bool	connectDB(std::string db);

	// Closes an open connection with an sqlite3 database.
	bool	closeDB(std::string db);

	// Sets which database to execute queries on.
	void	set_activedb(std::string db);

	// Execute a single sql query.
	//	
	//   If clear_results == true then all previous results tables
	//   will be deleted.
	bool	execute_query(std::string query, SQLFILE_DATA data = SQLFILE_DATA(), bool clear_results = true);

	// Execute multiple sql queries separated by a semi-colon.
	//	
	//  All previous results tables are cleared.
	int		execute_script(std::string text, SQLFILE_DATA data = SQLFILE_DATA());

	// Execute an sql script stored in a file on disk.
	//
	//	***************************************************
	//  All previous results tables are cleared.
	//  ***************************************************
	//
	//  The SQLFILE_DATA structure allows you to
	//  replace user defined keywords (in data.keywords)  
	//  within the script with the data at the 
	//  corresponding index in data.replace
	//
	//  ---------------------------------------------------
	//   e.g. 
	//	
	//	 SQLFILE_DATA data.keywords = ["_NAME_1", "_NAME_2"]
	//	 SQLFILE_DATA data.replace  = ["'John'", "'Andrew'"]
	//	
	//	 The script - 
	//  
	//		SELECT * FROM people WHERE name = _NAME_1;
	//		SELECT * FROM people WHERE name = _NAME_2;
	//		SELECT * FROM pets WHERE name = _NAME_1;
	//
	//   Will be executed as -
	//  
	//		SELECT * FROM people WHERE name = 'John';
	//		SELECT * FROM people WHERE name = 'Andrew';
	//		SELECT * FROM pets WHERE name = 'John';
	//
	int		execute_fscript(std::string file, SQLFILE_DATA data = SQLFILE_DATA());

	// Get the results from the most resent query or script 
	//
	//  Returns a vector containing all results tables from 
	//  the queries run
	//  
	//  Results from the active db set using set_activedb()
	//  are returned
	std::vector<DBTABLE> get_results();

	// Get the results from the most resent query or script 
	//
	//  Returns a vector containing all results tables from 
	//  the queries run
	//  
	//  The database to use is specified using the 'db'
	//  argument
	std::vector<DBTABLE> get_results(std::string db);

	// Get the results from the most resent query or script 
	//
	//  Returns a DBTABLE struct containing all results 
	//  table at the specified index
	//  
	//  Results from the active db set using set_activedb()
	//  are returned
	DBTABLE get_results(int index);

	// Get the results from the most resent query or script 
	//
	//  Returns a vector containing all results tables from 
	//  the queries run
	//  
	//  The database to use is specified using the 'db'
	//  argument
	DBTABLE get_results(std::string db, int index);

	// Returns the last error message from the specified
	// database
	const char* get_errmsg(std::string db);

	// Gets the last return code from sqlite3 functions
	int  get_last_result() { return m_retCode; }

	static CLog& get_logging_object() { return _LOG; }
private:
	static void _Log(const void * src_obj, unsigned int level, const char * format, ...);

	bool execute_select(std::string query);
	void clear_results(int dbIndex);

	std::string strip_comments(std::string s);
	std::string strip_leading_whitespace(std::string s);

	int m_curConn;

	std::vector<DBCONNECTION>	m_connections;
	int							m_retCode;

	int			get_dbIndex(std::string file);
	sqlite3*	get_dbHandle(int index);
	std::string get_dbFilename(int index);

	static CLog _sqlLog;
};