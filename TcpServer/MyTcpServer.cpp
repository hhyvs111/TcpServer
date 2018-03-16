#include "MyTcpServer.h"
#include "Database.h"
#include "MyThreadHandle.h"

MyTcpServer::MyTcpServer(QObject *parent, int numConnections) :
	QTcpServer(parent)
{
	//�ͻ��ˣ��������ӣ�һ���ͻ���һ��socket
	tcpClient = new  QHash<int, MyTcpSocket *>;
	MySql *mysql = new MySql();  //�������ݿ�
	mysql->connDB();
	//���ӵ������
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
	this->QTcpServer::setMaxPendingConnections(numConnections);//����Qtcpsocket�����������������������Ҫ��ʹmaxPendingConnections()��Ȼ��Ч
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

//������д�źŲۣ�������QTCPSERVER�Դ���
void MyTcpServer::incomingConnection(qintptr socketDescriptor) //���̱߳����ڴ˺����ﲶ��������
{
	//�����ǰ�����������ֵ
	if (tcpClient->size() > maxPendingConnections())//�̳���д�˺�����QMyTcpServerĬ�ϵ��ж����������ʧЧ���Լ�ʵ��
	{
		QTcpSocket tcp;
		tcp.setSocketDescriptor(socketDescriptor);
		tcp.disconnectFromHost();
		return;
	}

	
	auto th = ThreadHandle::getClass().getThread();	//����ΪʲôҪ�õ��̣߳�
	auto tcpTemp = new MyTcpSocket(socketDescriptor);
	QString ip = tcpTemp->peerAddress().toString();
	qint16 port = tcpTemp->peerPort();

	connect(tcpTemp, &MyTcpSocket::sockDisConnect, this, &MyTcpServer::sockDisConnectSlot);//NOTE:�Ͽ����ӵĴ������б��Ƴ������ͷŶϿ���Tcpsocket���˲۱���ʵ�֣��̹߳������Ҳ�ǿ�����
	connect(this, &MyTcpServer::sentDisConnect, tcpTemp, &MyTcpSocket::disConTcp);//�Ͽ��ź�

	tcpTemp->moveToThread(th);//��tcp���ƶ����µ��̣߳����̹߳������л�ȡ
	tcpClient->insert(socketDescriptor, tcpTemp);//���뵽������Ϣ��
	//qDebug() << "the SocketDescriptor:"<<setSocketDescriptor;
	qDebug() << "the ip:" << ip << "and the port:" << port;
	emit connectClient(socketDescriptor, ip, port);
}

//�Ͽ�����
void MyTcpServer::sockDisConnectSlot(int handle, const QString & ip, quint16 prot, QThread * th)
{
	tcpClient->remove(handle);//���ӹ������Ƴ��Ͽ����ӵ�socket
	ThreadHandle::getClass().removeThread(th); //�����̹߳������Ǹ��߳�������ӶϿ���
	emit sockDisConnect(handle, ip, prot);
}


void MyTcpServer::clear()
{
	emit this->sentDisConnect(-1);
	ThreadHandle::getClass().clear();
	tcpClient->clear();
}
