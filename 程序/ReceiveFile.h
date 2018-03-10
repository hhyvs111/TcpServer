//#pragma once
//#ifndef RECEIVEFILE_H_
//#define RECEIVEFILE_H_
//#include "Global.h"
//#include <QtNetwork>
//#include <QTimer>
//#include <QFile>  
//#include <QString> 
////#include <QFileDialog>
//#include <QIODevice>
//#include <QByteArray>
//#include <QDataStream>
//#include "File.h"
//#include "User.h"
//extern TcpServer *tcp;   //定义一个全局变量TCP
//class ReceiveFile : public QObject
//{
//	Q_OBJECT
//public:
//	void init();
//
//	public slots:
//	void receive();
//
//private:
//	//接收文件
//	QFile *newFile;
//	QByteArray inBlock;   // QByteArray可以存储raw bytes和传统的8-bits的字符串，都是以'\0'结尾的，使用比char*更方便
//	QString fileName;
//	qint64 RtotalSize;  //总共需要发送的文件大小（文件内容&文件名信息）  
//	qint64 byteReceived;  //已经接收的大小  
//	int receiveTime;	//接受的次数
//
//
//		//friend class TcpServer;  //声明友元？
//	User *user;
//};
//#endif // !RECEIVEFILE_H_
//
