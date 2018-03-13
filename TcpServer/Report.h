#pragma once

#ifndef REPORT_H_
#define REPORT_H_
#include"Database.h"
#include<QDataStream>
struct ReportInfo
{
	QString reportId;
	QString reportName;
	QString reportNumber;
	QString reportLevel;
	QString reportTeacher;
	QString reportTerm;
	QString reportWeek;
	QString reportTime;
	QString reportMain;
	QString reportQuestionForSolve;
	QString reportQuestionHasSolved;
	QString reportQuestionFromLastWeek;
	QString reportYourGet;
	QString reportUserName;
	int reportUserId;
};
class Report /*: public MySql*/
{
public:
	Report();
	~Report();


	//get
	ReportInfo getReportInfo() { return reportInfo; }

	void setReportInfo(ReportInfo ri) { reportInfo = ri; }

	bool insertReport();

	bool deleteReport(const int ReportId);

	
	//查询报告
	QList<ReportInfo> queryReport(QString);
	//QList<ReportInfo> queryReportByTrueName(QString);
	////根据报告id查询报告
	//QList<ReportInfo> queryReportbyId(const int ReportId);


private:
	ReportInfo reportInfo;
};

inline QDataStream &operator>>(QDataStream &in, ReportInfo &reportInfo)
{
	QString reportId;
	QString reportName;
	QString reportNumber;
	QString reportLevel;
	QString reportTeacher;
	QString reportTerm;
	QString reportWeek;
	QString reportTime;
	QString reportMain;
	QString reportQuestionForSolve;
	QString reportQuestionHasSolved;
	QString reportQuestionFromLastWeek;
	QString reportYourGet;
	QString reportUserName;
	int reportUserId;

	in >> reportId >> reportName >> reportNumber
		>> reportLevel >> reportTeacher >> reportTerm >> reportWeek
		>> reportTime >> reportMain >> reportQuestionForSolve >> reportQuestionHasSolved
		>> reportQuestionFromLastWeek >> reportYourGet
		>> reportUserName >> reportUserId;


	reportInfo.reportId = reportId;
	reportInfo.reportName = reportName;
	reportInfo.reportNumber = reportNumber;
	reportInfo.reportLevel = reportLevel;
	reportInfo.reportTeacher = reportTeacher;
	reportInfo.reportTerm = reportTerm;
	reportInfo.reportWeek = reportWeek;
	reportInfo.reportTime = reportTime;
	reportInfo.reportMain = reportMain;
	reportInfo.reportQuestionForSolve = reportQuestionForSolve;
	reportInfo.reportQuestionHasSolved = reportQuestionHasSolved;
	reportInfo.reportQuestionFromLastWeek = reportQuestionFromLastWeek;
	reportInfo.reportYourGet = reportYourGet;
	reportInfo.reportUserName = reportUserName;
	reportInfo.reportUserId = reportUserId;

	return in;
}


inline QDataStream &operator<<(QDataStream &out, const ReportInfo &reportInfo)
{
	out << reportInfo.reportId << reportInfo.reportName << reportInfo.reportNumber
		<< reportInfo.reportLevel << reportInfo.reportTeacher << reportInfo.reportTerm
		<< reportInfo.reportWeek << reportInfo.reportTime << reportInfo.reportMain
		<< reportInfo.reportQuestionForSolve << reportInfo.reportQuestionHasSolved
		<< reportInfo.reportQuestionFromLastWeek << reportInfo.reportYourGet
		<< reportInfo.reportUserName << reportInfo.reportUserId;
	return out;
}

#endif // !REPORT_H_ 
