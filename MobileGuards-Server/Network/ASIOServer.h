#ifndef ASIOSERVER_H
#define ASIOSERVER_H

#include "SocketReadWriteHandler.h"

class SocketReadWriteHandler;

class ASIOServer
{
public:
	ASIOServer(quint16 port);
	~ASIOServer();

	bool accept();
	void stopAccept();

	// Close socket handler and recycl Identity label.
	void killSocketHandler(std::shared_ptr<SocketReadWriteHandler> &socketHandler);
protected:
	std::shared_ptr<SocketReadWriteHandler> createSocketHandler();
	void socketHandlerError(const SocketReadWriteHandler &socketHandler, const boost::system::error_code &erc);
	void socketHandlerReceived(const SocketReadWriteHandler &socketHandler, const std::vector<char> *buffer);

	// Handle the server socket's error.
	void socketAcceptHandlerError(std::shared_ptr<SocketReadWriteHandler> socketHandler, const boost::system::error_code &erc);

	void recycleIdentity(SocketReadWriteHandler::Identity id);
private:
	CLASS_MEMBER_DECLARE(ASIOServer);
};

#endif // ASIOSERVER_H
