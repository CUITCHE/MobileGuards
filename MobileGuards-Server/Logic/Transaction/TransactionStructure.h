#ifndef TRANSACTIONSTRUCTURE_H
#define TRANSACTIONSTRUCTURE_H

#include "SocketReadWriteHandler.h"

/* 
 ���ݸ�ʽ��������JSON���䣬����Qt�Ķ������л��������������ǰд�Ķ������л�������ͬ��֮����
 ����Qt�ı���ԺͿ��֧�֣��ܸ���ȫ������Ҳ����á�
 ���ݷ�װ��ʽ��

+-------------------------------------------------------+
| data length | protocol | secret-key |       data      |
+-------------------------------------------------------+
|   4 Bytes   |  4 Bytes | 8 Bytes    |data length - 16 |
+-------------------------------------------------------+
data length(unsigned)���������ݳ��ȣ��������������ܳ���65535������������󣬷���˽�������Ӧ���ر�أ�ָδ��ѹ����data����
protocol(unsigned)������Э�飬Э���Ϊ�ͻ���Э��< 1<<16����ͷ����Э��(> 1<<16)
secret-key(unsigned)��������������������ϢժҪ�ó�һ��64bitֵ����Ϊһ�δ����Կ�ס��ͷ��˺ͷ���˸�����һ�Σ���ƥ��Ͷ�����
data���ᾭ��ѹ��
��֤���أ��ڿ������ڽ����ῼ�ǡ�
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