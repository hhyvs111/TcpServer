#include"MyThread.h"
#include<QtNetwork>
MyThread::MyThread(qintptr socketDescriptor,QString mType, QObject *parent) :
	QThread(parent), socketDescriptor(socketDescriptor)
{
	//qDebug() << "gou zao Worker Thread : " << currentThreadId();
	byteReceived = 0;
	receiveTime = 0;
	type = mType;
}
MyThread::MyThread(qintptr socketDescriptor, QString mType,QString mFileName, QObject *parent) :
	QThread(parent), socketDescriptor(socketDescriptor)
{
	//qDebug() << "gou zao Worker Thread : " << currentThreadId();
	sendFileName = mFileName;
	type = mType;
}

MyThread::~MyThread()
{
	//requestInterruption();
	quit();
	wait();
	qDebug() << "receive sockID:" << socketDescriptor <<"the Thread is closed! "<< currentThreadId();
}
//RUN完后线程自动关闭？
void MyThread::run()
{
	//互斥锁
	//while (!isInterruptionRequested())
	{
		//qDebug() << "run sockID:"<<socketDescriptor 
		//	<< "start new Thread id is" << currentThreadId();

		tcpSocket = new QTcpSocket();
		if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
			emit error(tcpSocket->error());
			return;
		}
		//qDebug() << socketDescriptor;
		if (type == "upload")
		{
			qDebug() << "receive sockID:" << socketDescriptor << "connect signal success!";
			connect(tcpSocket, SIGNAL(readyRead()),
				this, SLOT(receiveFile()),Qt::DirectConnection);
		}
		else if (type == "download")
		{
			
			//不能用这个函数，因为可能socket的线程还是在构造函数里
			//打开文件后发送
			if (openFile(sendFileName))
			{
				connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)), Qt::DirectConnection);
				qDebug() << "send thread now" << currentThreadId();
				byteToWrite = localFile->size();  //剩余数据的大小  
				qDebug() << "the file bytetowrite: " << byteToWrite;
				StotalSize = localFile->size();

				loadSize = 4 * 1024;  //每次发送数据的大小  

				QDataStream out(&outBlock, QIODevice::WriteOnly);

				//获取文件名字
				currentFileName = sendFileName.right(sendFileName.size() - sendFileName.lastIndexOf('/') - 1);

				//前面两个是文件大小和发送文件头的大小，后面是文件名和用户名
				out << qint64(0) << qint64(0) << currentFileName;

				StotalSize += outBlock.size();  //总大小为文件大小加上文件名等信息大小  
				byteToWrite += outBlock.size();
				qDebug() << "the total bytetowrite: " << byteToWrite;
				out.device()->seek(0);  //回到字节流起点来写好前面连个qint64，分别为总大小和文件名等信息大小  
				out << StotalSize << qint64(outBlock.size());
				//qDebug() << "the file head:" << outBlock;
				tcpSocket->write(outBlock);  //将读到的文件信息发送到套接字 
			}
		}
		exec();
     	}

}
void MyThread::ThreadExit()
{
	//exit();
	//qDebug() << "this Thread is closed!";
	//if (this->wait())
	//{
	//	qDebug() << "this Thread is closed!";
	//}
	this->~MyThread();
}

