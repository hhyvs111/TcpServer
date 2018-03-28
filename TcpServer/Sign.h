#pragma once
#ifndef SIGN_H_
#define SIGN_H_
#include "Database.h"
struct SignInfo
{
	int SignId;
	QString SignTime;
	QString userName;
	int userId;

};
class Sign
{
public:
	Sign();
	~Sign();
	SignInfo getSignInfo() { return signInfo; }

	void setSignInfo(SignInfo ri) { signInfo = ri; }
	bool insertSign();
	bool deleteSign(const int SignId);
	QList<SignInfo> querySign(QString);

private:
	SignInfo signInfo;
};
#endif // !SIGN_H_



