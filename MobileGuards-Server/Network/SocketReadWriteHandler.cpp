#include "stdafx.h"
#include "SocketReadWriteHandler.h"
#include "TransactionStructure.h"

CLASS_MEMBER_DEFINITION(SocketReadWriteHandler)
{
	quint32 numOfReadBytesNextTime;
	tcp::socket *socket;
	std::vector<char> *buffer = 0;
	SocketReadWriteHandler::ErrorCallbackType *errorCallback = 0;
	SocketReadWriteHandler::ReceivedCallbackType *recieveCallback = 0;
	CLASS_NAME_MEMBER(SocketReadWriteHandler)(io_service &ios)
		: CLASS_MEMBER_CONSTRUCTURE(socket, ios)
		, numOfReadBytesNextTime(4)
	{}
	~CLASS_NAME_MEMBER(SocketReadWriteHandler)()
	{
		delete_ptr(socket, buffer, errorCallback);
	}
	void prepareBuffer(size_t size)
	{
		this->numOfReadBytesNextTime = size;
		this->buffer->resize(size);
	}
};

SocketReadWriteHandler::SocketReadWriteHandler(io_service &ios, Identity id)
	: d(new CLASS_NAME_MEMBER(SocketReadWriteHandler)(ios))
	, id(id)
{
}

SocketReadWriteHandler::~SocketReadWriteHandler()
{
	this->closeSocket();
}

void SocketReadWriteHandler::asyncRead()
{
	if (!d->buffer) {
		d->buffer = new remove_pointer<decltype(d->buffer)>::type(4);
	}
	// 3种情况下会返回：
	// 1.缓冲区满
	// 2.transfer_exactly为真（收到特定数量字节即可返回）
	// 3.有错误发生
	boost::asio::async_read(
		*d->socket
		, buffer(*d->buffer)
		, transfer_exactly(d->numOfReadBytesNextTime)
		, [this](const boost::system::error_code &errorCode, size_t size) {
		do {
			// 出现错误
			if (errorCode != nullptr) {
				if (d->errorCallback && *d->errorCallback) {
					(*d->errorCallback)(*this, errorCode);
				}
				this->resetRead();
				break;
			}
			// 此次读取是读取了出去length字节头之后的数据
			if (d->numOfReadBytesNextTime == 4) {
				IntegerAndChar integer(d->buffer->data());
				if (integer > MAX_IP_PACK_SIZE) {
					this->resetRead();
					break;
				}
				// 除去已经读的4字节的length长度
				size -= 4;
				// 设置buffer缓存空间
				d->prepareBuffer(size);

				this->asyncRead();
			}
			(*d->recieveCallback)(*this, d->buffer);
			// 回调处理完毕后，即可发起下一次async read请求
			// 这里只是给事务中心添加了buffer数据，所以由事务中心发起下一次async read请求
		} while (0);
	});
}

size_t SocketReadWriteHandler::syncWrite(const char *data, size_t size)
{
	boost::system::error_code errorCode;
	size_t n = write(*d->socket, buffer(data, size * sizeof(char)), errorCode);
	if (errorCode != nullptr) {
		if (d->errorCallback && *d->errorCallback) {
			(*d->errorCallback)(*this, errorCode);
		}
	}
	return n;
}

const tcp::socket& SocketReadWriteHandler::getSocket() const
{
	return *d->socket;
}

tcp::socket& SocketReadWriteHandler::getSocket()
{
	return *d->socket;
}

void SocketReadWriteHandler::registerErrorCallback(ErrorCallbackType &&errorCallback)
{
	if (!d->errorCallback) {
		d->errorCallback = new ErrorCallbackType(mv(errorCallback));
	}
}

void SocketReadWriteHandler::registerRecieveCallback(ReceivedCallbackType &&recieveCallback)
{
	if (!d->recieveCallback) {
		d->recieveCallback = new ReceivedCallbackType(mv(recieveCallback));
	}
}

bool SocketReadWriteHandler::isVaild() const
{
	return d->socket->is_open();
}

void SocketReadWriteHandler::resetRead()
{
	d->prepareBuffer(4);
	this->asyncRead();
}

unsigned int SocketReadWriteHandler::lastReceivedLength() const
{
	return d->buffer->size();
}

void SocketReadWriteHandler::closeSocket()
{
	if (d->socket->is_open()) {
		boost::system::error_code errorCode;
		d->socket->shutdown(tcp::socket::shutdown_both, errorCode);
		d->socket->close(errorCode);
	}
}