void MyThread::receiveFile()
{
	if (byteReceived == 0)  //才刚开始接收数据，此数据为文件信息  
	{
		qDebug() << "receive sockID:" << socketDescriptor
			<< "start new Thread id is" << currentThreadId();

		QDataStream in(tcpSocket);    //从里面读

		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //读的是二进制字节？

		fileName = "files/" + fileName;

		qDebug() <<"receive sockID:" << socketDescriptor << "the filename of head: " << fileName
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
				qDebug() << "receive sockID:" << socketDescriptor << "the fileName :" << fileName;
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
		qDebug() <<"sockId:"<<socketDescriptor<< "ThreadId: "<<currentThreadId() << fileName<<" the " << receiveTime << "recv " /*<< "the inBlock size is " << inBlock.size()*/
			<< "current receive the byte is :" << byteReceived <<" and the total " << RtotalSize;

		inBlock = tcpSocket->readAll();
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
		QString currentDate = dt.toString("yyyy-MM-dd hh:mm");
		File file;
		User user;
		qDebug() << "receive sockID:" << socketDescriptor << "globalUserName: " << globalUserName;
		user.queryUserByName(globalUserName);
		//qDebug() << user->getUserName();
		//qDebug() << globalUserName;
		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
		file.setFileSize(QString::number(RtotalSize, 10));
		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //获取文件后缀;
		file.setFileTime(currentDate);
		file.setUserId((int)user.getUserId());  //获取由全局名得到的userid

		qDebug() << "receive sockID:" << socketDescriptor << "the file name:" << file.getFileName() << "the file time is :" << file.getFileTime()
			<< "the userid :" << user.getUserId();
		//qDebug() << "the userid from file:" << file.getUserId();

		if (file.insertFile())
		{
			inBlock.clear();
			byteReceived = 0;
			RtotalSize = 0;
			receiveTime = 0;
			newFile->close();
			
			qDebug() << "receive sockID:" << socketDescriptor << file.getFileName() << "insert file success!";
		}
		else
			qDebug() << "receive sockID:" << socketDescriptor << "insert error!";
		emit receiveDone();
	}
}

bool MyThread::openFile(QString filename)
{
	//qDebug() << globalUserName << " will send a file to server";
	loadSize = 0;
	byteToWrite = 0;
	//RtotalSize = 0;
	sendTimes = 0;
	outBlock.clear();
	//这个不能用
	sendFileName = filename;
	localFile = new QFile("files/"+filename);
	if (localFile->open(QFile::ReadOnly))
	{
		qDebug() << "open file:" << filename << " success!";
		qDebug() << "thread now" << currentThreadId();
		//sendFile();
		return true;
	}
	else
	{
		qDebug() << "open file failed!";
		return false;
	}

}

//发送文件头
void MyThread::sendFile()
{

	qDebug() << "send thread now" << currentThreadId();
	byteToWrite = localFile->size();  //剩余数据的大小  
	qDebug() << "the file bytetowrite: " << byteToWrite;
	StotalSize = localFile->size();

	loadSize = 4 * 1024;  //每次发送数据的大小  

	QDataStream out(&outBlock, QIODevice::WriteOnly);

	//获取文件名字
    currentFileName = sendFileName.right(sendFileName.size() - sendFileName.lastIndexOf('/') - 1);

	//前面两个是文件大小和发送文件头的大小，后面是文件名和用户名
	out << qint64(0) << qint64(0) << currentFileName;

	StotalSize += outBlock.size();  //总大小为文件大小加上文件名等信息大小  
	byteToWrite += outBlock.size();
	qDebug() << "the total bytetowrite: " << byteToWrite;
	out.device()->seek(0);  //回到字节流起点来写好前面连个qint64，分别为总大小和文件名等信息大小  
	out << StotalSize << qint64(outBlock.size());
	//qDebug() << "the file head:" << outBlock;
	tcpSocket->write(outBlock);  //将读到的文件信息发送到套接字 
}

void MyThread::goOnSend(qint64 numBytes)
{
	//QTcpSocket *tcpSocket = static_cast<QTcpSocket*>(this->sender());
	//QString clientip = tcpSocket->peerAddress().toString();  //得到连接客户端的IP

	sendTimes++;
	qDebug() << " threadId:" << QThread::currentThreadId() <<"sockID:"<<socketDescriptor
		<< " "<<currentFileName <<  "the " << sendTimes << "the numBytes: " << numBytes << "the loadSize:" << loadSize
		<< "  left byteTowrite: " << byteToWrite;

	byteToWrite -= numBytes;  //剩余数据大小  
	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //如果剩余数据比每次发送的小则发送剩余的

	tcpSocket->write(outBlock);    //将这个信息写入socket

	if (byteToWrite == 0)  //发送完毕  
	{
		//qDebug()<<QString::fromLocal8Bit("文件发送完毕!");
		disconnect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		sendTimes = 0;
		qDebug() << " threadId:" << currentThreadId() 
			<< "sockID:" << socketDescriptor << currentFileName<< "the file has sended";
		
		localFile->close();  //发送完文件要关闭，不然不能对其进行下一步操作
		emit receiveDone();
	}
}