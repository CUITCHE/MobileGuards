#include "stdafx.h"
#include "DBModule.h"


DBModule* DBModule::default()
{
	static DBModule *instance = new DBModule;
	return instance;
}

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
	qDebug() << "****************���ݿ����ģ���ʼ��****************";
	qDebug() << "Ĭ������:";
	qDebug() << "���ݿ��������ַ:" << MSSQLDBAccessHelperInfo.serverIP;
	qDebug() << "���ݿ�������˿�:" << MSSQLDBAccessHelperInfo.serverPort;
	qDebug() << "���ݿ�����:" << MSSQLDBAccessHelperInfo.serverDatebaseName;
	qDebug() << "���ݿ��û���:" << MSSQLDBAccessHelperInfo.databaseUserName;
	qDebug() << "���ݿ�����:" << MSSQLDBAccessHelperInfo.databasePassword;
	int trycount = 0;
	QSqlError::ErrorType errorType;
	forever {
		errorType = MSSQLConnectionHelper::initConnection();
		if (errorType == QSqlError::NoError || trycount >= 50) {
			break;
		}
		qDebug() << "�������ݿ�ʧ��" << ++trycount << "�Σ�5�������������......";
		QThread::msleep(5 * 1000);
	}
	if (errorType != QSqlError::NoError) {
		qDebug() << "����50���������ݿ�ʧ�ܣ����γ�ʼ��ʧ��!";
        emit dbmodule_connect_failure();
	}
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