#pragma once
#ifndef DATABASE_H_
#define DATABASE_H_

#include <QObject>
#include <QSql>  
#include <QSqlDatabase>  
#include <QSqlQuery>  
#include <QDebug>  

class MySql : public QObject
{
public:
	MySql(QObject *parent = 0);

	//连接数据库
	 bool connDB();
	//查询数据库
	static bool queryDB(const QString &Sql);
	static bool insertDB(const QString &Sql);

private:
	QString dbDriver;
	QString dbName;
	QString userNa;
	QString userPwd;
	QString hostName;
	int hostPort;
	//QSqlDatabase db;
	
};
#endif // !DATABASE_H_
