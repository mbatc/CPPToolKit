#include "CSQLInterface.h"

#include "../../tools/CLog.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

CSQLInterface* CSQLInterface::_instance = nullptr;
CLog CSQLInterface::_sqlLog;

CSQLInterface * CSQLInterface::get_instance() {
	if (!_instance)
	{
		_instance = new CSQLInterface();
		if (!_instance)
			_Log(nullptr, DEBUGLOG_LEVEL_ERROR, "Failed to create CSQLInterface instance!");
		else
			_Log(nullptr, DEBUGLOG_LEVEL_INFO, "Creating CSQLInterface instance (ptr=%p)", _instance);
	}

	return _instance;
}

bool CSQLInterface::connectDB(std::string db)
{
	sqlite3 *_hDb;
	m_retCode = sqlite3_open(db.c_str(), &_hDb);
	
	if (m_retCode)
	{
		_Log(this, DEBUGLOG_LEVEL_ERROR, "Error opening sql database: %s", sqlite3_errmsg(_hDb));
		sqlite3_close(_hDb);
		return false;
	}
	
	_Log(this,DEBUGLOG_LEVEL_INFO, "Opened connection with %s\n\n" ,db.c_str());

	DBCONNECTION con;
	con.filename = db;
	con._handle_ = _hDb;
	
	m_connections.push_back(con);
	return true;
}

bool CSQLInterface::closeDB(std::string db)
{
	int db_index = get_dbIndex(db);
	if (!VALID_INDEX(db_index, m_connections))
	{
		_Log(this, DEBUGLOG_LEVEL_WARNING, "closeDB() Error: No connection with %s exists", db.c_str());
		return false;
	}
	sqlite3_close(m_connections[db_index]._handle_);

	m_connections.erase(m_connections.begin() + db_index);
	return true;
}

void CSQLInterface::set_activedb(std::string db)
{
	int index = get_dbIndex(db);
	if (!VALID_INDEX(index, m_connections))
	{
		_Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to set active database to %s: no connection exists.", db.c_str());

		if (!m_connections.size())
			m_curConn = -1;
		return;
	}
	m_curConn = index;
}

bool CSQLInterface::execute_query(std::string query, SQLFILE_DATA data, bool clear_results)
{
	char* error = nullptr;
	sqlite3* con = get_dbHandle(m_curConn);

	if (!con)
		return false;

	if(clear_results)
		this->clear_results(m_curConn);

	int kw_start_index = 0;
	//Replace all specified keywords in the script with the data provided
	for (int i = 0; i < data.keywords.size() && i < data.replace.size();)
	{
		size_t kw_index = query.find(data.keywords[i], kw_start_index);
		if (kw_index == std::string::npos)
		{
			i++;
			kw_start_index = 0;
			continue;
		}

		query.replace(kw_index, data.keywords[i].length(), data.replace[i]);
		kw_start_index = kw_index + 1;
	}
	
	std::string query_lower = query;
	for (int i = 0; i < query_lower.size(); i++)
	{
		query_lower[i] = ::tolower(query_lower[i]);
	}

	query_lower = strip_comments(query_lower);
	query_lower = strip_leading_whitespace(query_lower);

	if (query_lower.find_first_of("select") == 0)
	{
		return execute_select(query);
	}

	m_retCode = sqlite3_exec(con, query.c_str(), nullptr, nullptr, &error);
	if (m_retCode)
	{
		_Log(this, DEBUGLOG_LEVEL_ERROR, "Error executing SQL statement: %s", sqlite3_errmsg(con));
		sqlite3_free(error);
		return false;
	}
	return true;
}

int CSQLInterface::execute_script(std::string text, SQLFILE_DATA data)
{
	clear_results(m_curConn);

	//Split text into each query separated by semi-colons
	std::vector<std::string> queries(1);
	int qIndex = 0;
	for (int i = 0; i < text.length(); i++)
	{
		queries[qIndex].push_back(text[i]);
		if (text[i] == ';')
		{
			qIndex++;
			queries.push_back(std::string());
		}
	}
	queries.pop_back();

	//Execute all queries and keep results from each one
	for (int i = 0; i < queries.size(); i++)
	{
		if (!execute_query(queries[i], data, false))
			return i+1;
	}

	return 0;
}

int CSQLInterface::execute_fscript(std::string file, SQLFILE_DATA data)
{
	ifstream stream;
	stream.open(file.c_str());
	if (!stream)
	{
		_Log(this, DEBUGLOG_LEVEL_ERROR, "Error could not open SQL script %s.", file.c_str());
		return 1;
	}
	char c = stream.get();
	std::string file_text;
	while (stream.good())
	{
		file_text.push_back(c);
		c = stream.get();
	}
	stream.close();

	return execute_script(file_text);
}

