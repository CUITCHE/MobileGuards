#ifndef PACKETTEST_H
#define PACKETTEST_H

#include "CommunicationPackageBase.h"

class ClientTestPacket : public CommunicationPackageBase
{
public:
	ClientTestPacket();
	~ClientTestPacket();
	friend QDataStream& operator<<(QDataStream &out, const ClientTestPacket &right);
	__member(int, test1);
};

#endif // PACKETTEST_H
