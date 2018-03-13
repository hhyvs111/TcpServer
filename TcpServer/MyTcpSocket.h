#pragma once
#ifndef MYTCPSOCKET  
#define MYTCPSOCKET  
#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
#include <QFile>
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

	//void receiveFile();  //接收文件
	////void ThreadExit(); 

	////send
	//bool openFile(QString);  //打开文件
	//void sendFile();   //发送文件
	//void goOnSend(qint64);		//继续发送

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


	////接收文件
	////QTcpSocket *tcpSocket;
	//QFile *newFile;
	//QByteArray inBlock;   // QByteArray可以存储raw bytes和传统的8-bits的字符串，都是以'\0'结尾的，使用比char*更方便
	//QString fileName;
	//qint64 RtotalSize;  //总共需要发送的文件大小（文件内容&文件名信息）  
	//qint64 byteReceived;  //已经接收的大小  
	//int receiveTime;	//接受的次数

	//QString globalUserName;
	////User *user;   //不能在这里初始化，因为实际上还是在主线程声明的，所以多个线程工作的时候可能会访问冲突！

	////发送文件
	//QFile *localFile;
	//QString sendFileName;  //文件名  
	//QByteArray outBlock;  //分次传  
	//qint64 loadSize;  //每次发送数据的大小  
	//qint64 byteToWrite;  //剩余数据大小  
	//qint64 StotalSize;  //文件总大小 
	//int sendTimes;  //用来标记是否为第一次发送，第一次以后连接信号触发，后面的则手动调用 
	//QString currentFileName;
	//
};

#endif // TCPSOCKET_H

