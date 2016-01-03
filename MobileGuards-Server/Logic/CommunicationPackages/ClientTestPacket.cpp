#include "stdafx.h"
#include "ClientTestPacket.h"

ClientTestPacket::ClientTestPacket()
	: CommunicationPackageBase(CommunicationProtocols::ClientTest)
{

}

ClientTestPacket::~ClientTestPacket()
{

}

QDataStream& operator<<(QDataStream &out, const ClientTestPacket &right)
{
	out << static_cast<const CommunicationPackageBase&>(right) << right.test1();
	return out;
}
