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
    // ��ȡbuffer������������ݲ�����
    // ����packet�࣬�����ݷ����е���Ӧ��������ȥ
}