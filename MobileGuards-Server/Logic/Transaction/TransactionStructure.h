#ifndef TRANSACTIONSTRUCTURE_H
#define TRANSACTIONSTRUCTURE_H

#include "SocketReadWriteHandler.h"

/* 
 数据格式，不采用JSON传输，而用Qt的对象序列化技术，这和我以前写的对象序列化有异曲同工之处。
 利用Qt的便捷性和框架支持，能更安全，性能也会更好。
 数据封装格式：

+-------------------------------------------------------+
| data length | protocol | secret-key |       data      |
+-------------------------------------------------------+
|   4 Bytes   |  4 Bytes | 8 Bytes    |data length - 16 |
+-------------------------------------------------------+
data length(unsigned)：整个数据长度，包含自身，但是总长度65535，超过这个数后，服务端将不会响应。特别地，指未经压缩的data长度
protocol(unsigned)：操作协议，协议分为客户端协（< 1<<16）议和服务端协议(> 1<<16)
secret-key(unsigned)：对整个数据链进行信息摘要得出一个64bit值，作为一次传输的钥匙。客服端和服务端各自算一次，不匹配就丢掉。
data：会经过压缩
验证开关，在开发初期将不会考虑。
*/
struct transaction_packet
{
	const std::vector<char> *buffer;
	std::shared_ptr<SocketReadWriteHandler> socket;
};

typedef enum class CommunicationProtocolsTag : unsigned int
{
	ProtocolNotExist = 0,
	ClientProtocolBegin,
	ClientTest,
	ClientProtocolEnd,
	ServerProtocolBegin = 1 << 16,
	ServerProtocolEnd
} CommunicationProtocols;

#ifndef PROTOCOLTRANSFER
#define PROTOCOLTRANSFER(protocol) #protocol
// transfer transaction_packet_protocol to string
static const char *protocolTransfer(CommunicationProtocols protocol)
{
	const char *str = 0;
	switch (protocol)
	{
	default:
		break;
	}
	return str;
}

template<typename _Ty>
inline static CommunicationProtocols dynamic_protocol_cast(_Ty protocol)
{
	static_assert(std::is_integral<_Ty>::value, "only integer is legal!");
	CommunicationProtocols cps = protocol_cast(protocol);
	if (!(cps != CommunicationProtocols::ProtocolNotExist &&
		((cps > CommunicationProtocols::ClientProtocolBegin && cps < CommunicationProtocols::ClientProtocolEnd) ||
		(cps > CommunicationProtocols::ServerProtocolBegin && cps < CommunicationProtocols::ServerProtocolEnd))
		)) {
		cps = CommunicationProtocols::ProtocolNotExist;
	}
	return cps;
}

template<typename _Ty>
inline static CommunicationProtocols protocol_cast(_Ty protocol)
{
	static_assert(std::is_integral<_Ty>::value, "only integer is legal!");
	return static_cast<CommunicationProtocols>(protocol);
}
#endif // !PROTOCOLTRANSFER


// return true if Little-Endian.
static bool EndianTest()
{
	static char t[4] = { 1, 0, 0, 0 };
	return *((int *)t) == 1;
}

union IntegerAndChar
{
	quint32 d;
	char var[4];
	IntegerAndChar(const char *bigEndian)
	{
		var[0] = bigEndian[3];
		var[1] = bigEndian[2];
		var[2] = bigEndian[1];
		var[3] = bigEndian[0];
	}
	IntegerAndChar(quint32 var)
		:d(var)
	{}
	operator quint32&() 
	{
		return d;
	}
	operator const char*() const
	{
		return var;
	}
};
#endif