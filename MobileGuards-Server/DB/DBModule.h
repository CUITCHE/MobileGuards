#ifndef DBMODULE_H
#define DBMODULE_H
/********************************************************************
	created:	2015/02/19
	created:	19:2:2015   13:01
	file base:	DBModule
	author:		CHE
	
	purpose:	�����ݿ�������ƣ�����prepareģʽ����DBModule��
				��дΪ����������ģʽ����ȡ���˾�̬���ò���
*********************************************************************/
#include "MSSQLConnectionHelper.h"
#include <QThread>

class DBModule: QThread
{
	PREPARE_INSTANCE_DECLARE(DBModule)
public:
	//��ʼ�����ݿ�����
	void initConnect();

	//�ر����ݿ�����
	void disconnect();

	//�����������ݿ�
	void reconnect();

private:
	DBModule(QObject *parent = 0);
	~DBModule();
};

#endif // DBMODULE_H