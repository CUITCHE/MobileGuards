#include "stdafx.h"
#include "ASIOServer.h"
#include <iostream>
using std::string;
const SocketReadWriteHandler::Identity MAX_CONNECTION_ID = 65535;
CLASS_MEMBER_DEFINITION(ASIOServer)
{
	io_service *ios;
	tcp::acceptor *acceptor;
	QMap<SocketReadWriteHandler::Identity, std::shared_ptr<void>> *connectionsMap;
	QList<SocketReadWriteHandler::Identity> *socketHandlerConnectionIdReserveList;
	CLASS_NAME_MEMBER(ASIOServer)(quint16 port)
		:ios(new io_service)
		, connectionsMap(new remove_pointer<decltype(connectionsMap)>::type)
	{
		acceptor = new tcp::acceptor(*ios, tcp::endpoint(tcp::v4(), port));

		socketHandlerConnectionIdReserveList = new remove_pointer<decltype(socketHandlerConnectionIdReserveList)>::type;
		// �������ӳ������id���
		socketHandlerConnectionIdReserveList->reserve(MAX_CONNECTION_ID);
		std::iota(socketHandlerConnectionIdReserveList->begin(), socketHandlerConnectionIdReserveList->end(), 1);
	}
	~CLASS_NAME_MEMBER(ASIOServer)()
	{
		delete_ptr(ios, acceptor, connectionsMap, socketHandlerConnectionIdReserveList);
	}
};

ASIOServer::ASIOServer(quint16 port, QObject *parent)
	: QObject(parent)
	, d(new CLASS_NAME_MEMBER(ASIOServer)(port))
{
}

ASIOServer::~ASIOServer()
{

}

bool ASIOServer::accept()
{
	qDebug() << "��ʼ����";
	auto socketHandler = this->createSocketHandler();
	if (socketHandler == false) {
		return false;
	}
	d->acceptor->async_accept(socketHandler->getSocket(), [=](const boost::system::error_code &erc) {
		do {
			if (erc) {
				this->socketAcceptHandlerError(socketHandler, erc);
				break;
			}
			d->connectionsMap->insert(socketHandler->id, socketHandler);
			// Ϊ�����ӽ�����socket����һ�ζ���������
			socketHandler->asyncRead();
		} while (0);

		// ������һ��accept����
		this->accept();
	});
	return true;
}

void ASIOServer::stopAccept()
{
	boost::system::error_code erc;
	d->acceptor->cancel(erc);
	d->acceptor->close(erc);
	d->ios->stop();
}

std::shared_ptr<SocketReadWriteHandler> ASIOServer::createSocketHandler()
{
	if (d->socketHandlerConnectionIdReserveList->isEmpty()) {
		qDebug() << "���ӳ����������ٽ����µ�����";
		return std::shared_ptr<SocketReadWriteHandler>(nullptr);
	}
	SocketReadWriteHandler::Identity connectId = d->socketHandlerConnectionIdReserveList->takeFirst();
	std::shared_ptr<SocketReadWriteHandler> socketHandler = std::make_shared<SocketReadWriteHandler>(*d->ios, connectId);
	socketHandler->registerErrorCallback(mv(std::bind(&ASIOServer::socketHandlerError, this, std::placeholders::_1, std::placeholders::_2)));
	socketHandler->registerRecieveCallback(mv(std::bind(&ASIOServer::socketHandlerReceived, this, std::placeholders::_1,std::placeholders::_2)));
	return socketHandler;
}

void ASIOServer::socketHandlerError(const SocketReadWriteHandler &socketHandler, const boost::system::error_code &erc)
{

}

void ASIOServer::socketHandlerReceived(const SocketReadWriteHandler &socketHandler, const std::array<char, MAX_IP_PACK_SIZE> &buffer)
{

}

void ASIOServer::socketAcceptHandlerError(std::shared_ptr<SocketReadWriteHandler> socketHandler, const boost::system::error_code &erc)
{
	qDebug() << "socket accept���մ���" << erc.value() << erc.message().c_str();
	socketHandler->closeSocket();
	this->recycleIdentity(socketHandler->id);
}

void ASIOServer::killSocketHandler(std::shared_ptr<SocketReadWriteHandler> &socketHandler)
{
	// Close socket stream
	socketHandler->closeSocket();
	// Recycle identity to use for future.
	this->recycleIdentity(socketHandler->id);
	// Remove connection from socket-connection-map.
	d->connectionsMap->remove(socketHandler->id);
}

void ASIOServer::recycleIdentity(SocketReadWriteHandler::Identity id)
{
	d->socketHandlerConnectionIdReserveList->removeOne(id);
	d->socketHandlerConnectionIdReserveList->append(id);
}
