#pragma once
#ifndef ACCOUNT_H_
#define ACCOUNT
#include "Database.h"
struct AccountInfo
{
	int accountId;
	int money;
	QString type;
	QString time;
	QString remark;
	QString name;
	int userId;
};
class Account
{
public:
	Account();
	~Account();
	AccountInfo getAccountInfo() { return accountInfo; }

	void setAccountInfo(AccountInfo ri) { accountInfo = ri; }
	bool insertAccount();

	bool deleteAccount(const int AccountId);


	//≤È—Ø±®∏Ê
	QList<AccountInfo> queryAccount(QString);

private:
	AccountInfo accountInfo;
};
#endif // !ACCOUNT_H_



