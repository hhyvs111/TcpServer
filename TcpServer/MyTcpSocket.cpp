#include "MyTcpSocket.h"
#include "MyThread.h"
#include "Report.h"
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
	socketDescriptor1 = socketDescriptor;
	qDebug() << "new connect"<< "socketId:" << socketDescriptor1 << "threadId: " << QThread::currentThreadId();
	//好像只要是新来的连接就是一个新的线程。
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
	qDebug() << dataread;
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
		
		connect(this, SIGNAL(readyRead()),
			this, SLOT(receiveFile()), Qt::BlockingQueuedConnection);

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
		qDebug() << "begin to downloadFile";
		MyThread *downloadThread = new MyThread(socketDescriptor1,"download",list[1],this);
		connect(downloadThread, SIGNAL(finished()), downloadThread, SLOT(deleteLater()));
		connect(downloadThread, SIGNAL(receiveDone()), downloadThread, SLOT(ThreadExit()));
		downloadThread->start();
		//if (openFile(list[1]))
		//{

		//	sendFile();
		//}
		//connect(downloadThread, SIGNAL(receiveDone()), downloadThread, SLOT(ThreadExit()));
	}
	else if (list[0] == "QueryReporter")
	{
		user->queryUserByName(list[1]);
		QString data = user->getTrueName() + "#" + user->getStudentId() + "#" + user->getLevel()
			+ "#" + user->getTeacher() + "#" + QString::number(user->getUserId(), 10);
		return data.toUtf8();
	}
	else if (list[0] == "sendReport")
	{
		//QByteArray dataread = list[1].toUtf8();  //先转换为字节流
		//qDebug() << dataread;
		////读二进制，重写in，输入结构体
		//QBuffer buf(&dataread);
		//buf.open(QIODevice::ReadOnly);
		//QDataStream in(&buf);

		//ReportInfo reportInfo;
		////二进制流依次写入
		//in >> reportInfo;

		Report *report = new Report();
		//先查询一下该报告是否存在,一周只能发一个周报
		QString Repeat = "1#reportRepeat#";
		ReportInfo ri;
		ri.reportUserId = list[1].toInt();
		ri.reportTerm = list[2];
		ri.reportWeek = list[3];
		ri.reportMain = list[4];
		ri.reportQuestionForSolve = list[5];
		ri.reportQuestionFromLastWeek = list[6];
		ri.reportQuestionHasSolved = list[7];
		ri.reportYourGet = list[8];
		//1#reportRepeat#reportUserId#reportTerm#reportWeek
		Repeat += list[1] + "#" + list[2] + "#" + list[3];
		QList<ReportInfo> ReportList = report->queryReport(Repeat);

		//如果大于0则该学期该周已经存在周报，不可重复提交
		if (ReportList.size() > 0)
		{
			QString data = "ReportEdit#repeatTrue";
			return data.toUtf8();
		}

		QDateTime dt;
		QTime time;
		QDate date;
		dt.setTime(time.currentTime());
		dt.setDate(date.currentDate());
		QString currentDate = dt.toString("yyyy-MM-dd hh:mm");
		ri.reportTime = currentDate;

		//设置report类的info
		report->setReportInfo(ri);

		//如果插入成功
		if (report->insertReport())
		{
			qDebug() << "insert report success!";
			QString data = "ReportEdit#true";
			return data.toUtf8();
		}
		else
		{
			qDebug() << "insert error!";
			QString data = "ReportEdit#false";
			return data.toUtf8();
		}
		return "N";
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

//void MyTcpSocket::receiveFile()
//{
//	if (byteReceived == 0)  //才刚开始接收数据，此数据为文件信息  
//	{
//		qDebug() << "receive sockID:" << socketDescriptor1
//			<< "start new Thread id is" << QThread::currentThreadId();
//		qDebug() << "receive the file of head";
//		//ui->receivedProgressBar->setValue(0);
//		QDataStream in(this);    //从里面读
//									  //QString globalUserName;
//		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //读的是二进制字节？
//
//																		  //user = new User();
//																		  /*user->queryUserByName(globalUserName);
//																		  qDebug() << "userName is :" << user->getUserName();*/
//
//		QByteArray change = fileName.toUtf8();
//		fileName = "files/" + QString::fromUtf8(change);
//
//		qDebug() << "the file of head: " << fileName;
//		qDebug() << "totalSize: " << RtotalSize;
//		qDebug() << " first byteReceived: " << byteReceived;
//
//		//设计一个文件读写不重复的功能
//		int i = 0;
//		QString suffix = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1); //获取文件后缀;
//		QString fullName = fileName.left(fileName.size() - (fileName.size() - fileName.lastIndexOf('.'))); //获取文件名
//		while (1)
//		{
//			QFile *existFile = new QFile(fileName);  //就是判断是否重复
//			if (existFile->exists())   //如果文件存在
//			{
//				i++;
//				fileName = fullName + "(" + QString::number(i, 10) + ")." + suffix;
//				qDebug() << "the fileName :" << fileName;
//			}
//			else
//				break;
//		}
//
//		newFile = new QFile(fileName);
//		newFile->open(QFile::WriteOnly);
//	}
//	else  //正式读取文件内容    
//	{
//		receiveTime++;
//		qDebug() << "sockId:" << socketDescriptor1 << "ThreadId: " << QThread::currentThreadId() << fileName << " the " << receiveTime << "recv " /*<< "the inBlock size is " << inBlock.size()*/
//			<< "current receive the byte is :" << byteReceived << " and the total " << RtotalSize;
//
//		inBlock = this->readAll();
//		byteReceived += inBlock.size();
//		newFile->write(inBlock);
//		newFile->flush();   //直接进入接收文件
//	}
//
//	if (byteReceived == RtotalSize)
//	{
//		//qDebug() << "user " << clientip << "receive is done";
//		//接收完成向数据库插入文件信息
//		QDateTime dt;
//		QTime time;
//		QDate date;
//		dt.setTime(time.currentTime());
//		dt.setDate(date.currentDate());
//		QString currentDate = dt.toString("yyyy-MM-dd hh:mm");
//		File file;
//		User *user = new User();
//		qDebug() << "name: " << globalUserName;
//		user->queryUserByName(globalUserName);
//		//qDebug() << user->getUserName();
//		//qDebug() << globalUserName;
//		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
//		file.setFileSize(QString::number(RtotalSize, 10));
//		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //获取文件后缀;
//		file.setFileTime(currentDate);
//		file.setUserId((int)user->getUserId());  //获取由全局名得到的userid
//
//		qDebug() << "the file name:" << file.getFileName();
//		qDebug() << "the file time is :" << file.getFileTime();
//		qDebug() << "the userid :" << user->getUserId();
//		//qDebug() << "the userid from file:" << file.getUserId();
//
//		if (file.insertFile())
//		{
//			//emit receiveDone();
//			qDebug() << file.getFileName() << "insert file success!";
//		}
//		else
//			qDebug() << "insert error!";
//		inBlock.clear();
//		byteReceived = 0;
//		RtotalSize = 0;
//		receiveTime = 0;
//		newFile->close();
//		//quit();  //退出该线程
//				 //
//				 //if (wait())
//				 //{
//				 //	qDebug() << "the Thread is closed!";
//				 //}
//
//				 /*disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
//				 connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));*/
//	}
//}


//bool MyTcpSocket::openFile(QString filename)
//{
//	//qDebug() << globalUserName << " will send a file to server";
//	loadSize = 0;
//	byteToWrite = 0;
//	//RtotalSize = 0;
//	sendTimes = 0;
//	outBlock.clear();
//	//这个不能用
//	sendFileName = filename;
//	localFile = new QFile("files/" + filename);
//	if (localFile->open(QFile::ReadOnly))
//	{
//		qDebug() << "open file:" << filename << " success!";
//		qDebug() << "thread now" << QThread::currentThreadId();
//		//sendFile();
//		return true;
//	}
//	else
//	{
//		qDebug() << "open file failed!";
//		return false;
//	}
//
//}
//
////发送文件头
//void MyTcpSocket::sendFile()
//{
//	connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//	qDebug() << "send thread now" << QThread::currentThreadId();
//	byteToWrite = localFile->size();  //剩余数据的大小  
//	qDebug() << "the file bytetowrite: " << byteToWrite;
//	StotalSize = localFile->size();
//
//	loadSize = 4 * 1024;  //每次发送数据的大小  
//
//	QDataStream out(&outBlock, QIODevice::WriteOnly);
//
//	//获取文件名字
//	currentFileName = sendFileName.right(sendFileName.size() - sendFileName.lastIndexOf('/') - 1);
//
//	//前面两个是文件大小和发送文件头的大小，后面是文件名和用户名
//	out << qint64(0) << qint64(0) << currentFileName;
//
//	StotalSize += outBlock.size();  //总大小为文件大小加上文件名等信息大小  
//	byteToWrite += outBlock.size();
//	qDebug() << "the total bytetowrite: " << byteToWrite;
//	out.device()->seek(0);  //回到字节流起点来写好前面连个qint64，分别为总大小和文件名等信息大小  
//	out << StotalSize << qint64(outBlock.size());
//	//qDebug() << "the file head:" << outBlock;
//	this->write(outBlock);  //将读到的文件信息发送到套接字 
//}
//
//void MyTcpSocket::goOnSend(qint64 numBytes)
//{
//	//QTcpSocket *tcpSocket = static_cast<QTcpSocket*>(this->sender());
//	//QString clientip = tcpSocket->peerAddress().toString();  //得到连接客户端的IP
//
//	sendTimes++;
//	qDebug() << " threadId:" << QThread::currentThreadId() << "sockID:" << socketDescriptor1
//		<< " " << currentFileName << "the " << sendTimes << "the numBytes: " << numBytes << "the loadSize:" << loadSize
//		<< "  left byteTowrite: " << byteToWrite;
//
//	byteToWrite -= numBytes;  //剩余数据大小  
//	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //如果剩余数据比每次发送的小则发送剩余的
//
//	this->write(outBlock);    //将这个信息写入socket
//
//	if (byteToWrite == 0)  //发送完毕  
//	{
//		//qDebug()<<QString::fromLocal8Bit("文件发送完毕!");
//		disconnect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//		sendTimes = 0;
//		qDebug() << " threadId:" << QThread::currentThreadId()
//			<< "sockID:" << socketDescriptor1 << currentFileName << "the file has sended";
//
//		localFile->close();  //发送完文件要关闭，不然不能对其进行下一步操作
//		//quit();
//	}
//}
