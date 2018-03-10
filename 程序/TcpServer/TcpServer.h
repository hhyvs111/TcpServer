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

//extern ReceiveFile *receiveFile;  //����һ�������ļ���ָ�롣
//extern User *user;  

struct openFileStruct  
{  
    QString filePath;  
    QString fileName;  
};  
  
struct clientInfo   //�ͻ�����Ϣ  
{  
    QString ip;     //ip  
    int state;      //״̬  
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
	void init();      // ��ʼ�������������һЩ�����ź����֮�����ϵ�ͱ�����ʼ������

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

	//����  
	QTimer *m_timer;

	//����ʱ��  
	int m_hearttime;

	bool m_startHeartTime;
	
	//friend class ReceiveFile;  //������Ԫ

	//�����ļ�
	QFile *localFile;
	QString sendFileName;  //�ļ���  
	QByteArray outBlock;  //�ִδ�  
	qint64 loadSize;  //ÿ�η������ݵĴ�С  
	qint64 byteToWrite;  //ʣ�����ݴ�С  
	qint64 StotalSize;  //�ļ��ܴ�С 
	int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ�����  

	//�����ļ�
	QFile *newFile;
	QByteArray inBlock;   // QByteArray���Դ洢raw bytes�ʹ�ͳ��8-bits���ַ�����������'\0'��β�ģ�ʹ�ñ�char*������
	QString fileName;
	qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	qint64 byteReceived;  //�Ѿ����յĴ�С  
	int receiveTime;	//���ܵĴ���
};

#endif // TCPSERVER_H
