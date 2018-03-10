#pragma once
#ifndef MYSERVER  
#define MYSERVER  
#include <QTcpServer>
#include <QHash> 
#include "MyTcpSocket.h"

class MyTcpServer :public QTcpServer {
	Q_OBJECT
public:
	explicit MyTcpServer(QObject *parent = 0, int numConnections = 10000);
	~MyTcpServer();

	void setMaxPendingConnections(int numConnections);//��д�����������������
signals:
	void connectClient(const int, const QString &, const quint16);//���� ���û�������Ϣ
	void readData(const int, const QString &, quint16, const QByteArray &);//���� ����û�������������
	void sockDisConnect(int, QString, quint16);//�Ͽ����ӵ��û���Ϣ
	void sentData(const QByteArray &, const int);//�� scoket������Ϣ
	void sentDisConnect(int i); //�Ͽ� �ض����ӣ����ͷ���Դ��-1Ϊ�Ͽ����С�

public slots:
	void clear(); //�Ͽ��������ӣ��̼߳�������0
protected slots:
	void sockDisConnectSlot(int handle, const QString & ip, quint16 prot, QThread *th);//�Ͽ����ӵ��û���Ϣ

protected:
	//
	void incomingConnection(qintptr socketDescriptor);//�����ѻ�ȡ���߳�
private:
	QHash<int, MyTcpSocket *> * tcpClient;//�������ӵ�map
	int maxConnections;

	
};
//|����Ľ��� �����;  
#endif // MYSERVER  