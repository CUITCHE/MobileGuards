#ifndef TRANSACTIONSTRUCTURE_H
#define TRANSACTIONSTRUCTURE_H

class SocketReadWriteHandler;
// 数据格式，不采用JSON传输，而用Qt的对象序列化技术，这和我以前写的对象序列化有异曲同工之处。
// 利用Qt的便捷性和框架支持，能更安全，性能也会更好。
// 数据封装格式：
/*+-------------------------------------------------------+
  | data length | protocol | secret-key |       data      |
  +-------------------------------------------------------+
  |   4 Bytes   |  4 Bytes | 8 Bytes    |data length - 16 |
  +-------------------------------------------------------+
  data length(unsigned)：整个数据长度，包含自身，但是总长度65535，超过这个数后，服务端将不会响应。特别低，指未经压缩的data长度
  protocol(unsigned)：操作协议，协议分为客户端协（< 1<<16）议和服务端协议(>= 1<<16)
  secret-key(unsigned)：对整个数据链进行信息摘要得出一个64bit值，作为一次传输的钥匙。客服端和服务端各自算一次，不匹配就丢掉。
  data：会经过压缩
  验证开关，在开发初期将不会考虑。
*/
struct transaction_packet
{
    const std::array<char, MAX_IP_PACK_SIZE> *buffer;
    std::shared_ptr<SocketReadWriteHandler> socket;
};

#endif