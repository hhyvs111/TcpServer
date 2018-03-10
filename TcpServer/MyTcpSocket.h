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
	QByteArray handleData(QByteArray data, const QString & ip, qint16 port);//�����������ݵĺ���

signals:
	//void readData(const int,const QString &,const quint16,const QByteArray &);
	void sockDisConnect(const int, const QString &, const quint16, QThread *);//NOTE:�Ͽ����ӵ��û���Ϣ�����źű��뷢�����̹߳���������źż�����
public slots:
	void sentData(const QByteArray &, const int);//�����źŵĲ�
	void disConTcp(int i);

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
	
	
};

#endif // TCPSOCKET_H

