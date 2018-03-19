#ifndef MYTHREAD_H_
#define MYTHREAD_H_
#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include"File.h"
#include "User.h"
 
 //继承QThread的TCP传输线程
 //主要是完成run()虚函数的定义
 //还有一些辅助变量的声明
 
class QFile;
class QTcpSocket;
 
class MyThread : public QThread
{
 Q_OBJECT
 public:
	MyThread(qintptr socketDescriptor,QString, QObject *parent = 0);
	MyThread(qintptr socketDescriptor, QString, QString,QString, QObject *parent = 0);
	MyThread(qintptr socketDescriptor, QString, QString,QString, QString, QObject *parent = 0);
	~MyThread();
 
	void run();
 
signals:
	void error(QTcpSocket::SocketError socketError);
	void bytesArrived(qint64,qint32,int);
	void receiveDone();
 
public slots:
	void receiveFile();
	void ThreadExit();

	//send
	bool openFile(QString);
	void sendFile();
	void goOnSend();
	void insertRecord(qint64, qint64);
	//void displayError(QAbstractSocket::SocketError);
 
private:
	int socketDescriptor;
	QString type;  //线程类型，是下载还是上传
	//qint64 bytesReceived; //收到的总字节
	//qint64 byteToRead; //准备读取的字节
	//qint32 TotalBytes; //总共传输的字节
	
	//QHostAddress fileName; //文件名
	//QFile *localFile;
	//QByteArray inBlock; //读取缓存

	//接收文件
	QTcpSocket *tcpSocket;
	QFile *newFile;
	QByteArray inBlock;   // QByteArray可以存储raw bytes和传统的8-bits的字符串，都是以'\0'结尾的，使用比char*更方便
	QString fileName;
	qint64 RtotalSize;  //总共需要发送的文件大小（文件内容&文件名信息）  
	qint64 byteReceived;  //已经接收的大小  
	int receiveTime;	//接受的次数

	QString globalUserName;
	//User *user;   //不能在这里初始化，因为实际上还是在主线程声明的，所以多个线程工作的时候可能会访问冲突！

	//发送文件
	QFile *localFile;
	QString sendFileName;  //文件名  
	QByteArray outBlock;  //分次传  
	qint64 loadSize;  //每次发送数据的大小  
	qint64 byteToWrite;  //剩余数据大小  
	qint64 StotalSize;  //文件总大小 
	int sendTimes;  //用来标记是否为第一次发送，第一次以后连接信号触发，后面的则手动调用 
	QString currentFileName;

	qint64 receiveStatus ;
	qint64 sumBlock;
	qint64 breakPoint;

	qint64 fileId;

 };
 
#endif // MYTHREAD_H_ 