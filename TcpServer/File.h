#pragma once

#ifndef  FILE_H_
#define FILE_H_
#include "Database.h"
#include<QDataStream>
#include<QList>
//把这个结构体发给客户端

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
	
	//插入文件
	bool insertFile();

	//删除文件
	bool deleteFile(const int fileId);


	//通过用户名查询该用户所有文件
	//QList<File> queryFilebyUser(QString name);  //返回一个Qlist
	QList<FileInfo> queryFilebyUser(QString name);  //返回fileinfo结构体的list
	//通过文件Id查询文件
	File& queryFileById(const int fileId);
	File& queryFileByFileName(const QString fileName);

	//查询文件
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

