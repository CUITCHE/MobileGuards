#ifndef SOCKETREADWRITEHANDLER_H
#define SOCKETREADWRITEHANDLER_H

#include <QObject>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

const int MAX_IP_PACK_SIZE = 65536;

class SocketReadWriteHandler
{
public:
	using Identity = int;
	const Identity id;

	using ErrorCallbackType = std::function < void(const SocketReadWriteHandler&, const boost::system::error_code&) > ;
	using ReceivedCallbackType = std::function < void(const SocketReadWriteHandler&, const std::vector<char>*) >;

	SocketReadWriteHandler(io_service &ios, Identity id);
	SocketReadWriteHandler(const SocketReadWriteHandler &) = delete;
	~SocketReadWriteHandler();

	void asyncRead();
	size_t syncWrite(const char *data, size_t size);

	const tcp::socket& getSocket() const;
	tcp::socket& getSocket();

	void registerErrorCallback(ErrorCallbackType &&errorCallback);
	void registerRecieveCallback(ReceivedCallbackType &&recieveCallback);

	bool isVaild() const;
	// socket将会进入“4字节”等待状态，重置缓存数据，并发起一次async read请求
	void resetRead();
	unsigned int lastReceivedLength() const;

protected:
	void closeSocket();

private:
	CLASS_MEMBER_DECLARE(SocketReadWriteHandler);
	friend class ASIOServer;
};

#endif // SOCKETREADWRITEHANDLER_H