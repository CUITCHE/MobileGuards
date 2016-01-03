#include "stdafx.h"
#include "CommunicationPackageBase.h"

QDataStream& operator>>(QDataStream &in, CommunicationProtocols &right)
{
	unsigned int &p = reinterpret_cast<unsigned int &>(right);
	in >> p;
	return in;
}

QDataStream& operator<<(QDataStream &out, const CommunicationProtocols &right)
{
	out << reinterpret_cast<const unsigned int&>(right);
	return out;
}

CommunicationPackageBase::CommunicationPackageBase(CommunicationProtocols protocol, QObject *parent)
	:QObject(parent)
	, communicationProtocol(protocol)
{
}

CommunicationPackageBase::~CommunicationPackageBase()
{
}

QDataStream& operator<<(QDataStream &out, const CommunicationPackageBase &right)
{
	out << right.communicationProtocol;
	return out;
}

QDataStream& operator>>(QDataStream &in, CommunicationPackageBase &right)
{
	in >> right.communicationProtocol;
	return in;
}
