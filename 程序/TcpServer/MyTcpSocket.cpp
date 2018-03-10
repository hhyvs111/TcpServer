#include "MyTcpSocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

MyTcpSocket::MyTcpSocket(qintptr socketDescriptor, QObject *parent) : //���캯�������߳�ִ�У�lambda�����߳�
	QTcpSocket(parent), socketID(socketDescriptor)
{
	this->setSocketDescriptor(socketDescriptor);
	connect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::readData);
	dis = connect(this, &MyTcpSocket::disconnected,
		[&]() {
		qDebug() << "disconnect";
		emit sockDisConnect(socketID, this->peerAddress().toString(), this->peerPort(), QThread::currentThread());//���ͶϿ����ӵ��û���Ϣ
		this->deleteLater();
	});
	connect(&watcher, &QFutureWatcher<QByteArray>::finished, this, &MyTcpSocket::startNext);
	connect(&watcher, &QFutureWatcher<QByteArray>::canceled, this, &MyTcpSocket::startNext);
	qDebug() << "new connect";
}

MyTcpSocket::~MyTcpSocket()
{
}


void MyTcpSocket::sentData(const QByteArray &data, const int id)
{
	if (id == socketID)
	{
		write(data);
	}
}

void MyTcpSocket::disConTcp(int i)
{
	if (i == socketID)
	{
		this->disconnectFromHost();
	}
	else if (i == -1) //-1Ϊȫ���Ͽ�
	{
		disconnect(dis); //�ȶϿ����ӵ��źŲۣ���ֹ��������
		this->disconnectFromHost();
		this->deleteLater();
	}
}

//����
void MyTcpSocket::readData()
{
	//    datas.append(this->readAll());
	auto data = handleData(this->readAll(), this->peerAddress().toString(), this->peerPort());
	qDebug() << data;
	this->write(data);
	//    if (!watcher.isRunning())//�ŵ��첽�߳��д���
	//    {
	//        watcher.setFuture(QtConcurrent::run(this,&MyTcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
	//    }
}

QByteArray MyTcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
	QTime time;
	time.start();

	QElapsedTimer tm;
	tm.start();
	while (tm.elapsed() < 100)
	{
	}
	data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
	return data;
}

void MyTcpSocket::startNext()
{
	this->write(watcher.future().result());
	if (!datas.isEmpty())
	{
		watcher.setFuture(QtConcurrent::run(this, &MyTcpSocket::handleData, datas.dequeue(), this->peerAddress().toString(), this->peerPort()));
	}
}
