#ifndef MSSQLCONNECTIONHELPER_H
#define MSSQLCONNECTIONHELPER_H
/********************************************************************
	created:	2015/02/19
	created:	19:2:2015   12:21
	file base:	MSSQLConnectionHelper
	author:		CHE
	
	purpose:	采用odbc连接到MSSQL
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
	//连接到数据库
	static QSqlError::ErrorType initConnection();

	//断开数据库连接
	static void closeConnection();

	//执行sql语句并返回行的结果集
	template<typename... Args>
	static const QSqlQuery execQuery(const QString &execSql, Args... args)
	{
		QSqlQuery query;
		query.prepare(execSql);
		execSql_impl(query, std::forward<Args>(args)...);
		execQuery(query);
		return query;
	}

	//执行sql语句并返回行的数量，即受影响的行数，如：select,update,delete,insert等
	template<typename... Args>
	static int execDML(const QString &execSql, Args... args)
	{
		QSqlQuery query;
		query.prepare(execSql);
		execSql_impl(query, std::forward<Args>(args)...);
		return execDML(query);
	}

	//执行sql语句并返回结果集的数量
	template<typename... Args>
	inline static int execScalar(const QString &execSql, Args... args)
	{
		QSqlQuery query;
		query.prepare(execSql);
		execSql_impl(query, std::forward<Args>(args)...);
		return execScalar(query);
	}

	//执行存储过程，会自动添加一个return output参数放在第一个[0]位置
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
	//[模版] 给sql语句添加参数
	static void execSql_impl(QSqlQuery &query){}
	template<typename SqlType, typename... Args>
	static void execSql_impl(QSqlQuery &query, const SqlType &d, Args...args){
		query.bindValue(++index, d);
		execSql_impl(query, std::forward<Args>(args)...);
	}

	//执行sql语句并返回行的结果集
	static void execQuery(QSqlQuery &query);

	//执行sql语句并返回行的数量，即受影响的行数，如：select,update,delete,insert等
	static int execDML(QSqlQuery &query);

	//执行sql语句并返回结果集的数量
	static int execScalar(QSqlQuery &query);

private:
	//检测数据是否处于open状态，open返回true
	static bool checkDB();
};

#endif // MSSQLCONNECTIONHELPER_H
