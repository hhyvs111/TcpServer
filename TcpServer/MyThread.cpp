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
//RUN����߳��Զ��رգ�
void MyThread::run()
{
	//������
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
			
			//�����������������Ϊ����socket���̻߳����ڹ��캯����
			//���ļ�����
			if (openFile(sendFileName))
			{
				connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)), Qt::DirectConnection);
				qDebug() << "send thread now" << currentThreadId();
				byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
				qDebug() << "the file bytetowrite: " << byteToWrite;
				StotalSize = localFile->size();

				loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  

				QDataStream out(&outBlock, QIODevice::WriteOnly);

				//��ȡ�ļ�����
				currentFileName = sendFileName.right(sendFileName.size() - sendFileName.lastIndexOf('/') - 1);

				//ǰ���������ļ���С�ͷ����ļ�ͷ�Ĵ�С���������ļ������û���
				out << qint64(0) << qint64(0) << currentFileName;

				StotalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
				byteToWrite += outBlock.size();
				qDebug() << "the total bytetowrite: " << byteToWrite;
				out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
				out << StotalSize << qint64(outBlock.size());
				//qDebug() << "the file head:" << outBlock;
				tcpSocket->write(outBlock);  //���������ļ���Ϣ���͵��׽��� 
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
	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
	{
		qDebug() << "receive sockID:" << socketDescriptor
			<< "start new Thread id is" << currentThreadId();

		QDataStream in(tcpSocket);    //�������

		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //�����Ƕ������ֽڣ�

		fileName = "files/" + fileName;

		qDebug() <<"receive sockID:" << socketDescriptor << "the filename of head: " << fileName
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
				qDebug() << "receive sockID:" << socketDescriptor << "the fileName :" << fileName;
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
		qDebug() <<"sockId:"<<socketDescriptor<< "ThreadId: "<<currentThreadId() << fileName<<" the " << receiveTime << "recv " /*<< "the inBlock size is " << inBlock.size()*/
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
		User user;
		qDebug() << "receive sockID:" << socketDescriptor << "globalUserName: " << globalUserName;
		user.queryUserByName(globalUserName);
		//qDebug() << user->getUserName();
		//qDebug() << globalUserName;
		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
		file.setFileSize(QString::number(RtotalSize, 10));
		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //��ȡ�ļ���׺;
		file.setFileTime(currentDate);
		file.setUserId((int)user.getUserId());  //��ȡ��ȫ�����õ���userid

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
	//���������
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

//�����ļ�ͷ
void MyThread::sendFile()
{

	qDebug() << "send thread now" << currentThreadId();
	byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
	qDebug() << "the file bytetowrite: " << byteToWrite;
	StotalSize = localFile->size();

	loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  

	QDataStream out(&outBlock, QIODevice::WriteOnly);

	//��ȡ�ļ�����
    currentFileName = sendFileName.right(sendFileName.size() - sendFileName.lastIndexOf('/') - 1);

	//ǰ���������ļ���С�ͷ����ļ�ͷ�Ĵ�С���������ļ������û���
	out << qint64(0) << qint64(0) << currentFileName;

	StotalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
	byteToWrite += outBlock.size();
	qDebug() << "the total bytetowrite: " << byteToWrite;
	out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
	out << StotalSize << qint64(outBlock.size());
	//qDebug() << "the file head:" << outBlock;
	tcpSocket->write(outBlock);  //���������ļ���Ϣ���͵��׽��� 
}

void MyThread::goOnSend(qint64 numBytes)
{
	//QTcpSocket *tcpSocket = static_cast<QTcpSocket*>(this->sender());
	//QString clientip = tcpSocket->peerAddress().toString();  //�õ����ӿͻ��˵�IP

	sendTimes++;
	qDebug() << " threadId:" << QThread::currentThreadId() <<"sockID:"<<socketDescriptor
		<< " "<<currentFileName <<  "the " << sendTimes << "the numBytes: " << numBytes << "the loadSize:" << loadSize
		<< "  left byteTowrite: " << byteToWrite;

	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //���ʣ�����ݱ�ÿ�η��͵�С����ʣ���

	tcpSocket->write(outBlock);    //�������Ϣд��socket

	if (byteToWrite == 0)  //�������  
	{
		//qDebug()<<QString::fromLocal8Bit("�ļ��������!");
		disconnect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		sendTimes = 0;
		qDebug() << " threadId:" << currentThreadId() 
			<< "sockID:" << socketDescriptor << currentFileName<< "the file has sended";
		
		localFile->close();  //�������ļ�Ҫ�رգ���Ȼ���ܶ��������һ������
		emit receiveDone();
	}
}