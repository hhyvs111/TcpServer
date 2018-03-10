#pragma once
#include"Database.h"
#ifndef REPORT_H_
#define REPORT_H_
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
class Report : public MySql
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

#endif // !REPORT_H_ 
