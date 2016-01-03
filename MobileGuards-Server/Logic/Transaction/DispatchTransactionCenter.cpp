#include "stdafx.h"
#include "DispatchTransactionCenter.h"
#include "TransactionCenter.h"
#include "CommunicationPackageBase.h"

CLASS_MEMBER_DEFINITION(DispatchTransactionCenter)
{
	TransactionCenter *transactionCenter;
	std::map<CommunicationProtocols, std::function<std::shared_ptr<CommunicationPackageBase>()>> *factoryMap;
	CLASS_NAME_MEMBER(DispatchTransactionCenter)()
		: CLASS_MEMBER_CONSTRUCTURE(transactionCenter,)
		, CLASS_MEMBER_CONSTRUCTURE(factoryMap,)
	{}
};

DispatchTransactionCenter::DispatchTransactionCenter(QObject *parent)
	:QObject(parent)
	, d(new CLASS_NAME_MEMBER(DispatchTransactionCenter))
{
	this->registerTransaction();
}

DispatchTransactionCenter::~DispatchTransactionCenter()
{
}

void DispatchTransactionCenter::putTransaction(transaction_packet &transaction)
{
	// data check.
	// ensure protocol and rest field is legal
	CommunicationProtocols protocol = CommunicationProtocols::ClientProtocolBegin;
	if (this->dataSecurityCheck(transaction.buffer->data(), transaction.socket->lastReceivedLength(), protocol)) {
		qDebug() << "Data check failed! This data packet is illegal, throw it. And will stop socket";
		transaction.socket->resetRead();
		return;
	}
	
	auto iter = d->factoryMap->find(protocol);
	if (iter == d->factoryMap->end()) {
		qDebug() << "You have forgotten to add method. " << "legal protocol is " << static_cast<unsigned int>(protocol);
		assert(0);
		return;
	}
	auto packet = (iter->second)();
	// emit Qt signal
}

void DispatchTransactionCenter::registerTransaction()
{
#ifndef _register_
#define _register_(type) d->factoryMap->emplace(std::make_pair(CommunicationProtocols::type, []{return std::make_shared<type##Packet>(); }))
#endif // !_register_

	_register_(ClientTest);
	d->factoryMap->emplace(std::make_pair(CommunicationProtocols::ClientTest, []{return std::make_shared<ClientTestPacket>(); }));

#ifdef _register_
#undef _register_
#endif // _register_

}

bool DispatchTransactionCenter::dataSecurityCheck(const char *byteArray, size_t length, CommunicationProtocols &protocol)
{
	bool success = false;
	do {
		IntegerAndChar integer(byteArray);
		protocol = dynamic_protocol_cast(integer.d);
		if (protocol == CommunicationProtocols::ProtocolNotExist) {
			qDebug() << "protocol illegal!";
			break;
		}
		// get secret-key and data field.
		byteArray += 4;
		success = true;
	} while (0);
	return success;
}
