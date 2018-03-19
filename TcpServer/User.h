#pragma once
#ifndef USER_H_
#define USER_H_
#include <QObject>
#include "Database.h"


//¸ãÒ»¸ö»¥³â

class User /*: public MySql*/
{
public:
	User();
	~User();
	//get
	QString getUserName() { return userName; }
	QString getUserPassword() { return userPassword; }
	QString getPhoneNumber() { if (phoneNumber.isEmpty()) return "null"; return phoneNumber; }
	QString getStudentId() { if (studentId.isEmpty()) return "null"; return studentId; }
	QString getQqNumber() { if (qqNumber.isEmpty()) return "null"; return qqNumber; }
	QString getMail() { if (mail.isEmpty()) return "null"; return mail; }
	QString getTrueName() { if (trueName.isEmpty()) return "null"; return trueName; }
	QString getTerm() { if (term.isEmpty()) return "null"; return term; }
	QString getLevel() { if (level.isEmpty()) return "null"; return level; }
	QString getTeacher() { if (teacher.isEmpty()) return "null"; return teacher; }
	int getUserId() { return userId; }

	//set
	void setUserName(QString un) { userName = un; }
	void setUserPassword(QString up) { userPassword = up; }
	void setPhoneNumber(QString ph) { phoneNumber = ph; }
	void setStudentId(QString si) { studentId = si; }
	void setQqNumber(QString qq) { qqNumber = qq; }
	void setMail(QString m) { mail = m; }
	void setUserId(int id) { userId = id; }
	void setTrueName(QString nn) { trueName = nn; }
	void setTerm(QString te) { term = te; }
	void setLevel(QString le) { level = le; }
	void setTeacher(QString te) { teacher = te; }
	

	User& queryUserByName(QString name);
	User& queryUserById(int);
	bool updataUser();
	

private:
	int userId;
	QString userName;
	QString userPassword;
	QString studentId;
	QString phoneNumber;
	QString qqNumber;
	QString mail;
	QString trueName;
	QString term;
	QString level;
	QString teacher;

};
#endif // USER_H_
