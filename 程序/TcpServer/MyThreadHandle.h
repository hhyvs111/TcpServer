#pragma once
#ifndef THREADHANDLE_H
#define THREADHANDLE_H

#include <QThread>
#include <QQueue>
#include <QList>
#include <QHash>
//�̹߳����࣬������һ���̳߳أ�������
//���ֳ�ʼ����ʽ��һ����ÿ���̴߳������������һ����һ�����ٸ��߳�
class ThreadHandle
{
public:
	~ThreadHandle();


	enum ThreadType
	{
		THREADSIZE, //�̶��߳���//�����3���̣߳�1�����̴߳������ӣ�1���̴߳���һ�����Ϣ��һ�������ļ�����
		HANDLESIZE //�̶�ÿ���̴߳����������
	};

	static ThreadHandle & getClass(); //���ض������ã��ǵ�����

	QThread * getThread(); //ȡ��Ӧ��������߳�
	void initThreadType(ThreadType type = HANDLESIZE, unsigned int max = 1000);//��ʼ���̹߳���ķ�ʽ
	void removeThread(QThread *);//���ӶϿ����̼߳�����һ
	void clear();//��ռ���

protected:
	void initThreadSize();//�½��̶��̺߳�����
	QThread *findThreadSize();//�̶��߳����Ĳ���
	QThread *findHandleSize();//�̶�����������
private:
	ThreadHandle();
	ThreadType type;//�߳�����
	unsigned int size;//���ֵ
	QHash<QThread *, unsigned int> threadSize;//����ÿ���̵߳���Ŀ
	bool initfist;//�Ƿ��ǵ�һ�γ�ʼ����ֻ�����ʼ��һ�Ρ�

};

#endif // THREADHANDLE_H
