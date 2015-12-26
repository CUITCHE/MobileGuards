#ifndef MSSQLCONNECTIONHELPER_H
#define MSSQLCONNECTIONHELPER_H
/********************************************************************
	created:	2015/02/19
	created:	19:2:2015   12:21
	file base:	MSSQLConnectionHelper
	author:		CHE
	
	purpose:	����odbc���ӵ�MSSQL
*********************************************************************/
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QSqlError>

struct MSSQLDBAccessInfo 
{
	QString serverIP;
	quint32 serverPort;
	QString serverDatebaseName;
	QString databaseUserName;
	QString databasePassword;
};

extern struct MSSQLDBAccessInfo MSSQLDBAccessHelperInfo;

class MSSQLConnectionHelper
{
public:
	MSSQLConnectionHelper() = delete;
	~MSSQLConnectionHelper() = delete;
	//���ӵ����ݿ�
	static QSqlError::ErrorType initConnection();

	//�Ͽ����ݿ�����
	static void closeConnection();

	//ִ��sql��䲢�����еĽ����
	template<typename... Args>
	static const QSqlQuery execQuery(const QString &execSql, Args... args)
	{
		QSqlQuery query;
		query.prepare(execSql);
		execSql_impl(query, std::forward<Args>(args)...);
		execQuery(query);
		return query;
	}

	//ִ��sql��䲢�����е�����������Ӱ����������磺select,update,delete,insert��
	template<typename... Args>
	static int execDML(const QString &execSql, Args... args)
	{
		QSqlQuery query;
		query.prepare(execSql);
		execSql_impl(query, std::forward<Args>(args)...);
		return execDML(query);
	}

	//ִ��sql��䲢���ؽ����������
	template<typename... Args>
	inline static int execScalar(const QString &execSql, Args... args)
	{
		QSqlQuery query;
		query.prepare(execSql);
		execSql_impl(query, std::forward<Args>(args)...);
		return execScalar(query);
	}

	//ִ�д洢���̣����Զ�����һ��return output�������ڵ�һ��[0]λ��
	template<typename... Args>
	inline	static const QSqlQuery execProcedure(const QString &execSql, Args... args)
	{
		QSqlQuery query;
		query.prepare(execSql);
		index = 0;
		query.bindValue(0,0,QSql::InOut);
		execSql_impl(query, std::forward<Args>(args)...);
		execQuery(query);
		return query;
	}
protected:
	//[ģ��] ��sql������Ӳ���
	static void execSql_impl(QSqlQuery &query){}
	template<typename SqlType, typename... Args>
	static void execSql_impl(QSqlQuery &query, const SqlType &d, Args...args){
		query.bindValue(++index, d);
		execSql_impl(query, std::forward<Args>(args)...);
	}

	//ִ��sql��䲢�����еĽ����
	static void execQuery(QSqlQuery &query);

	//ִ��sql��䲢�����е�����������Ӱ����������磺select,update,delete,insert��
	static int execDML(QSqlQuery &query);

	//ִ��sql��䲢���ؽ����������
	static int execScalar(QSqlQuery &query);

private:
	//��������Ƿ���open״̬��open����true
	static bool checkDB();
};

#endif // MSSQLCONNECTIONHELPER_H