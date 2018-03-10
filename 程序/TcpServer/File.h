#pragma once
#include "Database.h"
#include<QList>
#ifndef  FILE_H_
#define FILE_H_

struct FileInfo
{
	int fileId;
	QString fileName;
	QString fileSize;
	QString fileType;
	QString fileTime;
	//QString fileUser;
	int userId;
};
class File : public MySql
{
public:
	File();
	~File();

	//get
	QString getFileName() { return fileName; }
	QString getFileSize() { return fileSize; }
	QString getFileType() { return fileType; }
	QString getFileTime() { return fileTime; }
	//QString getFileUser() { return fileUser; }
	int getUserId() { return userId; }
	int getFileId() { return fileId; }
	//set
	void setFileName(QString fn) { fileName = fn; }
	void setFileSize(QString fs) { fileSize = fs; }
	void setFileType(QString ft) { fileType = ft; }
	void setFileTime(QString ft) { fileTime = ft; }
	//void setFileUser(QString fu) { fileUser = fu; }
	void setUserId(int id) { userId = id; }
	void setFileId(int id) { fileId = id; }
	
	//�����ļ�
	bool insertFile();

	//ɾ���ļ�
	bool deleteFile(const int fileId);


	//ͨ���û�����ѯ���û������ļ�
	//QList<File> queryFilebyUser(QString name);  //����һ��Qlist
	QList<FileInfo> queryFilebyUser(QString name);  //����fileinfo�ṹ���list
	//ͨ���ļ�Id��ѯ�ļ�
	File& queryFilebyId(const int fileId);

	//��ѯ�ļ�
	QList<FileInfo> queryFile(QString);


private:
	int fileId;
	QString fileName;
	QString fileSize;
	QString fileType;
	QString fileTime;
	//QString fileUser;
	int userId;
	FileInfo fileInfo;

};
#endif // ! FILE_H_

