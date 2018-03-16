#include "MyTcpServer.h"
#include "Database.h"
#include "MyThreadHandle.h"

MyTcpServer::MyTcpServer(QObject *parent, int numConnections) :
	QTcpServer(parent)
{
	//客户端，管理连接，一个客户端一个socket
	tcpClient = new  QHash<int, MyTcpSocket *>;
	MySql *mysql = new MySql();  //开启数据库
	mysql->connDB();
	//连接的最大数
	setMaxPendingConnections(numConnections);
	init();
}

MyTcpServer::~MyTcpServer()
{
	emit this->sentDisConnect(-1);
	delete tcpClient;
}

void MyTcpServer::setMaxPendingConnections(int numConnections)
{
	this->QTcpServer::setMaxPendingConnections(numConnections);//调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
	this->maxConnections = numConnections;
}
void MyTcpServer::init()
{
	if (!this->listen(QHostAddress::Any, 3389))
	{
		qDebug() << this->errorString();
		//close();
		return;
	}
	else
		qDebug() << "start listenning!";
}

//不用在写信号槽，好像是QTCPSERVER自带的
void MyTcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
	//如果当前数量大于最大值
	if (tcpClient->size() > maxPendingConnections())//继承重写此函数后，QMyTcpServer默认的判断最大连接数失效，自己实现
	{
		QTcpSocket tcp;
		tcp.setSocketDescriptor(socketDescriptor);
		tcp.disconnectFromHost();
		return;
	}

	
	auto th = ThreadHandle::getClass().getThread();	//这里为什么要得到线程？
	auto tcpTemp = new MyTcpSocket(socketDescriptor);
	QString ip = tcpTemp->peerAddress().toString();
	qint16 port = tcpTemp->peerPort();

	connect(tcpTemp, &MyTcpSocket::sockDisConnect, this, &MyTcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是靠的他
	connect(this, &MyTcpServer::sentDisConnect, tcpTemp, &MyTcpSocket::disConTcp);//断开信号

	tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
	tcpClient->insert(socketDescriptor, tcpTemp);//插入到连接信息中
	//qDebug() << "the SocketDescriptor:"<<setSocketDescriptor;
	qDebug() << "the ip:" << ip << "and the port:" << port;
	emit connectClient(socketDescriptor, ip, port);
}

//断开连接
void MyTcpServer::sockDisConnectSlot(int handle, const QString & ip, quint16 prot, QThread * th)
{
	tcpClient->remove(handle);//连接管理中移除断开连接的socket
	ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
	emit sockDisConnect(handle, ip, prot);
}


void MyTcpServer::clear()
{
	emit this->sentDisConnect(-1);
	ThreadHandle::getClass().clear();
	tcpClient->clear();
}
