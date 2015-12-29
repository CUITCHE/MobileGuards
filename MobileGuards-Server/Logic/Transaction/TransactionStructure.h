#ifndef TRANSACTIONSTRUCTURE_H
#define TRANSACTIONSTRUCTURE_H

class SocketReadWriteHandler;
// ���ݸ�ʽ��������JSON���䣬����Qt�Ķ������л��������������ǰд�Ķ������л�������ͬ��֮����
// ����Qt�ı���ԺͿ��֧�֣��ܸ���ȫ������Ҳ����á�
// ���ݷ�װ��ʽ��
/*+-------------------------------------------------------+
  | data length | protocol | secret-key |       data      |
  +-------------------------------------------------------+
  |   4 Bytes   |  4 Bytes | 8 Bytes    |data length - 16 |
  +-------------------------------------------------------+
  data length(unsigned)���������ݳ��ȣ��������������ܳ���65535������������󣬷���˽�������Ӧ���ر�ͣ�ָδ��ѹ����data����
  protocol(unsigned)������Э�飬Э���Ϊ�ͻ���Э��< 1<<16����ͷ����Э��(>= 1<<16)
  secret-key(unsigned)��������������������ϢժҪ�ó�һ��64bitֵ����Ϊһ�δ����Կ�ס��ͷ��˺ͷ���˸�����һ�Σ���ƥ��Ͷ�����
  data���ᾭ��ѹ��
  ��֤���أ��ڿ������ڽ����ῼ�ǡ�
*/
struct transaction_packet
{
    const std::array<char, MAX_IP_PACK_SIZE> *buffer;
    std::shared_ptr<SocketReadWriteHandler> socket;
};

#endif