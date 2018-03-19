#include "DownloadRecord.h"



DownloadRecord::DownloadRecord()
{
}


DownloadRecord::~DownloadRecord()
{
}

bool DownloadRecord::insertRecord()
{

	QString sql = "insert into downloadrecord(sumBlock,breakPoint,fileId,userId)values(" +
		QString::number(record.rSumBlock, 10) + "," + QString::number(record.rBreakPoint) + "," +
		QString::number(record.fileId) + "," + QString::number(record.userId) + ")";
	//qDebug() << sql;
	QSqlQuery insert;
	insert.exec(sql);
	if (insert.isActive())
	{
		qDebug() << sql;
		return true;
	}
	return false;
}

bool DownloadRecord::deleteRecord(const int recordId)
{
	QString sql = "delete from downloadrecord where recordId = " + QString::number(recordId, 10);
	qDebug() << sql;
	QSqlQuery del;
	del.exec(sql);
	if (del.isActive())
	{
		qDebug() << sql;
		return true;
	}
	return false;
}
