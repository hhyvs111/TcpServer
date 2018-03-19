#ifndef MYTHREAD_H_
#define MYTHREAD_H_
#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include"File.h"
#include "User.h"
 
 //�̳�QThread��TCP�����߳�
 //��Ҫ�����run()�麯���Ķ���
 //����һЩ��������������
 
class QFile;
class QTcpSocket;
 
class MyThread : public QThread
{
 Q_OBJECT
 public:
	MyThread(qintptr socketDescriptor,QString, QObject *parent = 0);
	MyThread(qintptr socketDescriptor, QString, QString,QString, QObject *parent = 0);
	MyThread(qintptr socketDescriptor, QString, QString,QString, QString, QObject *parent = 0);
	~MyThread();
 
	void run();
 
signals:
	void error(QTcpSocket::SocketError socketError);
	void bytesArrived(qint64,qint32,int);
	void receiveDone();
 
public slots:
	void receiveFile();
	void ThreadExit();

	//send
	bool openFile(QString);
	void sendFile();
	void goOnSend();
	void insertRecord(qint64, qint64);
	//void displayError(QAbstractSocket::SocketError);
 
private:
	int socketDescriptor;
	QString type;  //�߳����ͣ������ػ����ϴ�
	//qint64 bytesReceived; //�յ������ֽ�
	//qint64 byteToRead; //׼����ȡ���ֽ�
	//qint32 TotalBytes; //�ܹ�������ֽ�
	
	//QHostAddress fileName; //�ļ���
	//QFile *localFile;
	//QByteArray inBlock; //��ȡ����

	//�����ļ�
	QTcpSocket *tcpSocket;
	QFile *newFile;
	QByteArray inBlock;   // QByteArray���Դ洢raw bytes�ʹ�ͳ��8-bits���ַ�����������'\0'��β�ģ�ʹ�ñ�char*������
	QString fileName;
	qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	qint64 byteReceived;  //�Ѿ����յĴ�С  
	int receiveTime;	//���ܵĴ���

	QString globalUserName;
	//User *user;   //�����������ʼ������Ϊʵ���ϻ��������߳������ģ����Զ���̹߳�����ʱ����ܻ���ʳ�ͻ��

	//�����ļ�
	QFile *localFile;
	QString sendFileName;  //�ļ���  
	QByteArray outBlock;  //�ִδ�  
	qint64 loadSize;  //ÿ�η������ݵĴ�С  
	qint64 byteToWrite;  //ʣ�����ݴ�С  
	qint64 StotalSize;  //�ļ��ܴ�С 
	int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ����� 
	QString currentFileName;

	qint64 receiveStatus ;
	qint64 sumBlock;
	qint64 breakPoint;

	qint64 fileId;

 };
 
#endif // MYTHREAD_H_ 