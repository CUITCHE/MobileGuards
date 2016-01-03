#ifndef COMMUNICATIONPACKAGEBASE_H
#define COMMUNICATIONPACKAGEBASE_H

#include "TransactionStructure.h"
#include <QObject>
#ifdef QT_NO_DATASTREAM
#error No QDataStream, No Server
#endif // QT_NO_DATASTREAM

class CommunicationPackageBase : public QObject
{
public:
	CommunicationPackageBase(CommunicationProtocols protocol, QObject *parent = 0);
	~CommunicationPackageBase();
	friend QDataStream& operator<<(QDataStream &out, const CommunicationPackageBase &right);
	friend QDataStream& operator>>(QDataStream &in, CommunicationPackageBase &right);
protected:
	CommunicationProtocols communicationProtocol;
};

#endif // COMMUNICATIONPACKAGEBASE_H
