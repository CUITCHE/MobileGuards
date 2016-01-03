#ifndef DISPATCHTRANSACTIONCENTER_H
#define DISPATCHTRANSACTIONCENTER_H

#include "packets.h"

class DispatchTransactionCenter : public QObject
{
	Q_OBJECT

public:
	DispatchTransactionCenter(QObject *parent = 0);
	~DispatchTransactionCenter();

	void putTransaction(transaction_packet &transaction);
protected:
	void registerTransaction();
	// return true if check success, otherwise is false.
	// will check secret-key and data field.
	bool dataSecurityCheck(const char *byteArray, size_t length, CommunicationProtocols &protocol);
signals:

private:
	CLASS_MEMBER_DECLARE(DispatchTransactionCenter);
};

#endif