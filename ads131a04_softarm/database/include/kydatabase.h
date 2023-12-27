#ifndef __kydatabase_h_
#define __kydatabase_h_
#include<stdio.h>
#include "def.h"
#include "sqlite3.h"

#ifdef DEBUG_FLAG
#define database_log(format, ...) database_dbg(format, ##__VA_ARGS__)
#else
#define database_log(format, ...)
#endif

class DataBase
{
public:
	DataBase();
	virtual ~DataBase();

public:
	int DeInit();
	static DataBase* GetInstance();

	int LoadDatabase(pcstr Database_file);
	int UnLoadDatabase();


	int AddData2Table(pcstr message,const std::string & DataSheet,int DataLen);

	int	UpdateAPieceOfDataFromTheTable(const char *tbname,const char *dz,int value,int ID);
	
	int QueryAPieceOfDataFromATable(const std::string &DataSheet, char ***pazResult, int  ID);

	int DeleteARowOfDataFromTheTable(const std::string &DataSheet, int  ID);

	int GetLargestRow(const std::string &DataSheet,int *nrow);
	bool JudgeWhetherTheTableExists(sqlite3 *db, const std::string& tbname);
private:	
	static DataBase* m_DataBase;
	sqlite3*  m_pdatabase;
};

#endif
