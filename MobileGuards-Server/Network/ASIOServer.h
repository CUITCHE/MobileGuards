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
protected:
	std::shared_ptr<SocketReadWriteHandler> createSocketHandler();
	void socketHandlerError(const SocketReadWriteHandler &socketHandler, const boost::system::error_code &erc);
	void socketHandlerReceived(const std::array<char, MAX_IP_PACK_SIZE> &buffer);
private:
	CLASS_MEMBER_DECLARE(ASIOServer);
};

#endif // ASIOSERVER_H
