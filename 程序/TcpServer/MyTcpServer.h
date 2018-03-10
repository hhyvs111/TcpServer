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

	void setMaxPendingConnections(int numConnections);//重写设置最大连接数函数
signals:
	void connectClient(const int, const QString &, const quint16);//发送 新用户连接信息
	void readData(const int, const QString &, quint16, const QByteArray &);//发送 获得用户发过来的数据
	void sockDisConnect(int, QString, quint16);//断开连接的用户信息
	void sentData(const QByteArray &, const int);//向 scoket发送消息
	void sentDisConnect(int i); //断开 特定连接，并释放资源，-1为断开所有。

public slots:
	void clear(); //断开所有连接，线程计数器请0
protected slots:
	void sockDisConnectSlot(int handle, const QString & ip, quint16 prot, QThread *th);//断开连接的用户信息

protected:
	//
	void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程
private:
	QHash<int, MyTcpSocket *> * tcpClient;//管理连接的map
	int maxConnections;

	
};
//|定义的结束 必须加;  
#endif // MYSERVER  