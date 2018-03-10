#include "MyTcpSocket.h"
#include "MyThread.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

MyTcpSocket::MyTcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
	QTcpSocket(parent), socketID(socketDescriptor)
{
	this->setSocketDescriptor(socketDescriptor);
	connect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::readData);
	dis = connect(this, &MyTcpSocket::disconnected,
		[&]() {
		qDebug() << "disconnect";
		emit sockDisConnect(socketID, this->peerAddress().toString(), this->peerPort(), QThread::currentThread());//发送断开连接的用户信息
		this->deleteLater();
	});
	connect(&watcher, &QFutureWatcher<QByteArray>::finished, this, &MyTcpSocket::startNext);
	connect(&watcher, &QFutureWatcher<QByteArray>::canceled, this, &MyTcpSocket::startNext);
	qDebug() << "new connect";
	socketDescriptor1 = socketDescriptor;
	user = new User();  //在这里初始化应该不错
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
	else if (i == -1) //-1为全部断开
	{
		disconnect(dis); //先断开连接的信号槽，防止二次析构
		this->disconnectFromHost();
		this->deleteLater();
	}
}

//处理
void MyTcpSocket::readData()
{
	
	auto data = handleData(this->readAll(), this->peerAddress().toString(), this->peerPort());
	qDebug() << data;	//处理过后的信息
	auto checkdata = data;
	if (QString::fromUtf8(checkdata) == "N")
		return;
	this->write(data);
	//    if (!watcher.isRunning())//放到异步线程中处理。
	//    {
	//        watcher.setFuture(QtConcurrent::run(this,&MyTcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
	//    }
}


//返回处理结果
QByteArray MyTcpSocket::handleData(QByteArray dataread, const QString &ip, qint16 port)
{
	QTime time;
	time.start();

	QElapsedTimer tm;   //耗时操作，计算时间过了多久
	tm.start();
	while (tm.elapsed() < 100)
	{
	}

	//解析来自服务器的数据
	QString data = QString::fromUtf8(dataread);
	
	qDebug() << "user: " << ip <<" port: "<<port <<"sockId: "<< socketDescriptor1<<  " the data from client: " << data; //输出发送过来的信息
	QStringList list = data.split("#");  //用#分割
	
	//注册判断
	if (list[0] == "R")
	{
		QString sendData = list[0];
		qDebug() << "Register";
		//直接调用静态函数
		if (MySql::insertDB(list[1]))	//如果插入成功，则表示注册成功
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
	//登录判断
	else if (list[0] == "L")
	{
		QString sendData = list[0];
		qDebug() << "Login";
		if (MySql::queryDB(list[1]))	//如果插入成功，则表示注册成功
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
	//用户名重复性检查
	else if (list[0] == "RCU")
	{
		QString sendData = list[0];
		qDebug() << "RCU";
		if (MySql::queryDB(list[1]))	//如果插入成功，则表示注册成功
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
	//检查学号是否重复
	else if (list[0] == "RCS")
	{
		QString sendData = list[0];
		qDebug() << "RCS";
		if (MySql::queryDB(list[1]))	//如果插入成功，则表示注册成功
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
	//查询用户信息
	else if (list[0] == "QueryUserInfo")
	{
		QString sendName = list[1];
		qDebug() << "UserName:" << sendName;
		//user = new User();  //不能在这里初始化啊，得在登录的时候初始化，不然如果直接上传文件会触发空指针
		user->queryUserByName(list[1]);
		QString data = user->getUserName() + "#" + user->getUserPassword() + "#" + user->getStudentId()
			+ "#" + user->getPhoneNumber() + "#" + user->getQqNumber() + "#" + user->getMail();
		
		return data.toUtf8();
	}
	//修改用户信息
	else if (list[0] == "alterUserInfo")
	{
		
		user->queryUserByName(list[1]); //先通过userName查询

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
	//查询文件,发送文件列表结构体
	else if (list[0] == "findFile")
	{
		File *file = new File();   //初始化一个file结构
		QList<FileInfo> fileList = file->queryFile(list[1]);	//list[1]是查询条件
		//QString dataFile;  //总数据发到系统里
		//QString dataLenth = QString::number(fileList.size(), 10);//信息的第一位是文件个数
		//dataFile += dataLenth + "$";   //文件个数与文件信息分隔符		
		QByteArray fileSum;

		fileSum.resize(fileList.size() * sizeof(FileInfo));  //设置容量
		QBuffer buffer(&fileSum);
		buffer.open(QIODevice::WriteOnly);
		QDataStream out(&buffer);
		out << fileList.size();    //先发送文件个数过去

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
	//发来上传文件的指令，这里得用多线程上传?其实还是一个线程，只是一个文件一个线程可能吧。
	else if (list[0] == "uploadFile")
	{
		qDebug() << "begin to receiveFile";
		
		//socketDescriptor将这个传入MyThread
		MyThread *uploadThread = new MyThread(socketDescriptor1,"upload",this);

		uploadThread->setObjectName("receiveThread");
		//结束后删除
		connect(uploadThread, SIGNAL(receiveDone()), uploadThread, SLOT(ThreadExit()));
		connect(uploadThread, SIGNAL(finished()), uploadThread, SLOT(deleteLater()));
		uploadThread->start();
		qDebug()<<"the main thread:"<< QThread::currentThreadId();
		return "N";
	}
	//下载文件
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
