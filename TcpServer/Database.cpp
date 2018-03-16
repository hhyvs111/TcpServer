//#include <QtSql>
//#include <QSqlDatabase>
//#include <QDebug>
#include "Database.h" 
bool isOpen = false;
MySql::MySql(QObject *parent)
	:QObject(parent)
{
	dbDriver = "QMYSQL";
	dbName = "filemanagement";
	userNa = "root";
	userPwd = "123456";
	hostName = "127.0.0.1";
	hostPort = 5432;
}

bool MySql::connDB()
{
		if (isOpen)
			return true;
		QSqlDatabase db;
		if (QSqlDatabase::contains("qt_sql_default_connection"))
			db = QSqlDatabase::database("qt_sql_default_connection");
		else
			db = QSqlDatabase::addDatabase(dbDriver);
		db.setHostName(hostName);//����������
		db.setDatabaseName(dbName);//�������ݿ���
		db.setUserName(userNa);//�����û���
		db.setPassword(userPwd);//�����û�����
		
			if (db.open())
			{
				qDebug() << "Database connected successfully!";
				return true;
				isOpen = true;
			}
			else
			{
				qDebug() << "Database connected failed!";
				return false;
				isOpen = false;
			}
		
}

bool MySql::queryDB(const QString &Sql)
{
	qDebug() << Sql;
	QSqlQuery query;
	query.exec(Sql);
	if (query.next())
		//����м�¼��Ϊ�� �����˳��ж�
	{
		qDebug() << query.value(0).toString();
		return true;
	}
	return false;
}

bool MySql::insertDB(const QString &Sql)
{
	QSqlQuery insert;
	insert.exec(Sql);
	if (insert.isActive())
	{
		qDebug() << Sql;
		return true;
	}
	return false;
}
