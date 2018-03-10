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
////TCP��ʼ��
//void TcpServer::init()
//{
//	this->tcpServer = new QTcpServer(this);
//	this->tcpSocket = new QTcpSocket(this);
//
//	//���µ����������򴥷��ۺ���acceptConnection
//	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
//	m_timer = new QTimer();
//
//	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_updateUserList()));
//	//��������ʾ
//	//connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//	//	this, SLOT(displayError(QAbstractSocket::SocketError)));
//
//	//��������˿ڲ��ɹ�������Ϣ
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
//	//������ӵ��û�IP
//	qDebug() << "clientIp: " << clientIp;
//
//	//��ǰ�û�����
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
//	//������µ����ӣ���ô���IP��ͬ�����⣿
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
//	//����������������пɶ���Ϣ����
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
////��֤�Ͳ�ѯһЩ����,TCP������Ҫ�Ľ��ܲ���
//void TcpServer::receiveData()
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
//	QString clientip = newtcpsocket->peerAddress().toString();  //�õ������ӿͻ��˵�IP
//
//	QByteArray dataread = newtcpsocket->readAll();
//	QString data = QString::fromUtf8(dataread);
//	user = new User();  //�������ʼ��Ӧ�ò���
//
//	qDebug() << "user " << clientip << " the data from client: " << data;
//	QStringList list = data.split("#");  //��#�ָ�
//	bool ret = 0;
//
//	if (list[0] == "R")     //ע��
//		ret = checkRegister(list[1]);
//	else if (list[0] == "L")  //��¼
//		ret = checkLogin(list[1]);
//	else  if (list[0] == "RCU")  //����û����Ƿ��ظ�
//		ret = checkUser(list[1]);
//	else if (list[0] == "RCS") //���ѧ���Ƿ��ظ�
//		ret = checkUser(list[1]);
//
//
//	//�ͻ��˷���UserInfo���û�����������ѯ�û���
//	else if (list[0] == "QueryUserInfo")
//	{
//		QString sendName = list[1];
//		qDebug() << "list[1]:" << sendName;
//		//user = new User();  //�����������ʼ���������ڵ�¼��ʱ���ʼ������Ȼ���ֱ���ϴ��ļ��ᴥ����ָ��
//		user->queryUserByName(list[1]);
//		QString data = user->getUserName() + "#" + user->getUserPassword() + "#" + user->getStudentId()
//			+ "#" + user->getPhoneNumber() + "#" + user->getQqNumber() + "#" + user->getMail();
//		QByteArray datasend = data.toLocal8Bit();		//���ַ�ת����LOCAL8BIT������
//
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " data has sended:" <<datasend;
//			return;
//		}
//		return;
//	}
//
//	//������յ������޸��û���Ϣ
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
//	//��ѯ�ض��û����ļ�
//	else if (list[0] == "findFileByName") 
//	{
//		File *file = new File();   //��ʼ��һ��file
//		user->queryUserByName(list[1]);
//		QList<FileInfo> fileList = file->queryFilebyUser(user->getUserName());
//		QString dataFile;  //�����ݷ���ϵͳ��
//		
//		//�Ƚ�����Ϣ�ĸ������͹�ȥ
//		QString dataLenth = QString::number(fileList.size(), 10);
//		dataFile += dataLenth + "$";		//��Ϣ�ĵ�һλ���ļ�����
//
//		//��������ѭ�����͵��ͻ���
//		for (int i = 0;i < fileList.size();i++) {
//			//����Data��
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
//		QByteArray datasend = dataFile.toUtf8();   //ת��uft8���͵��ͻ���
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " file's data has sended:" << datasend;
//			return;
//		}
//		return;
//		//qDebug() << fileList.at(1).fileName;//���Է���
//	}
//	//��ѯ�����û����ļ�
//	else if (list[0] == "findFile")
//	{
//		File *file = new File();   //��ʼ��һ��file
//		//user->queryUserByName(list[1]);
//		//User *userFile = new User();  //������ѯ�ļ�������
//		QList<FileInfo> fileList = file->queryFile(list[1]);
//		QString dataFile;  //�����ݷ���ϵͳ��
//		QString dataLenth = QString::number(fileList.size(), 10);
//		dataFile += dataLenth + "$";		//��Ϣ�ĵ�һλ���ļ�����
//
//											//��������ѭ�����͵��ͻ���
//		for (int i = 0;i < fileList.size();i++) {
//			//����Data��
//			user->queryUserById(fileList.at(i).userId);  //ѭ����ѯ�û���id
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
//		QByteArray datasend = dataFile.toUtf8();   //ת��uft8���͵��ͻ���
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " file's data has sended:" << datasend;
//			return;
//		}
//		return;
//		//qDebug() << fileList.at(1).fileName;//���Է���
//	}
//	else if (list[0] == "uploadFile")   //������͵�upload��Ϣ
//	{
//		qDebug() << "change to receiveFile";
//		StotalSize = 0;
//		byteReceived = 0;
//		receiveTime = 0;
//		disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//		connect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
//
//		//receiveFile = new ReceiveFile();// ��ʼ������
//		//receiveFile->init();  //��ʼ���ļ�
//		//disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//		//connect(newtcpsocket, SIGNAL(readyRead()), receiveFile, SLOT(receive())); //����receiveFile�Ĳۺ���
//		return;
//	}
//	else if (list[0] == "downloadFile")
//	{
//		qDebug() << "user " << clientip << " begin to send file to client";
//		openFile(list[1]);
//		return;  //����û�мӷ��أ���������˵Ϊʲô���յ�downloadFile����Ϣ
//	}
//	//
//	else if (list[0] == "deleteFile")
//	{
//		qDebug() << "user " << clientip << "want to delete a file named:" << list[1];
//		deleteFile(list[1],list[2].toInt());
//		return;
//	}
//
//	//�ӷ���˷����ı����˲�ѯ
//	else if (list[0] == "QueryReporter")
//	{
//		user->queryUserByName(list[1]);
//		QString data = user->getTrueName() + "#" + user->getStudentId() + "#" + user->getLevel()
//			+ "#" + user->getTeacher() + "#" + QString::number(user->getUserId(), 10);
//		QByteArray datasend = data.toUtf8();		//���ַ�ת����LOCAL8BIT������
//
//		if (newtcpsocket->write(datasend))
//		{
//			qDebug() << "user " << clientip << " data has sended:" << datasend;
//			return;
//		}
//		return;
//	}
//	//������report�����뵽���ݿ�
//	else if (list[0] == "sendReport")
//	{
//		//QByteArray dataread = newtcpsocket->readAll();
//		//QString data = QString::fromUtf8(dataread);
//		//qDebug() << "the data from client: " << dataread;
//		//QStringList listNumber = data.split("#");
//		Report *report = new Report();
//
//		//�Ȳ�ѯһ�¸ñ����Ƿ����,һ��ֻ�ܷ�һ���ܱ�
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
//		//�������0���ѧ�ڸ����Ѿ������ܱ��������ظ��ύ
//		if (ReportList.size() > 0)
//		{
//			QString data = "ReportEdit#repeatTrue";
//			QByteArray datasend = data.toUtf8();		//���ַ�ת����LOCAL8BIT������
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
//		//����report���info
//		report->setReportInfo(ri);
//
//		if (report->insertReport())
//		{
//			qDebug() << "insert report success!";
//			QString data = "ReportEdit#true";
//			QByteArray datasend = data.toUtf8();		//���ַ�ת����LOCAL8BIT������
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
//			QByteArray datasend = data.toUtf8();		//���ַ�ת����LOCAL8BIT������
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
//	//���ձ����б�����
//	else if (list[0] == "lookReport")
//	{
//		Report *report = new Report();   //��ʼ��һ��Report
//		QList<ReportInfo> ReportList = report->queryReport(data);
//		//��������ǲ鿴�����ļ�
//		QString dataReport;  //�����ݷ���ϵͳ��
//
//		QString dataLenth = QString::number(ReportList.size(), 10);
//
//		////�����ѯ����1������Ҫ��$
//		//if(ReportList.size() > 0)
//			dataReport += dataLenth + "$";		//��Ϣ�ĵ�һλ���ļ�����
//												//��������ѭ�����͵��ͻ���
//		for (int i = 0;i < ReportList.size();i++) {
//				//����Data��
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
//		QByteArray datasend = dataReport.toUtf8();   //ת��uft8���͵��ͻ���
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
//	//���Ǽ���������֤�ģ�ǰ��Ķ���return���������ƥ����ߵ���һ����
//	QString sendData = list[0];
//	if (ret)
//		sendData += "#true";
//	else
//		sendData += "#false";
//	//�ְ����ݷ��͸��ͻ���
//	newtcpsocket->write(sendData.toUtf8());
//	qDebug() << "user " << clientip << " the data has send to client: " << sendData;
//
//	/*
//	if (connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData())))
//		qDebug() << "change to receiveData";*/
//}
//
////�����ļ�����
//void TcpServer::openFile(QString filename)
//{
//	//qDebug() << globalUserName << " will send a file to server";
//	loadSize = 0;
//	byteToWrite = 0;
//	//RtotalSize = 0;
//	sendTimes = 0;
//	outBlock.clear();
//	//���������
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
//	QString clientip = newtcpsocket->peerAddress().toString();  //�õ������ӿͻ��˵�IP
//
//	connect(newtcpsocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//
//
//	byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
//	qDebug() << "the file bytetowrite: " << byteToWrite;
//	StotalSize = localFile->size();
//
//	loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  
//
//	QDataStream out(&outBlock, QIODevice::WriteOnly);
//
//	//��ȡ�ļ�����
//	QString currentFileName = sendFileName.right(sendFileName.size() - sendFileName.lastIndexOf('/') - 1);
//
//	//ǰ���������ļ���С�ͷ����ļ�ͷ�Ĵ�С���������ļ������û���
//	out << qint64(0) << qint64(0) << currentFileName;
//
//	StotalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
//	byteToWrite += outBlock.size();
//	qDebug() << "the total bytetowrite: " << byteToWrite;
//	out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
//	out << StotalSize << qint64(outBlock.size());
//	//qDebug() << "the file head:" << outBlock;
//	newtcpsocket->write(outBlock);  //���������ļ���Ϣ���͵��׽��� 
//
//}
//
//void TcpServer::goOnSend(qint64 numBytes)
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());
//	QString clientip = newtcpsocket->peerAddress().toString();  //�õ����ӿͻ��˵�IP
//
//	sendTimes++;
//	qDebug() << "user " << clientip << " was " << sendTimes << " get in goOnSend";
//	qDebug() << "the numBytes: " << numBytes << "the loadSize:" << loadSize << "  left byteTowrite: " << byteToWrite;
//
//	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
//	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //���ʣ�����ݱ�ÿ�η��͵�С����ʣ���
//
//	newtcpsocket->write(outBlock);    //�������Ϣд��socket
//
//	if (byteToWrite == 0)  //�������  
//	{
//		//qDebug()<<QString::fromLocal8Bit("�ļ��������!");
//		disconnect(newtcpsocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//		sendTimes = 0;
//		qDebug() << "user " << clientip << "the file has sended";
//
//		localFile->close();  //�������ļ�Ҫ�رգ���Ȼ���ܶ��������һ������
//	}
//}
//
//
//
////�����ļ�����
//void TcpServer::receiveFile()
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //ÿ��������Ҫ�������
//	QString clientip = newtcpsocket->peerAddress().toString();  //�õ����ӿͻ��˵�IP
//
//	qDebug() << "user " << clientip << " was " << receiveTime << "get in receiveFile!!!";
//		if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
//		{
//			qDebug() << "receive the file of head";
//			//ui->receivedProgressBar->setValue(0);
//			QDataStream in(newtcpsocket);    //�������
//			QString globalUserName;
//			in >> RtotalSize >> byteReceived >> fileName >>globalUserName ;   //�����Ƕ������ֽڣ�
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
//			//���һ���ļ���д���ظ��Ĺ���
//			int i = 0;
//			QString suffix = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1); //��ȡ�ļ���׺;
//			QString fullName = fileName.left(fileName.size() - (fileName.size() - fileName.lastIndexOf('.'))); //��ȡ�ļ���
//			while (1)
//			{
//				QFile *existFile = new QFile(fileName);  //�����ж��Ƿ��ظ�
//				if (existFile->exists())   //����ļ�����
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
//		else  //��ʽ��ȡ�ļ�����    
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
//			newFile->flush();   //ֱ�ӽ�������ļ��ˣ�
//		}
//
//		if (byteReceived == RtotalSize)
//		{
//			qDebug() << "user " << clientip  << "receive is done";
//			//������������ݿ�����ļ���Ϣ
//			QDateTime dt;
//			QTime time;
//			QDate date;
//			dt.setTime(time.currentTime());
//			dt.setDate(date.currentDate());
//			QString currentDate = dt.toString("yyyy-MM-dd hh:mm");
//			File file;
//			file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
//			file.setFileSize(QString::number(RtotalSize,10));
//			file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //��ȡ�ļ���׺;
//			file.setFileTime(currentDate);
//			file.setUserId((int)user->getUserId());  //��ȡ��ȫ�����õ���userid
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
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //ÿ��������Ҫ�������
//	QString clientip = newtcpsocket->peerAddress().toString();  //�õ����ӿͻ��˵�IP
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
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //ÿ��������Ҫ�������
//	QString clientip = newtcpsocket->peerAddress().toString();  //�õ����ӿͻ��˵�IP
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
////����е��ߵľ���list��ȥ�����socket
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
//	//��������ʱ��������ߵ�����socket  
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
