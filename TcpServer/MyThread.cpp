#include"MyThread.h"
#include<QtNetwork>
MyThread::MyThread(qintptr socketDescriptor,QString mType, QObject *parent) :
	QThread(parent), socketDescriptor(socketDescriptor)
{
	qDebug() << "gou zao Worker Thread : " << QThread::currentThreadId();
	byteReceived = 0;
	receiveTime = 0;
	type = mType;
}

MyThread::~MyThread()
{
	requestInterruption();
	quit();
	wait();
	qDebug() <<"the Thread is closed! "<< currentThreadId();
}
//RUN完后线程自动关闭？
void MyThread::run()
{
	//互斥锁
	while (!isInterruptionRequested())
	{
		qDebug() << "sockID:"<<socketDescriptor 
			<< "start new Thread id is" << currentThreadId();

		tcpSocket = new QTcpSocket;
		if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
			emit error(tcpSocket->error());
			return;
		}
		//qDebug() << socketDescriptor;
		if (type == "upload")
		{
			connect(tcpSocket, SIGNAL(readyRead()),
				this, SLOT(receiveFile()), Qt::BlockingQueuedConnection);
		}
		else if (type == "download")
		{

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
		qDebug() << "receive the file of head";
		//ui->receivedProgressBar->setValue(0);
		QDataStream in(tcpSocket);    //从里面读
		//QString globalUserName;
		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //读的是二进制字节？

		//user = new User();
		/*user->queryUserByName(globalUserName);
		qDebug() << "userName is :" << user->getUserName();*/

		QByteArray change = fileName.toUtf8();
		fileName = "files/" + QString::fromUtf8(change);

		qDebug() << "the file of head: " << fileName;
		qDebug() << "totalSize: " << RtotalSize;
		qDebug() << " first byteReceived: " << byteReceived;

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
				qDebug() << "the fileName :" << fileName;
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
		qDebug() <<"sockId:"<<socketDescriptor<< "ThreadId: "<<currentThreadId() <<" the " << receiveTime << "recv " /*<< "the inBlock size is " << inBlock.size()*/
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
		User *user = new User();
		qDebug() << "name: " << globalUserName;
		user->queryUserByName(globalUserName);
		//qDebug() << user->getUserName();
		//qDebug() << globalUserName;
		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
		file.setFileSize(QString::number(RtotalSize, 10));
		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //获取文件后缀;
		file.setFileTime(currentDate);
		file.setUserId((int)user->getUserId());  //获取由全局名得到的userid

		qDebug() << "the file name:" << file.getFileName();
		qDebug() << "the file time is :" << file.getFileTime();
		qDebug() << "the userid :" << user->getUserId();
		//qDebug() << "the userid from file:" << file.getUserId();

		if (file.insertFile())
		{
			emit receiveDone();
			qDebug() << file.getFileName() << "insert file success!";
		}
		else
			qDebug() << "insert error!";
		inBlock.clear();
		byteReceived = 0;
		RtotalSize = 0;
		receiveTime = 0;
		newFile->close();
		//quit();  //退出该线程
		//
		//if (wait())
		//{
		//	qDebug() << "the Thread is closed!";
		//}
		
		/*disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		connect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));*/
	}
}