#ifndef SOCKETREADWRITEHANDLER_H
#define SOCKETREADWRITEHANDLER_H

#include <QObject>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

const int MAX_IP_PACK_SIZE = 65535;

class ASIOServer;

class SocketReadWriteHandler : public QObject
{
	Q_OBJECT

public:
	using Identity = int;
	const Identity id;

	using ErrorCallbackType = std::function < void(const SocketReadWriteHandler&, const boost::system::error_code&) > ;
	using ReceivedCallbackType = std::function < void(const SocketReadWriteHandler&, const std::array<char, MAX_IP_PACK_SIZE>&) >;

	SocketReadWriteHandler(io_service &ios, Identity id, QObject *parent = 0);
	~SocketReadWriteHandler();

	void asyncRead();
	size_t syncWrite(const char *data, size_t size);

	const tcp::socket& getSocket() const;
	tcp::socket& getSocket();

	void registerErrorCallback(ErrorCallbackType &&errorCallback);
	void registerRecieveCallback(ReceivedCallbackType &&recieveCallback);

protected:
	void closeSocket();

private:
	CLASS_MEMBER_DECLARE(SocketReadWriteHandler);
	friend class ASIOServer;
};

#endif // SOCKETREADWRITEHANDLER_H
