#include "TcpServer.h"
#include "MyTcpServer.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//TcpServer w;
	MyTcpServer *myTcpServer = new MyTcpServer();
	

	return a.exec();
}
