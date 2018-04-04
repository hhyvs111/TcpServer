#include "Sign.h"



Sign::Sign()
{
}


Sign::~Sign()
{
}
bool Sign::insertSign()
{
	QString sql = "insert into sign(signTime,userId)values('" +
		signInfo.SignTime + "'," + QString::number(signInfo.userId)+ ");";
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

//查询传入用户Id就行
QList<SignInfo> Sign::querySign(QString data)
{
	QStringList datalist = data.split("#");  //用#分割
	QList<SignInfo> list;
	QSqlQuery query;
	QString sql1;
	if (datalist[1] == "all")
	{
		sql1 = "select * from sign t1 left join user t2 on t1.userId = t2.userId order by t1.SignTime desc;";
	}
	else if (datalist[1] == "user")
	{
		sql1 = "select * from sign t1 left join user t2 on t1.userId = t2.userId where t2.userName = '" +
			datalist[2] + "';";
	}
	//这里判断是否重复签到
	else if (datalist[0] == "repeat")
	{
		sql1 = "select * from sign t1 left join user t2 on t1.userId = t2.userId where t2.userName = '" + 
			datalist[1] + "' and t1.SignTime like '" + datalist[2] + "%';";
	}
	else if (datalist[0] == "QueryMySign")
	{
		sql1 = "select * from sign t1 left join user t2 on t1.userId = t2.userId where t2.userName = '" +
			datalist[1] +"';";
	}
	query.exec(sql1);
	qDebug() << sql1;
	while (query.next())
	{
		SignInfo s;
		s.SignId = query.value(0).toInt();
		s.SignTime = query.value(1).toString();
		s.userId = query.value(3).toInt();
		s.userName = query.value(10).toString();
		list.append(s);
	}
	return list;
}