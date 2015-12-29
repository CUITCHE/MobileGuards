#include "stdafx.h"
#include "DispatchTransactionCenter.h"
#include "TransactionStructure.h"
#include "TransactionCenter.h"

CLASS_MEMBER_DEFINITION(DispatchTransactionCenter)
{
    TransactionCenter *transactionCenter;
};

DispatchTransactionCenter::DispatchTransactionCenter(QObject *parent)
    :QObject(parent)
    ,d(new CLASS_NAME_MEMBER(DispatchTransactionCenter))
{
}

DispatchTransactionCenter::~DispatchTransactionCenter()
{
    ;
}

void DispatchTransactionCenter::putTransaction(transaction_packet &transaction)
{
    // 读取buffer里面的数据内容并分析
    // 生成packet类，将数据反序列到响应的类里面去
}