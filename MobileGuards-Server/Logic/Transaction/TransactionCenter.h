#ifndef TRANSACTIONCENTER_H
#define TRANSACTIONCENTER_H

#include <QObject>

class TransactionCenter : public QObject
{
	Q_OBJECT

public:
	TransactionCenter(QObject *parent = 0);
	~TransactionCenter();

private:
	CLASS_MEMBER_DECLARE(TransactionCenter);
};

#endif