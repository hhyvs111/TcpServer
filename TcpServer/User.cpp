#include "User.h"
//���캯��
User::User()
{
}
User::~User()
{

}

//�����û�����ѯ�û�
User& User::queryUserByName( const QString name)
{
	qDebug() << name;
	QString sql = "select * from user where userName = '"
		+ name + "'";
	QSqlQuery query;
	qDebug() << sql;
	query.exec(sql);
	if (query.next())
	{
		userId = query.value(0).toInt();
		//qDebug() << userId;
		userName = query.value(1).toString();
		userPassword = query.value(2).toString();
		studentId = query.value(3).toString();
		phoneNumber = query.value(4).toString();
		qqNumber = query.value(5).toString();
		mail = query.value(6).toString();
		trueName = query.value(7).toString();
		term = query.value(8).toString();
		level = query.value(9).toString();
		teacher = query.value(10).toString();
		return *this;
	}
	return *this;

	//return *this; ���￴�ǲ�����ø��ƹ��캯���ģ���Ҫ����ĺ�������ô�����
	//	T& fun()
	//{
	//	return *this;
	//}
	//���ص���������ã����û���ø��ƹ��캯��
	//	T fun()
	//{
	//	return *this;
	//}
	//����͵������ƹ��캯����
}
User& User::queryUserById(const int Id)
{
	qDebug() << Id;
	QString sql = "select * from user where userId = "
		+ QString::number(Id, 10);
	QSqlQuery query;
	qDebug()<<"the queryUserById :" << sql;
	query.exec(sql);
	if (query.next())
	{
		userId = query.value(0).toInt();
		//qDebug() << userId;
		userName = query.value(1).toString();
		userPassword = query.value(2).toString();
		studentId = query.value(3).toString();
		phoneNumber = query.value(4).toString();
		qqNumber = query.value(5).toString();
		mail = query.value(6).toString();
		trueName = query.value(7).toString();
		term = query.value(8).toString();
		level = query.value(9).toString();
		teacher = query.value(10).toString();
		return *this;
	}
	return *this;

}
bool User::updataUser()
{
	QString sql = "update user set studentId = '" + studentId + "',phoneNumber = '"
		+ phoneNumber + "',qqNumber = '" + qqNumber + "',mail = '" +
		mail + "',trueName = '" + trueName + "',term = '" + term + "',level = '"
		+level + "',teacher = '" +teacher+ "' where userName = '" + userName + "'";
	QSqlQuery query;
	query.exec(sql);
	if (query.isActive())
	{
		qDebug() << sql;
		return true;
	}
	return false;

}