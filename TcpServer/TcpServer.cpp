//#include "TcpServer.h"
//#include "ui_TcpServer.h"
//#include "Database.h"
//#include "User.h"
//#include "File.h"
//#include "Report.h"
//
//const QString connectflag = "server return message: connect server is ok!";
//
//const QString hasconnect = "I'm still connect";
//
//const int kBase = 1000;
//
//const char *socketErrors[100] = { "ConnectionRefusedError",
//									"RemoteHostClosedError",
//									"HostNotFoundError",
//									"SocketAccessError",
//									"SocketResourceError",
//									"SocketTimeoutError",
//									"DatagramTooLargeError",
//									"NetworkError",
//									"AddressInUseError",
//									"SocketAddressNotAvailableError",
//									"UnsupportedSocketOperationError",
//									"ProxyAuthenticationRequiredError",
//									"SslHandshakeFailedError",
//									"UnfinishedSocketOperationError",
//									"ProxyConnectionRefusedError",
//									"ProxyConnectionClosedError",
//									"ProxyConnectionTimeoutError",
//									"ProxyNotFoundError",
//									"ProxyProtocolError",
//									"UnknownSocketError"
//};
//
//
////MySql *mysql = new MySql();
////User *user;
//
//TcpServer::TcpServer(QObject *parent) :
//	QObject(parent), m_hearttime(5),
//	m_startHeartTime(false)
//{
//	//ui->setupUi(this);
//	init();
//}
//
//TcpServer::~TcpServer()
//{
//	delete ui;
//}
////TCP初始化
//void TcpServer::init()
//{
//	this->tcpServer = new QTcpServer(this);
//	this->tcpSocket = new QTcpSocket(this);
//
//	//有新的连接请求则触发槽函数acceptConnection
//	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
//	m_timer = new QTimer();
//
//	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_updateUserList()));
//	//错误则提示
//	//connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//	//	this, SLOT(displayError(QAbstractSocket::SocketError)));
//
//	//如果监听端口不成功返回信息
//	if (!tcpServer->listen(QHostAddress::Any, 3389))
//	{
//		qDebug() << tcpServer->errorString();
//		//close();
//		return;
//	}
//	else
//		qDebug() << "start listenning!";
//}
//
//void TcpServer::acceptConnection()
//{
//	//tcpSocket = tcpServer->nextPendingConnection();
//	QTcpSocket *newtcpsocket = tcpServer->nextPendingConnection();
//
//	if (NULL == newtcpsocket)
//	{
//		qDebug() << "socket is error";
//		return;
//	}
//	tcpSocket = newtcpsocket;
//	QHostAddress address = newtcpsocket->peerAddress();
//
//	QString clientIp = address.toString();
//
//	//输出连接的用户IP
//	qDebug() << "clientIp: " << clientIp;
//
//	//当前用户数量
//	qDebug() << "currnet user count: " << m_userlist.size();
//	if (m_userlist.size() > 0)
//	{
//		qDebug() << "they is :";
//	}
//
//	QMap<QString, QTcpSocket*>::Iterator iter;
//	for (iter = m_userlist.begin(); iter != m_userlist.end(); ++iter)
//	{
//		qDebug() << iter.key();
//	}
//
//	//如果是新的连接，怎么解决IP相同的问题？
//	//if (NULL == m_userlist[clientIp])
//	{
//		qDebug() << "new user join it's ip: " << clientIp;
//
//		m_userlist.insert(clientIp, newtcpsocket);
//
//		connect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//		connect(newtcpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
//
//
//		/*if ("" != clientIp)
//		{
//			senddata(newtcpsocket, connectflag);
//		}*/
//		if (false == m_startHeartTime)
//		{
//			//qDebug() << "start heart time";  
//			startHeart();
//			m_startHeartTime = true;
//		}
//	}
//
//	//接受连接请求如果有可读信息则唤醒
//	//connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));   
//	//connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
//	
//}
//void TcpServer::startHeart()
//{
//	int socketcount = m_userlist.size();
//
//	if (socketcount > 0)
//	{
//		m_timer->start(m_hearttime * kBase);
//	}
//	else
//	{
//		m_startHeartTime = false;
//		m_timer->stop();
//	}
//}
////验证和查询一些数据,TCP的最主要的接受部分
//void TcpServer::receiveData()
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
//	QString clientip = newtcpsocket->peerAddress().toString();  //得到；连接客户端的IP
//
//	QByteArray dataread = newtcpsocket->readAll();
//	QString data = QString::fromUtf8(dataread);
//	user = new User();  //在这里初始化应该不错
//
//	qDebug() << "user " << clientip << " the data from client: " << data;
//	QStringList list = data.split("#");  //用#分割
//	bool ret = 0;
//
//	if (list[0] == "R")     //注册
//		ret = checkRegister(list[1]);
//	else if (list[0] == "L")  //登录
//		ret = checkLogin(list[1]);
//	else  if (list[0] == "RCU")  //检查用户名是否重复
//		ret = checkUser(list[1]);
//	else if (list[0] == "RCS") //检查学号是否重复
//		ret = checkUser(list[1]);
//
//
//	//客户端发送UserInfo和用户名过来，查询用户名
//	else if (list[0] == "QueryUserInfo")
//	{
//		QString sendName = list[1];
//		qDebug() << "list[1]:" << sendName;
//		//user = new User();  //不能在这里初始化啊，得在登录的时候初始化，不然如果直接上传文件会触发空指针
//		user->queryUserByName(list[1]);
//		QString data = user->getUserName() + "#" + user->getUserPassword() + "#" + user->getStudentId()
//			+ "#" + user->getPhoneNumber() + "#" + user->getQqNumber() + "#" + user->getMail();
//		QByteArray datasend = data.toLocal8Bit();		//将字符转换成LOCAL8BIT？？？
//
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " data has sended:" <<datasend;
//			return;
//		}
//		return;
//	}
//
//	//如果接收到的是修改用户信息
//	else if (list[0] == "alterUserInfo")
//	{
//		user->queryUserByName(list[1]);
//		user->setStudentId(list[2]);
//		user->setPhoneNumber(list[3]);
//		user->setQqNumber(list[4]);
//		user->setMail(list[5]);
//		QString data = "updateUserInfo#";
//		if (user->updataUser())
//		{
//			qDebug() << "update userInfo success!";
//			data += "true";
//		}
//		else
//		{
//			qDebug() << "update userInfo failed!";
//			data += "false";
//		}
//		QByteArray datasend = data.toUtf8();
//		qDebug() << "user " << clientip <<" "<< datasend;
//		newtcpsocket->write(datasend);
//		return;
//	}
//	//查询特定用户的文件
//	else if (list[0] == "findFileByName") 
//	{
//		File *file = new File();   //初始化一个file
//		user->queryUserByName(list[1]);
//		QList<FileInfo> fileList = file->queryFilebyUser(user->getUserName());
//		QString dataFile;  //总数据发到系统里
//		
//		//先将该信息的个数发送过去
//		QString dataLenth = QString::number(fileList.size(), 10);
//		dataFile += dataLenth + "$";		//信息的第一位是文件个数
//
//		//将该数据循环发送到客户端
//		for (int i = 0;i < fileList.size();i++) {
//			//超长Data？
//			QString data = QString::number(fileList.at(i).fileId, 10) + "#" +
//				fileList.at(i).fileName + "#" + fileList.at(i).fileSize +
//				"#" + fileList.at(i).fileType + "#" + fileList.at(i).fileTime+ "#"+
//				user->getUserName()+ "#" +
//				QString::number(fileList.at(i).userId, 10);
//			data += "$";
//			qDebug() << data;
//			dataFile += data;
//			//tcpSocket->write((char *)&fileList.at(i), sizeof(fileList.at(i)));
//		}
//
//		QByteArray datasend = dataFile.toUtf8();   //转成uft8发送到客户端
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " file's data has sended:" << datasend;
//			return;
//		}
//		return;
//		//qDebug() << fileList.at(1).fileName;//可以访问
//	}
//	//查询所有用户的文件
//	else if (list[0] == "findFile")
//	{
//		File *file = new File();   //初始化一个file
//		//user->queryUserByName(list[1]);
//		//User *userFile = new User();  //用来查询文件所有者
//		QList<FileInfo> fileList = file->queryFile(list[1]);
//		QString dataFile;  //总数据发到系统里
//		QString dataLenth = QString::number(fileList.size(), 10);
//		dataFile += dataLenth + "$";		//信息的第一位是文件个数
//
//											//将该数据循环发送到客户端
//		for (int i = 0;i < fileList.size();i++) {
//			//超长Data？
//			user->queryUserById(fileList.at(i).userId);  //循环查询用户的id
//			qDebug() << "the fileUser is :" << user->getUserName();
//			
//
//			QString data = QString::number(fileList.at(i).fileId, 10) + "^" +
//				fileList.at(i).fileName + "^" + fileList.at(i).fileSize +
//				"^" + fileList.at(i).fileType + "^" + fileList.at(i).fileTime + "^" +
//				user->getUserName() + "^" +
//				QString::number(fileList.at(i).userId, 10);
//			data += "$";
//			qDebug() << data;
//			dataFile += data;
//			//tcpSocket->write((char *)&fileList.at(i), sizeof(fileList.at(i)));
//		}
//		QByteArray datasend = dataFile.toUtf8();   //转成uft8发送到客户端
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " file's data has sended:" << datasend;
//			return;
//		}
//		return;
//		//qDebug() << fileList.at(1).fileName;//可以访问
//	}
//	else if (list[0] == "uploadFile")   //如果发送的upload信息
//	{
//		qDebug() << "change to receiveFile";
//		StotalSize = 0;
//		byteReceived = 0;
//		receiveTime = 0;
//		disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//		connect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
//
//		//receiveFile = new ReceiveFile();// 初始化定义
//		//receiveFile->init();  //初始化文件
//		//disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//		//connect(newtcpsocket, SIGNAL(readyRead()), receiveFile, SLOT(receive())); //触发receiveFile的槽函数
//		return;
//	}
//	else if (list[0] == "downloadFile")
//	{
//		qDebug() << "user " << clientip << " begin to send file to client";
//		openFile(list[1]);
//		return;  //这里没有加返回，尼玛我是说为什么会收到downloadFile的信息
//	}
//	//
//	else if (list[0] == "deleteFile")
//	{
//		qDebug() << "user " << clientip << "want to delete a file named:" << list[1];
//		deleteFile(list[1],list[2].toInt());
//		return;
//	}
//
//	//从服务端发来的报告人查询
//	else if (list[0] == "QueryReporter")
//	{
//		user->queryUserByName(list[1]);
//		QString data = user->getTrueName() + "#" + user->getStudentId() + "#" + user->getLevel()
//			+ "#" + user->getTeacher() + "#" + QString::number(user->getUserId(), 10);
//		QByteArray datasend = data.toUtf8();		//将字符转换成LOCAL8BIT？？？
//
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " data has sended:" << datasend;
//			return;
//		}
//		return;
//	}
//	//发来的report，插入到数据库
//	else if (list[0] == "sendReport")
//	{
//		//QByteArray dataread = newtcpsocket->readAll();
//		//QString data = QString::fromUtf8(dataread);
//		//qDebug() << "the data from client: " << dataread;
//		//QStringList listNumber = data.split("#");
//		Report *report = new Report();
//
//		//先查询一下该报告是否存在,一周只能发一个周报
//		QString Repeat = "1#reportRepeat#";
//		
//
//		ReportInfo ri;
//		ri.reportUserId = list[1].toInt();
//		ri.reportTerm = list[2];
//		ri.reportWeek = list[3];
//		ri.reportMain = list[4];
//		ri.reportQuestionForSolve = list[5];
//		ri.reportQuestionFromLastWeek = list[6];
//		ri.reportQuestionHasSolved = list[7];
//		ri.reportYourGet = list[8];
//		//1#reportRepeat#reportUserId#reportTerm#reportWeek
//		Repeat += list[1] + "#" + list[2] + "#" + list[3];
//		QList<ReportInfo> ReportList = report->queryReport(Repeat);
//
//		//如果大于0则该学期该周已经存在周报，不可重复提交
//		if (ReportList.size() > 0)
//		{
//			QString data = "ReportEdit#repeatTrue";
//			QByteArray datasend = data.toUtf8();		//将字符转换成LOCAL8BIT？？？
//
//			if (newtcpsocket->write(datasend))
//			{
//				qDebug() << "user " << clientip << " data has sended:" << datasend;
//				return;
//			}
//			return;
//		}
//		
//		QDateTime dt;
//		QTime time;
//		QDate date;
//		dt.setTime(time.currentTime());
//		dt.setDate(date.currentDate());
//		QString currentDate = dt.toString("yyyy-MM-dd hh:mm");
//		ri.reportTime = currentDate;
//
//		//设置report类的info
//		report->setReportInfo(ri);
//
//		if (report->insertReport())
//		{
//			qDebug() << "insert report success!";
//			QString data = "ReportEdit#true";
//			QByteArray datasend = data.toUtf8();		//将字符转换成LOCAL8BIT？？？
//
//			if (newtcpsocket->write(datasend))
//			{
//				qDebug() << "user " << clientip << " data has sended:" << datasend;
//				return;
//			}
//		}
//		else
//		{
//			qDebug() << "insert error!";
//			QString data = "ReportEdit#false";
//			QByteArray datasend = data.toUtf8();		//将字符转换成LOCAL8BIT？？？
//
//			if (newtcpsocket->write(datasend))
//			{
//				qDebug() << "user " << clientip << " data has sended:" << datasend;
//				return;
//			}
//		}
//		return;
//
//	}
//	//接收报告列表请求
//	else if (list[0] == "lookReport")
//	{
//		Report *report = new Report();   //初始化一个Report
//		QList<ReportInfo> ReportList = report->queryReport(data);
//		//如果命令是查看所有文件
//		QString dataReport;  //总数据发到系统里
//
//		QString dataLenth = QString::number(ReportList.size(), 10);
//
//		////如果查询大于1个，则要加$
//		//if(ReportList.size() > 0)
//			dataReport += dataLenth + "$";		//信息的第一位是文件个数
//												//将该数据循环发送到客户端
//		for (int i = 0;i < ReportList.size();i++) {
//				//超长Data？
//
//			QString data = ReportList.at(i).reportId + "#" +
//				ReportList.at(i).reportUserName + "#" + ReportList.at(i).reportTerm +
//				"#" + ReportList.at(i).reportWeek + "#" + ReportList.at(i).reportMain +
//				"#" + ReportList.at(i).reportTime + "#" + ReportList.at(i).reportName +
//			    "#" + ReportList.at(i).reportQuestionForSolve +
//				"#" + ReportList.at(i).reportQuestionHasSolved + "#" + ReportList.at(i).reportQuestionFromLastWeek +
//				"#" + ReportList.at(i).reportYourGet + 
//				"#" + QString::number(ReportList.at(i).reportUserId,10);
//			if (ReportList.size() > 1)
//				data += "$";
//			qDebug() << data;
//			dataReport += data;
//		}
//
//		QByteArray datasend = dataReport.toUtf8();   //转成uft8发送到客户端
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " Report's data has sended:" << datasend;
//			return;
//		}
//		return;
//	}
//	else if (list[0] == "deleteReport")
//	{
//		qDebug() << "user " << clientip << "want to delete a report named:" << list[1];
//		deleteReport(list[1].toInt());
//		return;
//	}
//	else
//		return;
//	//qDebug() <<"the sql return "<< ret;
//
//	//这是几个用来验证的，前面的都有return，如果都不匹配会走到这一步来
//	QString sendData = list[0];
//	if (ret)
//		sendData += "#true";
//	else
//		sendData += "#false";
//	//又把数据发送给客户端
//	newtcpsocket->write(sendData.toUtf8());
//	qDebug() << "user " << clientip << " the data has send to client: " << sendData;
//
//	/*
//	if (connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData())))
//		qDebug() << "change to receiveData";*/
//}
//
////发送文件区域
//void TcpServer::openFile(QString filename)
//{
//	//qDebug() << globalUserName << " will send a file to server";
//	loadSize = 0;
//	byteToWrite = 0;
//	//RtotalSize = 0;
//	sendTimes = 0;
//	outBlock.clear();
//	//这个不能用
//	sendFileName = filename;
//	localFile = new QFile("files/"+filename);
//	if (localFile->open(QFile::ReadOnly))
//	{
//		qDebug() << "open file:" << filename << " success!";
//		sendFile();
//	}
//	else
//	{
//		qDebug() << "open file failed!";
//	}
//
//}
//
//void TcpServer::sendFile()
//{
//	//send()
//	//disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
//	QString clientip = newtcpsocket->peerAddress().toString();  //得到；连接客户端的IP
//
//	connect(newtcpsocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//
//
//	byteToWrite = localFile->size();  //剩余数据的大小  
//	qDebug() << "the file bytetowrite: " << byteToWrite;
//	StotalSize = localFile->size();
//
//	loadSize = 4 * 1024;  //每次发送数据的大小  
//
//	QDataStream out(&outBlock, QIODevice::WriteOnly);
//
//	//获取文件名字
//	QString currentFileName = sendFileName.right(sendFileName.size() - sendFileName.lastIndexOf('/') - 1);
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
//	newtcpsocket->write(outBlock);  //将读到的文件信息发送到套接字 
//
//}
//
//void TcpServer::goOnSend(qint64 numBytes)
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
//	QString clientip = newtcpsocket->peerAddress().toString();  //得到连接客户端的IP
//
//	sendTimes++;
//	qDebug() << "user " << clientip << " was " << sendTimes << " get in goOnSend";
//	qDebug() << "the numBytes: " << numBytes << "the loadSize:" << loadSize << "  left byteTowrite: " << byteToWrite;
//
//	byteToWrite -= numBytes;  //剩余数据大小  
//	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //如果剩余数据比每次发送的小则发送剩余的
//
//	newtcpsocket->write(outBlock);    //将这个信息写入socket
//
//	if (byteToWrite == 0)  //发送完毕  
//	{
//		//qDebug()<<QString::fromLocal8Bit("文件发送完毕!");
//		disconnect(newtcpsocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//		sendTimes = 0;
//		qDebug() << "user " << clientip << "the file has sended";
//
//		localFile->close();  //发送完文件要关闭，不然不能对其进行下一步操作
//	}
//}
//
//
//
////接受文件区域
//void TcpServer::receiveFile()
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //每个函数都要用这个？
//	QString clientip = newtcpsocket->peerAddress().toString();  //得到连接客户端的IP
//
//	qDebug() << "user " << clientip << " was " << receiveTime << "get in receiveFile!!!";
//		if (byteReceived == 0)  //才刚开始接收数据，此数据为文件信息  
//		{
//			qDebug() << "receive the file of head";
//			//ui->receivedProgressBar->setValue(0);
//			QDataStream in(newtcpsocket);    //从里面读
//			QString globalUserName;
//			in >> RtotalSize >> byteReceived >> fileName >>globalUserName ;   //读的是二进制字节？
//			user->queryUserByName(globalUserName);
//			qDebug() << "user " << clientip << " name is :"<<user->getUserName();
//
//			QByteArray change = fileName.toUtf8();
//			fileName = "files/" + QString::fromUtf8(change);
//
//			qDebug() << "the file of head: " << fileName;
//			qDebug() << "totalSize: " << RtotalSize;
//			qDebug() << " first byteReceived: " << byteReceived;
//
//			//设计一个文件读写不重复的功能
//			int i = 0;
//			QString suffix = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1); //获取文件后缀;
//			QString fullName = fileName.left(fileName.size() - (fileName.size() - fileName.lastIndexOf('.'))); //获取文件名
//			while (1)
//			{
//				QFile *existFile = new QFile(fileName);  //就是判断是否重复
//				if (existFile->exists())   //如果文件存在
//				{
//					i++;
//					fileName = fullName + "(" + QString::number(i, 10) + ")." + suffix ;
//					
//					qDebug() << "the fileName :" << fileName;
//				}
//				else
//					break;
//			}
//
//			newFile = new QFile(fileName);
//			newFile->open(QFile::WriteOnly);
//		}
//		else  //正式读取文件内容    
//		{
//			receiveTime++;
//			qDebug() << "the "<<receiveTime<<" receive the file ";
//			inBlock = newtcpsocket->readAll();
//			qDebug() << "the inBlock size is " << inBlock.size();
//			qDebug() << "current receive the byte is :" << byteReceived;
//			//qDebug() << "the block: " << inBlock;
//			byteReceived += inBlock.size();
//
//			qDebug() << "begin to write";
//			newFile->write(inBlock);
//			newFile->flush();   //直接进入接收文件了？
//		}
//
//		if (byteReceived == RtotalSize)
//		{
//			qDebug() << "user " << clientip  << "receive is done";
//			//接收完成向数据库插入文件信息
//			QDateTime dt;
//			QTime time;
//			QDate date;
//			dt.setTime(time.currentTime());
//			dt.setDate(date.currentDate());
//			QString currentDate = dt.toString("yyyy-MM-dd hh:mm");
//			File file;
//			file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
//			file.setFileSize(QString::number(RtotalSize,10));
//			file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //获取文件后缀;
//			file.setFileTime(currentDate);
//			file.setUserId((int)user->getUserId());  //获取由全局名得到的userid
//
//			qDebug() << "the file name:"<<file.getFileName() ;
//			qDebug() << "the file time is :" << file.getFileTime();
//			qDebug() << "the userid :" << user->getUserId();
//			qDebug() << "the userid from file:" << file.getUserId();
//
//			if (file.insertFile())
//			{
//				qDebug() << file.getFileName() << "insert file success!";
//			}
//			else
//				qDebug() << "insert error!";
//
//			inBlock.clear();
//			byteReceived = 0;
//			RtotalSize = 0;
//			receiveTime = 0;
//			newFile->close();
//			disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
//			connect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//			
//		}
//}
//
//void TcpServer::deleteFile(QString deleteName,int fileId)
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //每个函数都要用这个？
//	QString clientip = newtcpsocket->peerAddress().toString();  //得到连接客户端的IP
//	if (!deleteName.isEmpty())
//	{
//		QString data = "deleteFile#";
//		if (QFile::remove("files/" + deleteName))
//		{
//			qDebug() << "the file:" << deleteName << "has deleted!";
//			File file;
//			if (file.deleteFile(fileId))
//			{
//				data += "true";
//				qDebug() << "the file in database has also deleted!";
//			}
//			else
//			{
//				qDebug() << "delete database failed";
//			}
//		}
//		else
//		{
//			qDebug() << "can not delete file, maybe it is not exist!";
//			data += "false";
//		}
//		QByteArray datasend = data.toUtf8();
//		qDebug() << "user " << clientip << " " << datasend;
//		newtcpsocket->write(datasend);
//	}
//}
//
//void TcpServer::deleteReport(int reportId)
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //每个函数都要用这个？
//	QString clientip = newtcpsocket->peerAddress().toString();  //得到连接客户端的IP
//	QString data = "deleteReport#";
//	if (reportId)
//	{
//		
//		qDebug() << "the Report:" << reportId << "has deleted!";
//		Report report;
//		if (report.deleteReport(reportId))
//		{
//			data += "true";
//			qDebug() << "the Report in database has also deleted!";
//		}
//		else
//		{
//			data += "false";
//			qDebug() << "delete database failed";
//		}
//	}
//	else
//	{
//		qDebug() << "can not delete Report, maybe it is not exist!";
//		data += "false";
//	}
//	QByteArray datasend = data.toUtf8();
//	qDebug() << "user " << clientip << " " << datasend;
//	newtcpsocket->write(datasend);
//}
//
//
////如果有掉线的就在list里去除这个socket
//void TcpServer::slot_updateUserList()
//{
//	QMap<QString, QTcpSocket*>::Iterator iter;
//	for (iter = m_userlist.begin(); iter != m_userlist.end(); )
//	{
//		//senddata()  
//		QAbstractSocket::SocketState curstate = (*iter)->state();
//
//		if (QAbstractSocket::UnconnectedState == curstate ||
//			QAbstractSocket::ClosingState == curstate)
//		{
//			qDebug() << "erase close socket";
//			iter = m_userlist.erase(iter);
//		}
//		else
//		{
//			++iter;
//		}
//	}
//}
//
//void TcpServer::displayError(QAbstractSocket::SocketError errors)
//{
//	QString displays = "";
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
//
//	//发生错误时，服务端踢掉出错socket  
//	m_userlist.remove(newtcpsocket->peerAddress().toString());
//
//	qDebug() << "user " << newtcpsocket->peerAddress().toString() << " is left..";
//
//	if (errors == 1)
//	{
//		return;
//	}
//	else if (-1 == errors)
//	{
//		displays = socketErrors[19];
//	}
//	else
//	{
//		displays = socketErrors[errors];
//	}
//	qDebug() << tcpSocket->errorString();
//	tcpSocket->close();
//}
//
//bool TcpServer::checkLogin(QString sql)
//{
//
//	bool ret = mysql->queryDB(sql);
//	return ret;
//}
//
//bool TcpServer::checkRegister(QString sql)
//{
//
//	bool ret = mysql->insertDB(sql);
//	return ret;
//}
//
//bool TcpServer::checkUser(QString sql)
//{
//	bool ret = mysql->queryDB(sql);
//	return ret;
//
//}
