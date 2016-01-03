#include "stdafx.h"
#include "ASIOServer.h"
#include "DispatchTransactionCenter.h"
#include <iostream>
#include <list>
using std::string;
using std::list;

const SocketReadWriteHandler::Identity MAX_CONNECTION_ID = 65535;

CLASS_MEMBER_DEFINITION(ASIOServer)
{
	io_service *ios;
	tcp::acceptor *acceptor;
	QMap<SocketReadWriteHandler::Identity, std::shared_ptr<SocketReadWriteHandler>> *connectionsMap;
	list<SocketReadWriteHandler::Identity> *socketHandlerConnectionIdReserveList;
	DispatchTransactionCenter *dispatchTransactionCenter;
	CLASS_NAME_MEMBER(ASIOServer)(quint16 port)
		:ios(new io_service)
		, connectionsMap(new remove_pointer<decltype(connectionsMap)>::type)
		, dispatchTransactionCenter()
	{
		acceptor = new tcp::acceptor(*ios, tcp::endpoint(tcp::v4(), port));

		socketHandlerConnectionIdReserveList = new remove_pointer<decltype(socketHandlerConnectionIdReserveList)>::type;
		// 生成连接池所需的id编号
		socketHandlerConnectionIdReserveList->resize(MAX_CONNECTION_ID);
		std::iota(socketHandlerConnectionIdReserveList->begin(), socketHandlerConnectionIdReserveList->end(), 1);
	}
	~CLASS_NAME_MEMBER(ASIOServer)()
	{
		delete_ptr(ios, acceptor, connectionsMap, socketHandlerConnectionIdReserveList, dispatchTransactionCenter);
	}
};

ASIOServer::ASIOServer(quint16 port)
	: d(new CLASS_NAME_MEMBER(ASIOServer)(port))
{
}

ASIOServer::~ASIOServer()
{
    this->stopAccept();
}

bool ASIOServer::accept()
{
	qDebug() << "开始监听";
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
			// 为新连接进来的socket发起一次读数据请求
			socketHandler->asyncRead();
		} while (0);

		// 发起下一次accept请求
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
	if (d->socketHandlerConnectionIdReserveList->empty()) {
		qDebug() << "连接池已满，不再接收新的连接";
		return std::shared_ptr<SocketReadWriteHandler>(nullptr);
	}
	SocketReadWriteHandler::Identity connectId = d->socketHandlerConnectionIdReserveList->front();
	d->socketHandlerConnectionIdReserveList->pop_front();
	std::shared_ptr<SocketReadWriteHandler> socketHandler = std::make_shared<SocketReadWriteHandler>(*d->ios, connectId);
	socketHandler->registerErrorCallback(mv(std::bind(&ASIOServer::socketHandlerError, this, std::placeholders::_1, std::placeholders::_2)));
	socketHandler->registerRecieveCallback(mv(std::bind(&ASIOServer::socketHandlerReceived, this, std::placeholders::_1,std::placeholders::_2)));
	return socketHandler;
}

void ASIOServer::socketHandlerError(const SocketReadWriteHandler &socketHandler, const boost::system::error_code &erc)
{

}

void ASIOServer::socketHandlerReceived(const SocketReadWriteHandler &socketHandler, const std::vector<char> *buffer)
{
	if (!socketHandler.isVaild()) {  
		return;
	}
	auto iter = d->connectionsMap->find(socketHandler.id);
	if (iter != d->connectionsMap->end()) {
		transaction_packet transaction = { buffer, *iter };
		d->dispatchTransactionCenter->putTransaction(transaction);
	}
}

void ASIOServer::socketAcceptHandlerError(std::shared_ptr<SocketReadWriteHandler> socketHandler, const boost::system::error_code &erc)
{
	qDebug() << "socket accept接收错误" << erc.value() << erc.message().c_str();
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
	d->socketHandlerConnectionIdReserveList->remove(id);
	d->socketHandlerConnectionIdReserveList->push_back(id);
}
