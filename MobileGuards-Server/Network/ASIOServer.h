#ifndef ASIOSERVER_H
#define ASIOSERVER_H

#include <QObject>
#include "SocketReadWriteHandler.h"

class SocketReadWriteHandler;

class ASIOServer : public QObject
{
	Q_OBJECT

public:
	ASIOServer(quint16 port, QObject *parent = 0);
	~ASIOServer();

	bool accept();
	void stopAccept();

	// Close socket handler and recycl Identity label.
	void killSocketHandler(std::shared_ptr<SocketReadWriteHandler> &socketHandler);
protected:
	std::shared_ptr<SocketReadWriteHandler> createSocketHandler();
	void socketHandlerError(const SocketReadWriteHandler &socketHandler, const boost::system::error_code &erc);
	void socketHandlerReceived(const SocketReadWriteHandler &socketHandler, const std::array<char, MAX_IP_PACK_SIZE> &buffer);

	// Handle the server socket's error.
	void socketAcceptHandlerError(std::shared_ptr<SocketReadWriteHandler> socketHandler, const boost::system::error_code &erc);

	void recycleIdentity(SocketReadWriteHandler::Identity id);
private:
	CLASS_MEMBER_DECLARE(ASIOServer);
};

#endif // ASIOSERVER_H
