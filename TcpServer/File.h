#pragma once

#ifndef  FILE_H_
#define FILE_H_
#include "Database.h"
#include<QDataStream>
#include<QList>
//������ṹ�巢���ͻ���

struct FileInfo
{
	int fileId;
	QString fileName;
	QString fileSize;
	QString fileType;
	QString fileTime;
	QString fileUser;
	int userId;
};
class File /*: public MySql*/
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
	File& queryFileById(const int fileId);
	File& queryFileByFileName(const QString fileName);

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

inline QDataStream &operator<<(QDataStream &out, const FileInfo &fileInfo)
{
	out << fileInfo.fileId << fileInfo.fileName<<fileInfo.fileSize<< fileInfo.fileTime<< fileInfo.fileType<< fileInfo.fileUser<< fileInfo.userId;
	return out;
}
#endif // ! FILE_H_

