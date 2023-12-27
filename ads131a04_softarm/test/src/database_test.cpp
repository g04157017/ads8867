#include "kydatabase.h"

int db_select_callback(void * data, int col_count, char ** col_values, char ** col_Name)
{
// 每条记录回调一次该函数,有多少条就回调多少次
  	int i;
  	for( i=0; i < col_count; i++){
    	printf( "%s = %s\n", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
  	}
  	return 0;
}

int main()
{
	int ret;
	char * pErrMsg = 0;
	DataBase * pDataBase;
	char*sql2;
	int i;
	int max_ID;
	char* sql = "1,2,3,201630506413,15.5 was a test";
	char **azResult=NULL; //二维数组存放结果
	 int nrow=0;
	 int ncolumn = 0;
	
	pDataBase = DataBase::GetInstance();

	ret = pDataBase->LoadDatabase("/home/data.db");

	if(ERR == ret)
	{
		printf("can't open the /home/data.db\n");
		return ERR;
	}

	ret = pDataBase->GetLargestRow("SenSorTable2", &nrow);
	
	if(ERR == ret)
	{
		printf("can't open the SenSorTable\n");
		return ERR;
	}

	printf("the nrow is = %d\n",nrow);
//	ret = pDataBase->AddData2Table(sql,"SenSorTable2");
//	
//	if(ERR == ret)
//	{
//		printf("can't open the SenSorTable\n");
//		return ERR;
//	}

//	ret = pDataBase->DeleteARowOfDataFromTheTable("SenSorTable2",1);
//	
//	if(ERR == ret)
//	{
//		printf("can't delete the DeleteARowOfDataFromTheTable\n");
//		return ERR;
//	}

//	ret = pDataBase->QueryAPieceOfDataFromATable("SenSorTable2", &azResult, 2);
//	
//	if(ERR == ret)
//	{
//		printf("can't  QueryAPieceOfDataFromATable\n");
//		return ERR;
//	}
//
//	printf("azResult =%s\n",azResult[0]);
//	printf("azResult =%s\n",azResult[1]);
//	printf("azResult =%s\n",azResult[2]);
//	printf("azResult =%s\n",azResult[3]);
	
	pDataBase->UnLoadDatabase();

//	ret=pDataBase->db_open_database((char *)DATABASE_PATH_DATA);
//	if(ERR == ret){
//		pDataBase->db_close_database();
//		return ERR;
//	}
	//ret = pDataBase->Init();
//	printf("main %s\n",TabName);

/*	ret = pDataBase->db_Insert_Table(TabName,sql);
	ret = pDataBase->db_Insert_Table(TabName,"2,2,3,201630506413,15.5");
*/
//	ret = pDataBase->db_get_maxID(TabName, &max_ID);
//	printf("max ID = %d\n",max_ID);
//	ret = pDataBase->db_Insert_Table(TabName,"201630506413","200,100,200,300",1111);
//	ret = pDataBase->db_Insert_Table(TabName,0,"201630506413","2","3");
//	ret = pDataBase->db_update_one_group_data(TabName,"SensorID", 1000, 1);
//	ret = pDataBase->db_delete_one_group_data(TabName,1);
//	ret = pDataBase->db_delete_all_data_InTab(TabName);
//	ret = pDataBase->db_get_maxID(TabName, &max_ID);
//	printf("max ID = %d\n",max_ID);
//	
//	ret = pDataBase->db_Query_ALLTable(TabName, &azResult, &ncolumn, &nrow);
//	printf("nrow=%d ncolumn=%d\n",nrow,ncolumn);
//    printf("the result is:\n");
//    for(i=0;i<(nrow+1)*ncolumn;i++){
//    	printf("azResult[%d]=%s\n",i,azResult[i]);
//    }
////	ret = pDataBase->db_delete_one_table(TabName);
//	sqlite3_free_table(azResult);
//	pDataBase->db_close_database();
	while(1)
		{
			sleep(1);
		}
}
