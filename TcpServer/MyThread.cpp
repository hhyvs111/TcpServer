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
//RUN����߳��Զ��رգ�
void MyThread::run()
{
	//������
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
	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
	{
		qDebug() << "receive the file of head";
		//ui->receivedProgressBar->setValue(0);
		QDataStream in(tcpSocket);    //�������
		//QString globalUserName;
		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //�����Ƕ������ֽڣ�

		//user = new User();
		/*user->queryUserByName(globalUserName);
		qDebug() << "userName is :" << user->getUserName();*/

		QByteArray change = fileName.toUtf8();
		fileName = "files/" + QString::fromUtf8(change);

		qDebug() << "the file of head: " << fileName;
		qDebug() << "totalSize: " << RtotalSize;
		qDebug() << " first byteReceived: " << byteReceived;

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
				qDebug() << "the fileName :" << fileName;
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
		qDebug() <<"sockId:"<<socketDescriptor<< "ThreadId: "<<currentThreadId() <<" the " << receiveTime << "recv " /*<< "the inBlock size is " << inBlock.size()*/
			<< "current receive the byte is :" << byteReceived <<" and the total " << RtotalSize;

		inBlock = tcpSocket->readAll();
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
		QString currentDate = dt.toString("yyyy-MM-dd hh:mm");
		File file;
		User *user = new User();
		qDebug() << "name: " << globalUserName;
		user->queryUserByName(globalUserName);
		//qDebug() << user->getUserName();
		//qDebug() << globalUserName;
		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
		file.setFileSize(QString::number(RtotalSize, 10));
		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //��ȡ�ļ���׺;
		file.setFileTime(currentDate);
		file.setUserId((int)user->getUserId());  //��ȡ��ȫ�����õ���userid

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
		//quit();  //�˳����߳�
		//
		//if (wait())
		//{
		//	qDebug() << "the Thread is closed!";
		//}
		
		/*disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		connect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));*/
	}
}