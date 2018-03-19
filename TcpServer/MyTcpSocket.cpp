#include "MyTcpSocket.h"
#include "MyThread.h"
#include "Report.h"
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
		qDebug() << socketDescriptor<< "disconnect";
		emit sockDisConnect(socketID, this->peerAddress().toString(), this->peerPort(), QThread::currentThread());//���ͶϿ����ӵ��û���Ϣ
		this->deleteLater();
	});
	connect(&watcher, &QFutureWatcher<QByteArray>::finished, this, &MyTcpSocket::startNext);
	connect(&watcher, &QFutureWatcher<QByteArray>::canceled, this, &MyTcpSocket::startNext);
	socketDescriptor1 = socketDescriptor;
	qDebug() << "new connect"<< "socketId:" << socketDescriptor1 << "threadId: " << QThread::currentThreadId();
	//����ֻҪ�����������Ӿ���һ���µ��̡߳�
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
	qDebug() <<"socketId:"<<socketDescriptor1<< data;	//����������Ϣ
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
	qDebug() << dataread;
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
			sendData += "#false";
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
			sendData += "#false";
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
			sendData += "#false";
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
		disconnect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::readData);
		connect(this, SIGNAL(readyRead()),
			this, SLOT(receiveFile()), Qt::DirectConnection);
		byteReceived = 0;
		receiveTime = 0;
		//socketDescriptor���������MyThread
		//MyThread *uploadThread = new MyThread(socketDescriptor1,"upload",this);

		//uploadThread->setObjectName("receiveThread");
		////������ɾ��
		//connect(uploadThread, SIGNAL(receiveDone()), uploadThread, SLOT(ThreadExit()));
		//connect(uploadThread, SIGNAL(finished()), uploadThread, SLOT(deleteLater()));
		//uploadThread->start();


		//qDebug()<<"the main thread:"<< QThread::currentThreadId();
		return "N";
	}
	//�����ļ�
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
		//QByteArray dataread = list[1].toUtf8();  //��ת��Ϊ�ֽ���
		//qDebug() << dataread;
		////�������ƣ���дin������ṹ��
		//QBuffer buf(&dataread);
		//buf.open(QIODevice::ReadOnly);
		//QDataStream in(&buf);

		//ReportInfo reportInfo;
		////������������д��
		//in >> reportInfo;

		Report *report = new Report();
		//�Ȳ�ѯһ�¸ñ����Ƿ����,һ��ֻ�ܷ�һ���ܱ�
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

		//�������0���ѧ�ڸ����Ѿ������ܱ��������ظ��ύ
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

		//����report���info
		report->setReportInfo(ri);

		//�������ɹ�
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
		Report *report = new Report();   //��ʼ��һ��Report
		QList<ReportInfo> ReportList = report->queryReport(data);
		//��������ǲ鿴�����ļ�
		QString dataReport;  //�����ݷ���ϵͳ��
		QString dataLenth = QString::number(ReportList.size(), 10);
		
		////�����ѯ����1������Ҫ��$
		//if(ReportList.size() > 0)
			dataReport += dataLenth + "$";		//��Ϣ�ĵ�һλ���ļ�����
												//��������ѭ�����͵��ͻ���
		for (int i = 0;i < ReportList.size();i++) {
				//����Data��
		
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
		
		return dataReport.toUtf8();   //ת��uft8���͵��ͻ���

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
	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
	{
		qDebug() << "receive sockID:" << socketDescriptor1
			<< "start new Thread id is" << QThread::currentThreadId();

		QDataStream in(this);    //�������

		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //�����Ƕ������ֽڣ�

		fileName = "files/" + fileName;

		qDebug() << "receive sockID:" << socketDescriptor1 << "the filename of head: " << fileName
			<< "totalSize: " << RtotalSize << " first byteReceived: " << byteReceived;


		//���һ���ļ���д���ظ��Ĺ���
		int i = 0;
		QString suffix = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1); //��ȡ�ļ���׺;
		QString fullName = fileName.left(fileName.size() - (fileName.size() - fileName.lastIndexOf('.'))); //��ȡ�ļ���
		while (1)
		{
			QFile *existFile = new QFile(fileName);  //�����ж��Ƿ��ظ�
			if (existFile->exists())   //����ļ�����
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
	else  //��ʽ��ȡ�ļ�����    
	{
		receiveTime++;
		qDebug() << "sockId:" << socketDescriptor1 << "ThreadId: " << QThread::currentThreadId() << fileName << " the " << receiveTime << "recv " /*<< "the inBlock size is " << inBlock.size()*/
			<< "current receive the byte is :" << byteReceived << " and the total " << RtotalSize;

		inBlock = this->readAll();
		byteReceived += inBlock.size();
		newFile->write(inBlock);
		newFile->flush();   //ֱ�ӽ�������ļ�
	}

	if (byteReceived == RtotalSize)
	{
		//qDebug() << "user " << clientip << "receive is done";
		//������������ݿ�����ļ���Ϣ
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
			//���û��
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
		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //��ȡ�ļ���׺;
		file.setFileTime(currentDate);
		file.setUserId((int)user->getUserId());  //��ȡ��ȫ�����õ���userid

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