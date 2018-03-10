//#include "ReceiveFile.h"
//
//
//void ReceiveFile::init()
//{
//	RtotalSize = 0;  //总共需要发送的文件大小（文件内容&文件名信息）  
//	byteReceived = 0;  //已经接收的大小  
//	receiveTime = 0;	//接受的次数
//	user = new User();
//}
//
//void ReceiveFile::receive()
//{
//	QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());  //每个函数都要用这个？
//	QString clientip = newtcpsocket->peerAddress().toString();  //得到连接客户端的IP
//
//	qDebug() << "user " << clientip << " was " << receiveTime << "get in receiveFile!!!";
//	if (byteReceived == 0)  //才刚开始接收数据，此数据为文件信息  
//	{
//		qDebug() << "receive the file of head";
//		//ui->receivedProgressBar->setValue(0);
//		QDataStream in(newtcpsocket);    //从里面读
//		QString globalUserName;
//		in >> RtotalSize >> byteReceived >> fileName >> globalUserName;   //读的是二进制字节？
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
//	else  //正式读取文件内容    
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
//		newFile->flush();   //直接进入接收文件了？
//	}
//
//	if (byteReceived == RtotalSize)
//	{
//		qDebug() << "user " << clientip << "receive is done";
//		//接收完成向数据库插入文件信息
//
//		File file;
//		file.setFileName(fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1));
//		file.setFileSize(QString::number(RtotalSize, 10));
//		file.setFileType(fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)); //获取文件后缀;
//		file.setUserId((int)user->getUserId());  //获取由全局名得到的userid
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