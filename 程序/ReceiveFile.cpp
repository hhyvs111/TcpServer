//#include "ReceiveFile.h"
//
//
//void ReceiveFile::init()
//{
//	RtotalSize = 0;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
//	byteReceived = 0;  //�Ѿ����յĴ�С  
//	receiveTime = 0;	//���ܵĴ���
//	user = new User();
//}
//
//void ReceiveFile::receive()
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //ÿ��������Ҫ�������
//	QString clientip = newtcpsocket->peerAddress().toString();  //�õ����ӿͻ��˵�IP
//
//	qDebug() << "user " << clientip << " was " << receiveTime << "get in receiveFile!!!";
//	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
//	{
//		qDebug() << "receive the file of head";
//		//ui->receivedProgressBar->setValue(0);
//		QDataStream in(newtcpsocket);    //�������
//		QString globalUserName;
//		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //�����Ƕ������ֽڣ�
//		user->queryUserByName(globalUserName);
//		qDebug() << "user " << clientip << " name is :" << user->getUserName();
//
//		QByteArray change = fileName.toUtf8();
//		fileName = "files/" + QString::fromUtf8(change);
//
//		qDebug() << "the file of head: " << fileName;
//		qDebug() << "totalSize: " << RtotalSize;
//		qDebug() << " first byteReceived: " << byteReceived;
//		newFile = new QFile(fileName);
//		newFile->open(QFile::WriteOnly);
//	}
//	else  //��ʽ��ȡ�ļ�����    
//	{
//		receiveTime++;
//		qDebug() << "the " << receiveTime << " receive the file ";
//		inBlock = newtcpsocket->readAll();
//		qDebug() << "the inBlock size is " << inBlock.size();
//		qDebug() << "current receive the byte is :" << byteReceived;
//		//qDebug() << "the block: " << inBlock;
//		byteReceived += inBlock.size();
//
//		qDebug() << "begin to write";
//		newFile->write(inBlock);
//		newFile->flush();   //ֱ�ӽ�������ļ��ˣ�
//	}
//
//	if (byteReceived == RtotalSize)
//	{
//		qDebug() << "user " << clientip << "receive is done";
//		//������������ݿ�����ļ���Ϣ
//
//		File file;
//		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
//		file.setFileSize(QString::number(RtotalSize, 10));
//		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //��ȡ�ļ���׺;
//		file.setUserId((int)user->getUserId());  //��ȡ��ȫ�����õ���userid
//
//		qDebug() << "the file name:" << file.getFileName();
//		qDebug() << "the userid :" << user->getUserId();
//		qDebug() << "the userid from file:" << file.getUserId();
//
//		if (file.insertFile())
//		{
//			qDebug() << file.getFileName() << "insert file success!";
//		}
//		else
//			qDebug() << "insert error!";
//
//		inBlock.clear();
//		//byteReceived = 0;
//		//RtotalSize = 0;
//		//receiveTime = 0;
//		init();
//
//		newFile->close();
//		disconnect(newtcpsocket, SIGNAL(readyRead()), this, SLOT(receive()));
//		connect(newtcpsocket, SIGNAL(readyRead()), tcp, SLOT(receiveData()));
//	}
//}