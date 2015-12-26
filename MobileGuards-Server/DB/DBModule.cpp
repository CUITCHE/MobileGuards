#include "stdafx.h"
#include "DBModule.h"

PREPARE_INSTANCE_DEFINITION(DBModule);

DBModule::DBModule(QObject *parent)
	:QThread(parent)
{
}

DBModule::~DBModule()
{

}

void DBModule::initConnect()
{
	// 从配置文件中读取数据库的相关信息
	QSqlError::ErrorType errorType;
	qDebug() << "****************数据库服务模块初始化****************";
	qDebug() << "默认配置:";
	qDebug() << "数据库服务器地址:" << MSSQLDBAccessHelperInfo.serverIP;
	qDebug() << "数据库服务器端口:" << MSSQLDBAccessHelperInfo.serverPort;
	qDebug() << "数据库名称:" << MSSQLDBAccessHelperInfo.serverDatebaseName;
	qDebug() << "数据库用户名:" << MSSQLDBAccessHelperInfo.databaseUserName;
	qDebug() << "数据库密码:" << MSSQLDBAccessHelperInfo.databasePassword;
	int trycount = -1;
	do
	{
		++trycount;
		errorType = MSSQLConnectionHelper::initConnection();
		if (trycount > 0){
			qDebug() << "连接数据库失败" << trycount + 1 << "次，5秒后尝试重新连接......";
			QThread::msleep(5 * 1000);
		}
	} while (errorType != QSqlError::NoError);
}

void DBModule::disconnect()
{
	MSSQLConnectionHelper::closeConnection();
}

void DBModule::reconnect()
{
	disconnect();

	MSSQLConnectionHelper::initConnection();
}