#pragma once
#ifndef DOWNLOADRECORD_H_
#define DOWNLOARRECORD_H_
#include "Database.h"
#include<QDataStream>
#include<QList>

struct Record
{
	int rId;
	QString rFileName;
	QString rFileSize;
	QString rFilePath;
	qint64 rSumBlock;
	qint64 rBreakPoint;
	int fileId;
	int userId;
};
class DownloadRecord
{
public:
	DownloadRecord();
	~DownloadRecord();

	Record getRecord() { return record; }

	void setRecord(Record ri) { record = ri; }

	bool insertRecord();

	bool deleteRecord(const int ReportId);
private:
	Record record;
};
#endif // !DOWNLOADRECORD_H_



