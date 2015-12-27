#include "stdafx.h"
#include "SocketReadWriteHandler.h"

CLASS_MEMBER_DEFINITION(SocketReadWriteHandler)
{
	tcp::socket *socket;
	std::array<char, MAX_IP_PACK_SIZE> *buffer = 0;
	SocketReadWriteHandler::ErrorCallbackType *errorCallback = 0;
	SocketReadWriteHandler::ReceivedCallbackType *recieveCallback = 0;

	CLASS_NAME_MEMBER(SocketReadWriteHandler)(io_service &ios)
		: socket(new tcp::socket(ios))
	{}
	~CLASS_NAME_MEMBER(SocketReadWriteHandler)()
	{
		delete_ptr(socket, buffer, errorCallback);
	}
};

SocketReadWriteHandler::SocketReadWriteHandler(io_service &ios, Identity id, QObject *parent)
	: QObject(parent)
	, d(new CLASS_NAME_MEMBER(SocketReadWriteHandler)(ios))
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
		d->buffer = new remove_pointer<decltype(d->buffer)>::type;
	}
	// 3������»᷵�أ�
	// 1.��������
	// 2.transfer_at_leastΪ�棨�յ��ض������ֽڼ��ɷ��أ�
	// 3.�д�����
	boost::asio::async_read(
		*d->socket
		, buffer(*d->buffer)
		, transfer_at_least(1)
		, [this](const boost::system::error_code &errorCode, size_t size){
		if (errorCode != nullptr) {
			if (d->errorCallback && *d->errorCallback) {
				(*d->errorCallback)(*this, errorCode);
			}
		}
		(*d->recieveCallback)(*this, *d->buffer);
		// �ص�������Ϻ󣬼��ɷ�����һ��async read����
		this->asyncRead();
	});
}

size_t SocketReadWriteHandler::syncWrite(const char *data, size_t size)
{
	boost::system::error_code errorCode;
	size_t n = write(*d->socket, buffer(data, size), errorCode);
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

void SocketReadWriteHandler::closeSocket()
{
	if (d->socket->is_open()) {
		boost::system::error_code errorCode;
		d->socket->shutdown(tcp::socket::shutdown_both, errorCode);
		d->socket->close(errorCode);
	}
}