std::vector<CSQLInterface::DBTABLE> CSQLInterface::get_results()
{
	if (!VALID_INDEX(m_curConn, m_connections))
		return std::vector<DBTABLE>();
	return m_connections[m_curConn].results_table;
}

std::vector<CSQLInterface::DBTABLE> CSQLInterface::get_results(std::string db)
{
	int index = get_dbIndex(db);
	if (!VALID_INDEX(index, m_connections))
		return std::vector<DBTABLE>();
	return m_connections[index].results_table;
}

CSQLInterface::DBTABLE CSQLInterface::get_results(int index)
{
	if (!VALID_INDEX(m_curConn, m_connections))
		return DBTABLE();
	if (!VALID_INDEX(index, m_connections[m_curConn].results_table))
		return DBTABLE();
	return m_connections[m_curConn].results_table[index];
}

CSQLInterface::DBTABLE CSQLInterface::get_results(std::string db, int index)
{
	int dbIndex = get_dbIndex(db);
	if (!VALID_INDEX(dbIndex, m_connections))
		return DBTABLE();
	if (!VALID_INDEX(index, m_connections[m_curConn].results_table))
		return DBTABLE();
	return m_connections[dbIndex].results_table[index];
}

const char * CSQLInterface::get_errmsg(std::string db)
{
	int i = get_dbIndex(db);
	if (!VALID_INDEX(i, m_connections))
		return nullptr;

	return sqlite3_errmsg(m_connections[i]._handle_);
}

void CSQLInterface::_Log(const void * src_obj, unsigned int level, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	_sqlLog.Log(src_obj, level, format, args);
	va_end(args);
}

bool CSQLInterface::execute_select(std::string query)
{
	char* error = nullptr;
	char** ppRes = nullptr;
	int nRow = 0, nCol = 0;
	sqlite3* con = get_dbHandle(m_curConn);
	m_retCode = sqlite3_get_table(con, query.c_str(), &ppRes, &nRow, &nCol, &error);
	if (m_retCode)
	{
		_Log(this, DEBUGLOG_LEVEL_ERROR, "Error executing SQL Query: %s", sqlite3_errmsg(con));
		sqlite3_free(error);
	}

	DBTABLE results_table;
	results_table.nCol = nCol;
	//Exclude the header row
	results_table.nRows = nRow - 1;

	results_table.headers.resize(nCol);
	results_table.results.resize(nCol*(nRow));

	for (int c = 0; c < nCol; c++)
		results_table.headers[c] = ppRes[c];

	for (int r = 1; r < nRow; r++)
	{
		for (int c = 0; c < nCol; c++)
		{
			results_table.results[((r - 1)*nCol) + c] = ppRes[(r*nCol) + c];
		}
	}
	sqlite3_free_table(ppRes);
	m_connections[m_curConn].results_table.push_back(results_table);
	return false;
}

void CSQLInterface::clear_results(int dbIndex)
{
	if (!VALID_INDEX(dbIndex, m_connections))
		return;
	m_connections[dbIndex].results_table.clear();
}

std::string CSQLInterface::strip_comments(std::string s)
{
	std::string stripped = "";
	bool inComment = false;
	for (int i = 0; i < s.length(); i++)
	{
		if(i+1 < s.length())
			if (s[i] == '/' && s[i+1] == '*')
				inComment = true;

		if (!inComment)
			stripped.push_back(s[i]);

		if (i - 1 >= 0)
			if (s[i-1] == '*' && s[i] == '/')
				inComment = false;

	}
	return stripped;
}

std::string CSQLInterface::strip_leading_whitespace(std::string s)
{
	int start_i = 0;
	while (s[start_i] == ' ' || s[start_i] == '\n' || s[start_i] == '\t'
		|| s[start_i] == '\r')
		start_i++;

	s = &s.c_str()[start_i];
	return s;
}

int CSQLInterface::get_dbIndex(std::string file)
{
	for (int i = 0; i < m_connections.size(); i++)
	{
		if (m_connections[i].filename == file)
			return i;
	}
	return -1;
}

sqlite3* CSQLInterface::get_dbHandle(int index)
{
	if (!VALID_INDEX(index, m_connections))
		return nullptr;
	return m_connections[index]._handle_;
}

std::string CSQLInterface::get_dbFilename(int index)
{
	if (!VALID_INDEX(index, m_connections))
		return std::string();
	return m_connections[index].filename;
}

