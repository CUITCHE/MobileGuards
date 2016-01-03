#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "SocketReadWriteHandler.h"
#include "ThreadPool.h"
#include "ASIOServer.h"
#include "TransactionStructure.h"
#include "ClientTestPacket.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ThreadPool<transaction_packet> pool([](const transaction_packet &ob){});
	ClientTestPacket test;
	test.test1() = 2258;
	
	unsigned int abc = 50;
	CommunicationProtocols p = dynamic_protocol_cast(abc);
	qDebug() << "Äãº¾";
	return a.exec();
}
