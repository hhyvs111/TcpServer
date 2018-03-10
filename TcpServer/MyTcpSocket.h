#pragma once
#ifndef MYTCPSOCKET  
#define MYTCPSOCKET  
#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
#include "User.h"
#include "File.h"


class MyTcpSocket : public QTcpSocket
{
	Q_OBJECT
public:
	explicit MyTcpSocket(qintptr socketDescriptor, QObject *parent = 0);
	~MyTcpSocket();
	QByteArray handleData(QByteArray data, const QString & ip, qint16 port);//用来处理数据的函数

signals:
	//void readData(const int,const QString &,const quint16,const QByteArray &);
	void sockDisConnect(const int, const QString &, const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
	void sentData(const QByteArray &, const int);//发送信号的槽
	void disConTcp(int i);

protected slots:
	void readData();//接收数据
	void startNext();//处理下一个

protected:
	QFutureWatcher<QByteArray> watcher;
	QQueue<QByteArray> datas;  //数据集
private:
	qintptr socketID;
	QMetaObject::Connection dis;
	qintptr socketDescriptor1;
	//用户类
	User *user;
	
	
};

#endif // TCPSOCKET_H

