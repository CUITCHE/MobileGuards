#include "stdafx.h"
#include "ASIOServer.h"

const SocketReadWriteHandler::Identity MAX_CONNECTION_ID = 65535;
CLASS_MEMBER_DEFINITION(ASIOServer)
{
	io_service *ios;
	tcp::acceptor *acceptor;
	QMap<SocketReadWriteHandler::Identity, std::shared_ptr<void>> *connectionsMap;
	QList<SocketReadWriteHandler::Identity> *socketHandlerConnectionIdReserve;
	CLASS_NAME_MEMBER(ASIOServer)(quint16 port)
		:ios(new io_service)
		, connectionsMap(new remove_pointer<decltype(connectionsMap)>::type)
	{
		acceptor = new tcp::acceptor(*ios, tcp::endpoint(tcp::v4(), port));

		socketHandlerConnectionIdReserve = new remove_pointer<decltype(socketHandlerConnectionIdReserve)>::type;
		// 生成连接池所需的id编号
		socketHandlerConnectionIdReserve->reserve(MAX_CONNECTION_ID);
		std::iota(socketHandlerConnectionIdReserve->begin(), socketHandlerConnectionIdReserve->end(), 1);
	}
	~CLASS_NAME_MEMBER(ASIOServer)()
	{
		delete_ptr(ios, acceptor, connectionsMap, socketHandlerConnectionIdReserve);
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
	qDebug() << "开始监听";
	auto socketHandler = this->createSocketHandler();
	if (socketHandler == false) {
		return false;
	}
	d->acceptor->async_accept(socketHandler->getSocket(), [=](const boost::system::error_code &erc) {
		if (erc) {
			// TODO:错误处理
			return;
		}
		d->connectionsMap->insert(socketHandler->id, socketHandler);
		// 为新连接进来的socket发起一次读数据请求
		socketHandler->asyncRead();
		// 发起下一次accept请求
		this->accept();
	});
	return true;
}

std::shared_ptr<SocketReadWriteHandler> ASIOServer::createSocketHandler()
{
	if (d->socketHandlerConnectionIdReserve->isEmpty()) {
		qDebug() << "连接池已满，不再接收新的连接";
		return std::shared_ptr<SocketReadWriteHandler>(nullptr);
	}
	SocketReadWriteHandler::Identity connectId = d->socketHandlerConnectionIdReserve->takeFirst();
	std::shared_ptr<SocketReadWriteHandler> socketHandler = std::make_shared<SocketReadWriteHandler>(*d->ios, connectId);
	socketHandler->registerErrorCallback(mv(std::bind(&ASIOServer::socketHandlerError, this, std::placeholders::_1, std::placeholders::_2)));
	socketHandler->registerRecieveCallback(mv(std::bind(&ASIOServer::socketHandlerReceived, this, std::placeholders::_1)));
	return socketHandler;
}

void ASIOServer::socketHandlerError(const SocketReadWriteHandler &socketHandler, const boost::system::error_code &erc)
{

}

void ASIOServer::socketHandlerReceived(const std::array<char, MAX_IP_PACK_SIZE> &buffer)
{

}
