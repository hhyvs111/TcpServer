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
	QByteArray handleData(QByteArray data, const QString & ip, qint16 port);//�����������ݵĺ���

signals:
	//void readData(const int,const QString &,const quint16,const QByteArray &);
	void sockDisConnect(const int, const QString &, const quint16, QThread *);//NOTE:�Ͽ����ӵ��û���Ϣ�����źű��뷢�����̹߳���������źż�����
public slots:
	void sentData(const QByteArray &, const int);//�����źŵĲ�
	void disConTcp(int i);

	//void receiveFile();  //�����ļ�
	////void ThreadExit(); 

	////send
	//bool openFile(QString);  //���ļ�
	//void sendFile();   //�����ļ�
	//void goOnSend(qint64);		//��������

protected slots:
	void readData();//��������
	void startNext();//������һ��

protected:
	QFutureWatcher<QByteArray> watcher;
	QQueue<QByteArray> datas;  //���ݼ�
private:
	qintptr socketID;
	QMetaObject::Connection dis;
	qintptr socketDescriptor1;
	//�û���
	User *user;


	////�����ļ�
	////QTcpSocket *tcpSocket;
	//QFile *newFile;
	//QByteArray inBlock;   // QByteArray���Դ洢raw bytes�ʹ�ͳ��8-bits���ַ�����������'\0'��β�ģ�ʹ�ñ�char*������
	//QString fileName;
	//qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	//qint64 byteReceived;  //�Ѿ����յĴ�С  
	//int receiveTime;	//���ܵĴ���

	//QString globalUserName;
	////User *user;   //�����������ʼ������Ϊʵ���ϻ��������߳������ģ����Զ���̹߳�����ʱ����ܻ���ʳ�ͻ��

	////�����ļ�
	//QFile *localFile;
	//QString sendFileName;  //�ļ���  
	//QByteArray outBlock;  //�ִδ�  
	//qint64 loadSize;  //ÿ�η������ݵĴ�С  
	//qint64 byteToWrite;  //ʣ�����ݴ�С  
	//qint64 StotalSize;  //�ļ��ܴ�С 
	//int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ����� 
	//QString currentFileName;
	//
};

#endif // TCPSOCKET_H

