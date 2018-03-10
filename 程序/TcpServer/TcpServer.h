#ifndef TCPSERVER_H
#define TCPSERVER_H
//#include "Global.h"
#include <QMainWindow>
#include <QtNetwork>
#include <QTimer>
#include <QFile>  
#include <QString> 
//#include <QFileDialog>
#include <QIODevice>
#include <QByteArray>
#include <QDataStream>
//#include "ReceiveFile.h"

#include "File.h"
#include "User.h"

//extern ReceiveFile *receiveFile;  //定义一个接收文件的指针。
//extern User *user;  

struct openFileStruct  
{  
    QString filePath;  
    QString fileName;  
};  
  
struct clientInfo   //客户端信息  
{  
    QString ip;     //ip  
    int state;      //状态  
    QString id;     //id  
};  

namespace Ui {
	class TcpServer;
}



class TcpServer : public QObject
{
	Q_OBJECT

public:
	explicit TcpServer(QObject *parent = 0);
	~TcpServer();

	bool checkLogin(QString sql);
	bool checkRegister(QString sql);
	bool checkUser(QString sql);

protected:
	void init();      // 初始化函数用于完成一些诸如信号与槽之间的联系和变量初始化工作

private slots:
	void acceptConnection();

	void receiveData();
	void receiveFile();
	void deleteFile(QString,int);
	void deleteReport(int);
	void slot_updateUserList();
	//send
	void openFile(QString);
	void sendFile();
	void goOnSend(qint64);
	void displayError(QAbstractSocket::SocketError);

private:
	Ui::TcpServer *ui;
	QTcpSocket *tcpSocket;
	QTcpServer *tcpServer;
	QMap<QString, QTcpSocket *> m_userlist;

	
	User *user;

	void startHeart();

	//心跳  
	QTimer *m_timer;

	//心跳时间  
	int m_hearttime;

	bool m_startHeartTime;
	
	//friend class ReceiveFile;  //声明友元

	//发送文件
	QFile *localFile;
	QString sendFileName;  //文件名  
	QByteArray outBlock;  //分次传  
	qint64 loadSize;  //每次发送数据的大小  
	qint64 byteToWrite;  //剩余数据大小  
	qint64 StotalSize;  //文件总大小 
	int sendTimes;  //用来标记是否为第一次发送，第一次以后连接信号触发，后面的则手动调用  

	//接收文件
	QFile *newFile;
	QByteArray inBlock;   // QByteArray可以存储raw bytes和传统的8-bits的字符串，都是以'\0'结尾的，使用比char*更方便
	QString fileName;
	qint64 RtotalSize;  //总共需要发送的文件大小（文件内容&文件名信息）  
	qint64 byteReceived;  //已经接收的大小  
	int receiveTime;	//接受的次数
};

#endif // TCPSERVER_H
