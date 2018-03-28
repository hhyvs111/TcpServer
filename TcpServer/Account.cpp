#include "Account.h"



Account::Account()
{
}


Account::~Account()
{
}

bool Account::insertAccount()
{
	QString sql = "insert into account (money,type,time,remark,userId)values(" +
		QString::number(accountInfo.money) + ",'" + accountInfo.type + "','" + accountInfo.time
		+ "','" + accountInfo.remark + "'," + QString::number(accountInfo.userId) + ");";
	qDebug() << sql;
	QSqlQuery insert;
	insert.exec(sql);
	if (insert.isActive())
	{
		qDebug() << sql;
		return true;
	}
	return false;
}

QList<AccountInfo> Account::queryAccount(QString data)
{
	QStringList datalist = data.split("#");  //”√#∑÷∏Ó
	QList<AccountInfo> list;
	QSqlQuery query;
	QString sql1;
	if (datalist[1] == "all")
	{
		sql1 = "select * from account t1 left join user t2 on t1.userId = t2.userId;";
	}
	query.exec(sql1);
	qDebug() << sql1;
	while (query.next())
	{
		AccountInfo ac;
		ac.accountId = query.value(0).toInt();
		ac.money = query.value(1).toInt();
		ac.type = query.value(2).toString();
		ac.time = query.value(3).toString();
		ac.remark = query.value(4).toString();
		ac.userId = query.value(5).toInt();
		ac.name = query.value(13).toString();
		list.append(ac);
	}
	return list;
}