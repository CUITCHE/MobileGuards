#include "stdafx.h"
#include "MSSQLConnectionHelper.h"

struct MSSQLDBAccessInfo MSSQLDBAccessHelperInfo;

QSqlError::ErrorType MSSQLConnectionHelper::initConnection()
{
	QSqlDatabase mssqlDB = QSqlDatabase::addDatabase("QODBC");
	const QString dsn = QString("Driver={SQL SERVER};SERVER=%1;PORT=%2;DATABASE=%3;UID=%4;PWD=%5;")
		.arg(MSSQLDBAccessHelperInfo.serverIP)
		.arg(MSSQLDBAccessHelperInfo.serverPort)
		.arg(MSSQLDBAccessHelperInfo.serverDatebaseName)
		.arg(MSSQLDBAccessHelperInfo.databaseUserName)
		.arg(MSSQLDBAccessHelperInfo.databasePassword);
	//	QString dsn = "Driver={sql server};SERVER=222.18.159.25;PORT=1433;DATABASE=Queue_Share;UID=sa;PWD=Dbke6413;";
	mssqlDB.setDatabaseName(dsn);
	if (mssqlDB.open())
		qDebug() << "数据库连接成功!!!";
	else
		qDebug() << "数据库连接失败!!!\n详细错误信息:" << mssqlDB.lastError();

	return mssqlDB.lastError().type();
}

void MSSQLConnectionHelper::closeConnection()
{
	QString connectionName;
	{
		QSqlDatabase mssqlDB = QSqlDatabase::database();
		if (mssqlDB.isOpen())
		{
			mssqlDB.close();
			connectionName = mssqlDB.connectionName();
		}
	}

	QSqlDatabase::removeDatabase(connectionName);
}

void MSSQLConnectionHelper::execQuery(QSqlQuery &query)
{
	checkDB();
	if (!query.exec())
		query.clear();
}

int MSSQLConnectionHelper::execDML(QSqlQuery &query)
{
	checkDB();
	if (query.exec())
		return query.numRowsAffected();
	return -1;
}

int MSSQLConnectionHelper::execScalar(QSqlQuery &execSql)
{
	QSqlQuery query;
	checkDB();
	if (query.exec())
		return query.size();
	return -1;
}

bool MSSQLConnectionHelper::checkDB()
{
	QSqlDatabase mssqlDB = QSqlDatabase::database();
	if (!mssqlDB.isOpen())
	{
		mssqlDB.open();
	}
	return mssqlDB.isOpen();
}
