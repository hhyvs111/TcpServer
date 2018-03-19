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
		qDebug() << socketDescriptor<< "disconnect";
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
	qDebug() <<"socketId:"<<socketDescriptor1<< data;	//处理过后的信息
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
			sendData += "#false";
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
			sendData += "#false";
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
			sendData += "#false";
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
		disconnect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::readData);
		connect(this, SIGNAL(readyRead()),
			this, SLOT(receiveFile()), Qt::DirectConnection);
		byteReceived = 0;
		receiveTime = 0;
		//socketDescriptor将这个传入MyThread
		//MyThread *uploadThread = new MyThread(socketDescriptor1,"upload",this);

		//uploadThread->setObjectName("receiveThread");
		////结束后删除
		//connect(uploadThread, SIGNAL(receiveDone()), uploadThread, SLOT(ThreadExit()));
		//connect(uploadThread, SIGNAL(finished()), uploadThread, SLOT(deleteLater()));
		//uploadThread->start();


		//qDebug()<<"the main thread:"<< QThread::currentThreadId();
		return "N";
	}
	//下载文件
	else if (list[0] == "downloadFile")
	{
		qDebug() << "begin to downloadFile";
		MyThread *downloadThread = new MyThread(socketDescriptor1, "download", list[1], list[2],this);
		
		connect(downloadThread, SIGNAL(finished()), downloadThread, SLOT(deleteLater()));
		connect(downloadThread, SIGNAL(receiveDone()), downloadThread, SLOT(ThreadExit()));
		downloadThread->start();
	}
	else if (list[0] == "downloadBreakFile")
	{
		MyThread *downloadThread = new MyThread(socketDescriptor1, "downloadBreakFile", list[1], list[2],list[3],this);

		connect(downloadThread, SIGNAL(finished()), downloadThread, SLOT(deleteLater()));
		connect(downloadThread, SIGNAL(receiveDone()), downloadThread, SLOT(ThreadExit()));
		downloadThread->start();
	}
	else if (list[0] == "deleteFile")
	{
		//deleteFile(list[1], list[2].toInt());
		QString deleteName = list[1];
		int fileId = list[2].toInt();
		if (!deleteName.isEmpty())
			{
				QString data = "deleteFile#";
				if (QFile::remove("files/" + deleteName))
				{
					qDebug() << "the file:" << deleteName << "has deleted!";
					File file;
					if (file.deleteFile(fileId))
					{
						data += "true";
						qDebug() << "the file in database has also deleted!";
					}
					else
					{
						qDebug() << "delete database failed";
					}
				}
				else
				{
					qDebug() << "can not delete file, maybe it is not exist!";
					data += "false";
				}
				QByteArray datasend = data.toUtf8();
				return datasend;
			}
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
	else if (list[0] == "lookReport")
	{
		Report *report = new Report();   //初始化一个Report
		QList<ReportInfo> ReportList = report->queryReport(data);
		//如果命令是查看所有文件
		QString dataReport;  //总数据发到系统里
		QString dataLenth = QString::number(ReportList.size(), 10);
		
		////如果查询大于1个，则要加$
		//if(ReportList.size() > 0)
			dataReport += dataLenth + "$";		//信息的第一位是文件个数
												//将该数据循环发送到客户端
		for (int i = 0;i < ReportList.size();i++) {
				//超长Data？
		
			QString data = ReportList.at(i).reportId + "#" +
				ReportList.at(i).reportUserName + "#" + ReportList.at(i).reportTerm +
				"#" + ReportList.at(i).reportWeek + "#" + ReportList.at(i).reportMain +
				"#" + ReportList.at(i).reportTime + "#" + ReportList.at(i).reportName +
				"#" + ReportList.at(i).reportQuestionForSolve +
				"#" + ReportList.at(i).reportQuestionHasSolved + "#" + ReportList.at(i).reportQuestionFromLastWeek +
				"#" + ReportList.at(i).reportYourGet + 
				"#" + QString::number(ReportList.at(i).reportUserId,10);
			if (ReportList.size() > 1)
				data += "$";
			qDebug() << data;
			dataReport += data;
		}
		
		return dataReport.toUtf8();   //转成uft8发送到客户端

	}
	else if (list[0] == "deleteReport")
	{
		qDebug() << "user " << "want to delete a report named:" << list[1];
		QString dataToSend = "deleteReport#";
		int reportId = list[1].toInt();
		if (reportId)
		{
			//qDebug() << "the Report:" << reportId << "has deleted!";
			Report report;
			if (report.deleteReport(reportId))
			{
				dataToSend += "true";
				qDebug() << "the Report in database has also deleted!";
			}
			else
			{
				dataToSend += "false";
				qDebug() << "delete database failed";
			}
		}
		else
		{
			qDebug() << "can not delete Report, maybe it is not exist!";
			dataToSend += "false";
		}
		return dataToSend.toUtf8();
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

void MyTcpSocket::receiveFile()
{
	if (byteReceived == 0)  //才刚开始接收数据，此数据为文件信息  
	{
		qDebug() << "receive sockID:" << socketDescriptor1
			<< "start new Thread id is" << QThread::currentThreadId();

		QDataStream in(this);    //从里面读

		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //读的是二进制字节？

		fileName = "files/" + fileName;

		qDebug() << "receive sockID:" << socketDescriptor1 << "the filename of head: " << fileName
			<< "totalSize: " << RtotalSize << " first byteReceived: " << byteReceived;


		//设计一个文件读写不重复的功能
		int i = 0;
		QString suffix = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1); //获取文件后缀;
		QString fullName = fileName.left(fileName.size() - (fileName.size() - fileName.lastIndexOf('.'))); //获取文件名
		while (1)
		{
			QFile *existFile = new QFile(fileName);  //就是判断是否重复
			if (existFile->exists())   //如果文件存在
			{
				i++;
				fileName = fullName + "(" + QString::number(i, 10) + ")." + suffix;
				qDebug() << "receive sockID:" << socketDescriptor1 << "the fileName :" << fileName;
			}
			else
				break;
		}

		newFile = new QFile(fileName);
		newFile->open(QFile::WriteOnly);
	}
	else  //正式读取文件内容    
	{
		receiveTime++;
		qDebug() << "sockId:" << socketDescriptor1 << "ThreadId: " << QThread::currentThreadId() << fileName << " the " << receiveTime << "recv " /*<< "the inBlock size is " << inBlock.size()*/
			<< "current receive the byte is :" << byteReceived << " and the total " << RtotalSize;

		inBlock = this->readAll();
		byteReceived += inBlock.size();
		newFile->write(inBlock);
		newFile->flush();   //直接进入接收文件
	}

	if (byteReceived == RtotalSize)
	{
		//qDebug() << "user " << clientip << "receive is done";
		//接收完成向数据库插入文件信息
		QDateTime dt;
		QTime time;
		QDate date;
		dt.setTime(time.currentTime());
		dt.setDate(date.currentDate());
		QString currentDate = dt.toString("yyyy-MM-dd hh:mm:ss");
		File file;
		//User user;
		qDebug() << "receive sockID:" << socketDescriptor1 << "globalUserName: " << globalUserName;
		while (1)
		{
			//如果没用
			if (!inUse)
			{
				user->queryUserByName(globalUserName);
				break;
			}
		}
		//qDebug() << user->getUserName();
		//qDebug() << globalUserName;
		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
		file.setFileSize(QString::number(RtotalSize, 10));
		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //获取文件后缀;
		file.setFileTime(currentDate);
		file.setUserId((int)user->getUserId());  //获取由全局名得到的userid

		qDebug() << "receive sockID:" << socketDescriptor1 << "the file name:" << file.getFileName() << "the file time is :" << file.getFileTime()
			<< "the userid :" << user->getUserId();
		//qDebug() << "the userid from file:" << file.getUserId();

		if (file.insertFile())
		{
			inBlock.clear();
			byteReceived = 0;
			RtotalSize = 0;
			receiveTime = 0;
			newFile->close();

			qDebug() << "receive sockID:" << socketDescriptor1 << file.getFileName() << "insert file success!";
		}
		else
			qDebug() << "receive sockID:" << socketDescriptor1 << "insert error!";
		//emit receiveDone();
	}
}