#ifndef DBMODULE_H
#define DBMODULE_H
/********************************************************************
	created:	2015/02/19
	created:	19:2:2015   13:01
	file base:	DBModule
	author:		CHE
	
	purpose:	对数据库操作控制，利用prepare模式。把DBModule类
				改写为面向对象的类模式，而取消了静态调用操作
*********************************************************************/
#include "MSSQLConnectionHelper.h"
#include <QThread>

class DBModule: QThread
{
	PREPARE_INSTANCE_DECLARE(DBModule)
public:
	//初始化数据库连接
	void initConnect();

	//关闭数据库连接
	void disconnect();

	//重新连接数据库
	void reconnect();

private:
	DBModule(QObject *parent = 0);
	~DBModule();
};

#endif // DBMODULE_H
