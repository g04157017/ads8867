#include "kydatabase.h"

void database_dbg(const char* strlog, ...)
{
	printf  ("[DataBase dbg]: ");
	va_list vp;
	va_start(vp, strlog);
	vprintf (strlog, vp);
	va_end  (vp);
	printf  ("\n");
	return;
}

DataBase::DataBase()
{
	m_pdatabase = NULL;
}

DataBase::~DataBase()
{
	DeInit();
}

int DataBase::DeInit()
{
	int ret = ERR;
	if(NULL != m_pdatabase)
	{
		CHK_ERR(ret,ERR);
		m_pdatabase = NULL;
	}
	return OK;
}

DataBase* DataBase::m_DataBase = NULL_PTR;
DataBase* DataBase::GetInstance()
{
	if (NULL_PTR == m_DataBase)
		{
			m_DataBase = new DataBase();
		}
	return (m_DataBase == NULL_PTR) ? NULL_PTR : m_DataBase;
}

int DataBase::LoadDatabase(pcstr Database_file)
{
	int ret = ERR;
	
	CHK_NULL(Database_file, ERR);
	if (NULL == m_pdatabase) 
    {
        ret =  sqlite3_open(Database_file, &m_pdatabase);
        CHK_ERR(ret, ERR);
    }

	ret = sqlite3_busy_timeout(m_pdatabase,1000);
	if(ret != SQLITE_OK)
	{
		UnLoadDatabase();
		return ERR;
	}
    return OK;
}
int DataBase::UnLoadDatabase()
{
	int ret = ERR;
	
	if (NULL == m_pdatabase) 
    {
        return OK;
    }
    
   	ret = sqlite3_close(m_pdatabase);
	CHK_ERR(ret,ERR);
    
    m_pdatabase = NULL;
    while (system("sync") < 0) {
        database_log("cmd: sync\t error: %s", strerror(errno));
	}
    return OK;
}

int DataBase::AddData2Table(pcstr message,const std::string &DataSheet,int DataLen)
{
	int ret = ERR;
	int Max_ID = 0;
	char * pErrMsg = 0;
	char Max_ID_P[10]={0};
	char DataLen_P[10]={0};
	std::string strFindTable;

	if(!JudgeWhetherTheTableExists(m_pdatabase,DataSheet))
	{
		database_log("The datasheet was not exist !");

		strFindTable = "CREATE TABLE '" + DataSheet + "'(ID INTEGER  PRIMARY KEY NOT NULL,RECORD TEXT NOT NULL,RECORDLEN INTEGER NOT NULL);";

		ret=sqlite3_exec(m_pdatabase,strFindTable.c_str(),0, 0, &pErrMsg);
		if(OK != ret)
		{
			database_log("creat table  fail, close the Database!");
			ret = sqlite3_close(m_pdatabase);
			CHK_ERR(ret,ERR);
		}
	}
	ret = GetLargestRow(DataSheet,&Max_ID);
	CHK_ERR(ret,ERR);

	if(Max_ID <=1000)
	{
		Max_ID = Max_ID+1;
		sprintf(Max_ID_P,"%d",Max_ID);

		sprintf(DataLen_P,"%d",DataLen);
		
		strFindTable = "INSERT OR REPLACE INTO '"+ DataSheet +"' VALUES('"+Max_ID_P+"','"+message+"','"+DataLen_P+"');";
		ret = sqlite3_exec(m_pdatabase, strFindTable.c_str(), 0, 0, &pErrMsg);
		CHK_ERR(ret,ERR);
		sqlite3_free(pErrMsg);
		while (system("sync") < 0) {
			database_log("Can't Sync Database File !");
		}
	}else
		return ERR;
	return OK;
}
/*
函数功能：删除某个特定ID的一组数值,自下而上
*/
int DataBase::DeleteARowOfDataFromTheTable(const std::string &DataSheet, int  ID)
{
	int ret;
	char* szError=0;
	char ID_P[10]={0};
	std::string strFindTable;

	if(ID !=0 )
	{

		sprintf(ID_P,"%d",ID);

		strFindTable = "delete from '"+DataSheet+"' where ID = '"+ID_P+"';";
		ret = sqlite3_exec(m_pdatabase,strFindTable.c_str() , 0, 0, &szError);
		if( ret != SQLITE_OK ) 
		{
			database_log("can't DeleteARowOfDataFromTheTable : %s", sqlite3_errmsg(m_pdatabase));
			sqlite3_free(szError);
			return ERR;
		}
		while (system("sync") < 0) {
			database_log("Can't Sync Database File !");
		}
		sqlite3_free(szError);
	}
	return OK;
}

