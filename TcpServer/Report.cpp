#include "Report.h"

Report::Report()
{

}
Report::~Report()
{

}

bool Report::insertReport()
{
	QString sql = "insert into report (reportTerm,reportWeek,reportTime,reportMain,reportQuestionForSolve,reportQuestionHasSolved,reportQuestionFromLastWeek,reportYourGet,reportUserId)values('" 
		+ reportInfo.reportTerm + "','" + 
		reportInfo.reportWeek + "','" + 
		reportInfo.reportTime + "','" + 
		reportInfo.reportMain + "','" + 
		reportInfo.reportQuestionForSolve + "','" +
		reportInfo.reportQuestionHasSolved + "','" +
		reportInfo.reportQuestionFromLastWeek + "','" +
		reportInfo.reportYourGet + "'," +
		QString::number(reportInfo.reportUserId, 10) + ")";
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

bool Report::deleteReport(const int reportId)
{
	QString sql = "delete from report where reportId = " + QString::number(reportId, 10);
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

//QList<ReportInfo> Report::queryAllReport()
//{
//	QList<ReportInfo> list;
//	QSqlQuery query;
//	//左连接 只查表相关的数据
//
//	query.exec(sql1);
//	while (query.next())
//	{
//		//不能直接用[i]
//		ReportInfo ri;
//		ri.reportId = query.value(0).toInt();
//		ri.reportTerm = query.value(1).toString();
//		ri.reportWeek = query.value(2).toString();
//		ri.reportTime = query.value(3).toString();
//		ri.reportMain = query.value(4).toString();
//		ri.reportQuestionForSolve = query.value(5).toString();
//		ri.reportQuestionHasSolved = query.value(6).toString();
//		ri.reportQuestionFromLastWeek = query.value(7).toString();
//		ri.reportYourGet = query.value(8).toString();
//		ri.reportUserId = query.value(9).toInt();
//		ri.reportUserName = query.value(11).toString();
//		ri.reportName = query.value(17).toString();
//		ri.reportLevel = query.value(19).toString();
//
//		list.append(ri);
//	}
//	return list;
//}




//查询真名的所有报告
QList<ReportInfo> Report::queryReport(QString data)
{
	QStringList datalist = data.split("#");  //用#分割
	QList<ReportInfo> list;
	QSqlQuery query;
	QString sql1;
	//左连接 只查表相关的数据
	if (datalist[1] == "all")
	{
		sql1 = " select * from report t1 left join  user t2 on t1.reportUserId = t2.userId; ";
	}
	//根据真名其报告
	else if (datalist[1] == "TrueName")
	{
	 sql1 = " select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t2.trueName = '"
			+ datalist[2] + "';";
	}
	else if (datalist[1] == "UserName")
	{
		sql1 = " select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t2.userName = '"
			+ datalist[2] + "';";
	}
	else if (datalist[1] == "reportId")
	{
		sql1 = "select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t1.reportId = "
			+ datalist[2];
	}
	//把这个周和学期还有姓名发过来 1#reportRepeat#reportUserId#reportTerm#reportWeek
	else if (datalist[1] == "reportRepeat")
	{
		sql1 = "select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t1.reportUserId = "
			+ datalist[2] + " and t1.reportTerm = '" 
			+ datalist[3] + "' and t1.reportWeek = '" 
			+ datalist[4] + "';";
	}
	//学期和周数
	else if (datalist[1] == "TW")
	{
		sql1 = "select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t1.reportTerm = '"
			+ datalist[2] + "' and t1.reportWeek = '"
			+ datalist[3] + "';";
	}
	else if (datalist[1] == "TWC")
	{
		sql1 = "select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t1.reportTerm = '"
			+ datalist[2] + "' and t1.reportWeek = '"
			+ datalist[3] + "' and t1.reportMain like '%"
			+ datalist[4] + "%';";
	}
	else if (datalist[1] == "TAW")
	{
		sql1 = "select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t1.reportTerm = '"
			+ datalist[2] + "';";
	}
	else if (datalist[1] == "TAWC")
	{
		sql1 = "select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t1.reportTerm = '"
			+ datalist[2] + "' and t1.reportMain like '%"
			+ datalist[3] + "';";
	}

	query.exec(sql1);
	qDebug() << sql1;
	while (query.next())
	{
		//不能直接用[i]
		ReportInfo ri;
		ri.reportId = query.value(0).toString();
		ri.reportTerm = query.value(1).toString();
		ri.reportWeek = query.value(2).toString();
		ri.reportTime = query.value(3).toString();
		ri.reportMain = query.value(4).toString();
		ri.reportQuestionForSolve = query.value(5).toString();
		ri.reportQuestionHasSolved = query.value(6).toString();
		ri.reportQuestionFromLastWeek = query.value(7).toString();
		ri.reportYourGet = query.value(8).toString();
		ri.reportUserId = query.value(9).toInt();
		ri.reportUserName = query.value(11).toString();
		ri.reportName = query.value(17).toString();
		ri.reportLevel = query.value(19).toString();
		list.append(ri);
	}
	return list;
}

QString Report::queryLastWeek(QString data)
{
	QStringList datalist = data.split("#");  //用#分割
	QList<ReportInfo> list;
	QSqlQuery query;
	QString sql1;
	if (datalist[3] == "1")
	{
		return "newTerm";
	}
	sql1 = "select reportQuestionForSolve from report t1 left join  user t2 on t1.reportUserId = t2.userId where t2.userName = '" +
		datalist[1] + "' and t1.reportTerm = '" + datalist[2] + "' and t1.reportWeek = '" + QString::number(datalist[3].toInt() - 1) + "';";
	query.exec(sql1);
	qDebug() << sql1;
	if (query.next())
	{
		return query.value(0).toString();
	}
	return "null";
}



//QList<ReportInfo> Report::queryReportbyId(const int ReportId)
//{
//	
//	QList<ReportInfo> list;
//	QSqlQuery query;
//	//左连接 只查表相关的数据
//	QString sql1 = "select * from report t1 left join  user t2 on t1.reportUserId = t2.userId where t1.reportId = "
//		+ QString::number(ReportId, 10);
//	query.exec(sql1);
//	qDebug() << sql1;
//	while (query.next())
//	{
//		//不能直接用[i]
//		ReportInfo ri;
//		ri.reportId = query.value(0).toInt();
//		ri.reportTerm = query.value(1).toString();
//		ri.reportWeek = query.value(2).toString();
//		ri.reportTime = query.value(3).toString();
//		ri.reportMain = query.value(4).toString();
//		ri.reportQuestionForSolve = query.value(5).toString();
//		ri.reportQuestionHasSolved = query.value(6).toString();
//		ri.reportQuestionFromLastWeek = query.value(7).toString();
//		ri.reportYourGet = query.value(8).toString();
//		ri.reportUserId = query.value(9).toInt();
//		ri.reportUserName = query.value(11).toString();
//		ri.reportName = query.value(17).toString();
//		ri.reportLevel = query.value(19).toString();
//		list.append(ri);
//	}
//	return list;
//}
