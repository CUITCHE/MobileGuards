#ifndef DISPATCHTRANSACTIONCENTER_H
#define DISPATCHTRANSACTIONCENTER_H

#include <QObject>

struct transaction_packet;

class DispatchTransactionCenter : public QObject
{
    Q_OBJECT
    
public:
    DispatchTransactionCenter(QObject *parent = 0);
    ~DispatchTransactionCenter;
    
    void putTransaction(transaction_packet &transaction);
private:
    CLASS_MEMBER_DECLARE(DispatchTransactionCenter);    
};

#endif