int	DataBase::QueryAPieceOfDataFromATable(const std::string &DataSheet, char ***pazResult, int  ID)
{
	int ret;
	char* szError=0;
	char ID_P[10]={0};
	int pnColumn = 0;
	int pnRow = 0;
	std::string strFindTable;

	if(ID != 0)
	{
		sprintf(ID_P,"%d",ID);
		strFindTable = "select * from '"+DataSheet+"'where ID = '"+ID_P+"';";

		ret = sqlite3_get_table( m_pdatabase , strFindTable.c_str() , pazResult , &pnRow , &pnColumn , &szError );
		if( ret != SQLITE_OK ) {
	    	database_log("can't query table : %s", sqlite3_errmsg(m_pdatabase));
			sqlite3_free(szError);
	    	return ERR;
	  	}
		sqlite3_free(szError);
	}
	return OK;		
}

int DataBase::GetLargestRow(const std::string &DataSheet,int *nrow)
{
	char **azResult=NULL; //二维数组存放结果
	char* szError=0;
	int ncolumn = 0;
	std::string strFindTable;
	int ret = 0;

	strFindTable = "select *from '"+DataSheet+"'";
	ret = sqlite3_get_table( m_pdatabase , strFindTable.c_str() , &azResult , nrow , &ncolumn , &szError );
	if( ret != SQLITE_OK ) {
    	database_log("can't query table : %s", sqlite3_errmsg(m_pdatabase));
		sqlite3_free(szError);
		sqlite3_free_table(azResult);
    	return ERR;
  	}
	sqlite3_free(szError);
	sqlite3_free_table(azResult);
	return OK;	
}

int	DataBase::UpdateAPieceOfDataFromTheTable(const char *tbname,const char *dz,int value,int ID)
{
	int ret;
	char* szError=0;
	char sql[100];
	sprintf(sql,"update %s set %s = %d where ID = %d",tbname,dz,value,ID);
	ret = sqlite3_exec(m_pdatabase,sql , 0, 0, &szError);
    if( ret != SQLITE_OK ) {
    	database_log("can't delete all data in tab: %s", sqlite3_errmsg(m_pdatabase));
		sqlite3_free(szError);
    	return ERR;
  	}
	sqlite3_free(szError);
	return OK;
}

int sqlite3_callback_func( void* pHandle,int iRet, char** szSrc, char** szDst )
{
	if ( 1 == iRet )
	{
		int iTableExist = atoi(*(szSrc));  //此处返回值为查询到同名表的个数，没有则为0，否则大于0
		if (pHandle != nullptr)
		{
			int* pRes = (int*)pHandle;
			*pRes = iTableExist;
		}
		// szDst 指向的内容为"count(*)"
	}
 
	return 0; //返回值一定要写，否则下次调用 sqlite3_exec(...) 时会返回 SQLITE_ABORT
}

/*
函数功能；检测数据库中表格是否存在
*/
bool DataBase::JudgeWhetherTheTableExists(sqlite3 *db, const std::string& tbname)
{
 	std::string strFindTable = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='" + tbname + "'";
	char* sErrMsg = nullptr;
	int nTableNums = 0;
	if (sqlite3_exec(db, strFindTable.c_str(), &sqlite3_callback_func, &nTableNums, &sErrMsg) != SQLITE_OK)
	{
		database_log(" no 'tbname' !");
		sqlite3_free(sErrMsg);
		return false;
	}
	sqlite3_free(sErrMsg);
	//回调函数无返回值，则此处第一次时返回SQLITE_OK， 第n次会返回SQLITE_ABORT
	return nTableNums > 0;
}

