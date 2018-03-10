#include "MyTcpSocket.h"
#include "MyThread.h"
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
	socketDescriptor1 = socketDescriptor;
	user = new User();  //�������ʼ��Ӧ�ò���
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
	
	auto data = handleData(this->readAll(), this->peerAddress().toString(), this->peerPort());
	qDebug() << data;	//����������Ϣ
	auto checkdata = data;
	if (QString::fromUtf8(checkdata) == "N")
		return;
	this->write(data);
	//    if (!watcher.isRunning())//�ŵ��첽�߳��д���
	//    {
	//        watcher.setFuture(QtConcurrent::run(this,&MyTcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
	//    }
}


//���ش�����
QByteArray MyTcpSocket::handleData(QByteArray dataread, const QString &ip, qint16 port)
{
	QTime time;
	time.start();

	QElapsedTimer tm;   //��ʱ����������ʱ����˶��
	tm.start();
	while (tm.elapsed() < 100)
	{
	}

	//�������Է�����������
	QString data = QString::fromUtf8(dataread);
	
	qDebug() << "user: " << ip <<" port: "<<port <<"sockId: "<< socketDescriptor1<<  " the data from client: " << data; //������͹�������Ϣ
	QStringList list = data.split("#");  //��#�ָ�
	
	//ע���ж�
	if (list[0] == "R")
	{
		QString sendData = list[0];
		qDebug() << "Register";
		//ֱ�ӵ��þ�̬����
		if (MySql::insertDB(list[1]))	//�������ɹ������ʾע��ɹ�
		{
			qDebug() << "Register success!";
			sendData += "#true";
		}
		else
		{
			qDebug() << "Register failed!";
			sendData += "false";
		}
		return sendData.toUtf8();
	}
	//��¼�ж�
	else if (list[0] == "L")
	{
		QString sendData = list[0];
		qDebug() << "Login";
		if (MySql::queryDB(list[1]))	//�������ɹ������ʾע��ɹ�
		{
			qDebug() << "Login success!";
			sendData += "#true";
		}
		else
		{
			qDebug() << "Login failed!";
			sendData += "false";
		}
		return sendData.toUtf8();
	}
	//�û����ظ��Լ��
	else if (list[0] == "RCU")
	{
		QString sendData = list[0];
		qDebug() << "RCU";
		if (MySql::queryDB(list[1]))	//�������ɹ������ʾע��ɹ�
		{
			qDebug() << "username available!";
			sendData += "#true";
		}
		else
		{
			qDebug() << "username exist!";
			sendData += "false";
		}
		return sendData.toUtf8();
	}
	//���ѧ���Ƿ��ظ�
	else if (list[0] == "RCS")
	{
		QString sendData = list[0];
		qDebug() << "RCS";
		if (MySql::queryDB(list[1]))	//�������ɹ������ʾע��ɹ�
		{
			qDebug() << "studentNumber available!";
			sendData += "#true";
		}
		else
		{
			qDebug() << "studentNumber exist!";
			sendData += "false";
		}
		return sendData.toUtf8();
	}
	//��ѯ�û���Ϣ
	else if (list[0] == "QueryUserInfo")
	{
		QString sendName = list[1];
		qDebug() << "UserName:" << sendName;
		//user = new User();  //�����������ʼ���������ڵ�¼��ʱ���ʼ������Ȼ���ֱ���ϴ��ļ��ᴥ����ָ��
		user->queryUserByName(list[1]);
		QString data = user->getUserName() + "#" + user->getUserPassword() + "#" + user->getStudentId()
			+ "#" + user->getPhoneNumber() + "#" + user->getQqNumber() + "#" + user->getMail();
		
		return data.toUtf8();
	}
	//�޸��û���Ϣ
	else if (list[0] == "alterUserInfo")
	{
		
		user->queryUserByName(list[1]); //��ͨ��userName��ѯ

		user->setStudentId(list[2]);
		user->setPhoneNumber(list[3]);
		user->setQqNumber(list[4]);
		user->setMail(list[5]);
		QString data = "updateUserInfo#";
		if (user->updataUser())
		{
			qDebug() << "update userInfo success!";
			data += "true";
		}
		else
		{
			qDebug() << "update userInfo failed!";
			data += "false";
		}
		return data.toUtf8();
	}
	//��ѯ�ļ�,�����ļ��б�ṹ��
	else if (list[0] == "findFile")
	{
		File *file = new File();   //��ʼ��һ��file�ṹ
		QList<FileInfo> fileList = file->queryFile(list[1]);	//list[1]�ǲ�ѯ����
		//QString dataFile;  //�����ݷ���ϵͳ��
		//QString dataLenth = QString::number(fileList.size(), 10);//��Ϣ�ĵ�һλ���ļ�����
		//dataFile += dataLenth + "$";   //�ļ��������ļ���Ϣ�ָ���		
		QByteArray fileSum;

		fileSum.resize(fileList.size() * sizeof(FileInfo));  //��������
		QBuffer buffer(&fileSum);
		buffer.open(QIODevice::WriteOnly);
		QDataStream out(&buffer);
		out << fileList.size();    //�ȷ����ļ�������ȥ

		for (int i = 0;i < fileList.size();i++)
		{
			FileInfo fileInfo;
			fileInfo.fileId = fileList.at(i).fileId;
			fileInfo.fileName = fileList.at(i).fileName;
			fileInfo.fileSize = fileList.at(i).fileSize;
			fileInfo.fileTime = fileList.at(i).fileTime;
			fileInfo.fileType = fileList.at(i).fileType;
			fileInfo.fileUser = fileList.at(i).fileUser;
			fileInfo.userId = fileList.at(i).userId;
			out << fileInfo;
		}
		buffer.close();
		return fileSum ;
	}
	//�����ϴ��ļ���ָ�������ö��߳��ϴ�?��ʵ����һ���̣߳�ֻ��һ���ļ�һ���߳̿��ܰɡ�
	else if (list[0] == "uploadFile")
	{
		qDebug() << "begin to receiveFile";
		
		//socketDescriptor���������MyThread
		MyThread *uploadThread = new MyThread(socketDescriptor1,"upload",this);

		uploadThread->setObjectName("receiveThread");
		//������ɾ��
		connect(uploadThread, SIGNAL(receiveDone()), uploadThread, SLOT(ThreadExit()));
		connect(uploadThread, SIGNAL(finished()), uploadThread, SLOT(deleteLater()));
		uploadThread->start();
		qDebug()<<"the main thread:"<< QThread::currentThreadId();
		return "N";
	}
	//�����ļ�
	else if (list[0] == "downloadFile")
	{
		qDebug() << "begin to receiveFile";
		//MyThread *downloadThread = new MyThread(socketDescriptor1);
	}
	else if (list[0] == "QueryReporter")
	{
		user->queryUserByName(list[1]);
		QString data = user->getTrueName() + "#" + user->getStudentId() + "#" + user->getLevel()
			+ "#" + user->getTeacher() + "#" + QString::number(user->getUserId(), 10);
		return data.toUtf8();
	}
	

	//else
	//return dataread;

	return "";

	//data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
	//return data;
}

void MyTcpSocket::startNext()
{
	this->write(watcher.future().result());
	if (!datas.isEmpty())
	{
		watcher.setFuture(QtConcurrent::run(this, &MyTcpSocket::handleData, datas.dequeue(), this->peerAddress().toString(), this->peerPort()));
	}
}
