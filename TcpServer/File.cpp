#include "File.h"

File::File()
{
}
File::~File()
{

}

//插入文件
bool File::insertFile()
{

	QString sql = "insert into file (fileName,fileSize,fileType,fileTime,userId)values('" 
		+ fileName + "','" + fileSize + "','" + fileType+"','"  + fileTime + "'," + QString::number(userId,10) + ")";

	QSqlQuery insert;
	insert.exec(sql);
	if (insert.isActive())
	{
		qDebug() << sql;
		return true;
	}
	return false;
}

//删除文件记录
bool File::deleteFile(const int fileId)
{
	QString sql = "delete from file where fileId = " + QString::number(fileId, 10);
	qDebug() << sql;
	QSqlQuery delFile;
	delFile.exec(sql);
	if (delFile.isActive())
	{
		qDebug() << sql;
		return true;
	}
	return false;
}

//查询当前用户所上传的文件
QList<FileInfo> File::queryFilebyUser( QString name)
{
	QList<FileInfo> list;
	QString userIdFile;  //只是用来查询的
	//先通过用户名来得到用户ID
	QString sql = "select userId from user where userName = '"		
		+ name + "'";
	QSqlQuery query;
	query.exec(sql);
	
	if (query.next())
	{
		userIdFile = query.value(0).toString();
		qDebug() << userIdFile;
	}
	QString sql1 = " select * from file where userId =  " + userIdFile;
	query.exec(sql1);
	qDebug() << sql1;
	while (query.next())
	{
		//不能直接用[i]
		FileInfo fi;
		fi.fileId = query.value(0).toInt();
		fi.fileName = query.value(1).toString();
		fi.fileSize = query.value(2).toString();
		fi.fileType = query.value(3).toString();
		fi.fileTime = query.value(4).toString();
		fi.userId = query.value(5).toInt();
		list.append(fi);
	}
	return list;
}

//查询数据库里的文件
QList<FileInfo> File::queryFile(QString condition)
{
	QList<FileInfo> list;
	QSqlQuery query;
	QString sql1;
	if (condition == "all")
	{			
		sql1 = " select fileId,fileName,fileSize,fileType,fileTime,t1.userId,trueName from file t1 left join user t2 on t1.userId = t2.userId order by fileTime desc;";
	}
	else
	{
		sql1 = "select fileId,fileName,fileSize,fileType,fileTime,t1.userId,trueName from file t1 left join user t2 on t1.userId = t2.userId  where t1.fileName like '%" + condition + "%' order by fileTime desc;";
	}

	query.exec(sql1);
	qDebug() << sql1;
	while (query.next())
	{
		//不能直接用[i]
		FileInfo fi;
		fi.fileId = query.value(0).toInt();
		fi.fileName = query.value(1).toString();
		fi.fileSize = query.value(2).toString();
		fi.fileType = query.value(3).toString();
		fi.fileTime = query.value(4).toString();
		fi.userId = query.value(5).toInt();
		fi.fileUser = query.value(6).toString();
		list.append(fi);
	}
	return list;
}

File& File::queryFileById(const int fileId)
{
	QString sql = "select * from file where fileId = " + QString::number(fileId) + ";";
	QSqlQuery query;
	query.exec(sql);
	/*File file;*/
	if (query.next())
	{	
		fileName = query.value(1).toString();
		fileSize = query.value(2).toString();
		fileType = query.value(3).toString();
		fileTime = query.value(4).toString();
		userId = query.value(5).toInt();
		return *this;
	}
	return *this;

}
File& File::queryFileByFileName(const QString mfileName)
{
	QString sql = "select * from file where fileName = '" + mfileName + "';";
	qDebug() << sql;
	QSqlQuery query;
	query.exec(sql);
	if (query.next())
	{
		fileId = query.value(0).toInt();
		fileName = query.value(1).toString();
		fileSize = query.value(2).toString();
		fileType = query.value(3).toString();
		fileTime = query.value(4).toString();
		userId = query.value(5).toInt();
		return *this;
	}
	return *this;
}

