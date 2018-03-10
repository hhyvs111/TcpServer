#include "File.h"

File::File()
{
}
File::~File()
{

}

//�����ļ�
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

//ɾ���ļ���¼
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

//��ѯ��ǰ�û����ϴ����ļ�
QList<FileInfo> File::queryFilebyUser( QString name)
{
	QList<FileInfo> list;
	QString userIdFile;  //ֻ��������ѯ��
	//��ͨ���û������õ��û�ID
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
		//����ֱ����[i]
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

//��ѯ���ݿ������е��ļ�
QList<FileInfo> File::queryFile(QString condition)
{
	QList<FileInfo> list;
	QSqlQuery query;
	QString sql1;
	if (condition == "all")
	{			
		sql1 = " select * from file ";
	}
	else
	{
		sql1 = "select * from file where fileName like '%" + condition + "%';";
	}

	query.exec(sql1);
	qDebug() << sql1;
	while (query.next())
	{
		//����ֱ����[i]
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

File& File::queryFilebyId(const int fileId)
{
	QString sql = "select * from file where fileId = " + fileId;
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

