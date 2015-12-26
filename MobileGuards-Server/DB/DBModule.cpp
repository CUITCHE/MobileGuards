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
	// �������ļ��ж�ȡ���ݿ�������Ϣ
	QSqlError::ErrorType errorType;
	qDebug() << "****************���ݿ����ģ���ʼ��****************";
	qDebug() << "Ĭ������:";
	qDebug() << "���ݿ��������ַ:" << MSSQLDBAccessHelperInfo.serverIP;
	qDebug() << "���ݿ�������˿�:" << MSSQLDBAccessHelperInfo.serverPort;
	qDebug() << "���ݿ�����:" << MSSQLDBAccessHelperInfo.serverDatebaseName;
	qDebug() << "���ݿ��û���:" << MSSQLDBAccessHelperInfo.databaseUserName;
	qDebug() << "���ݿ�����:" << MSSQLDBAccessHelperInfo.databasePassword;
	int trycount = -1;
	do
	{
		++trycount;
		errorType = MSSQLConnectionHelper::initConnection();
		if (trycount > 0){
			qDebug() << "�������ݿ�ʧ��" << trycount + 1 << "�Σ�5�������������......";
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