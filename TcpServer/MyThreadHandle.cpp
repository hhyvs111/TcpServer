#include "MyThreadHandle.h"
//#include "eventdispatcher_libev/eventdispatcher_libev.h"

ThreadHandle::ThreadHandle()  //���캯��������û�������ʼ��
{
	initFirst = false;
}

ThreadHandle::~ThreadHandle() //ֹͣ�����̣߳����ͷ���Դ����������
{
	QThread * tmp;
	for (auto it = threadSize.begin(); it != threadSize.end(); ++it)
	{
		tmp = it.key();
		tmp->exit();
		tmp->wait(3000);
		delete tmp;
	}
}

ThreadHandle & ThreadHandle::getClass()  //����һ���ࣿ
{
	static ThreadHandle th;
	return th;
}

QThread *ThreadHandle::getThread()   //������е��߳�
{
	//���û�г�ʼ�����ʼ��
	if (!initFirst)
	{
		qDebug() << "Thread init";
		initThreadType(THREADSIZE, 20);
	}
	if (type == THREADSIZE)
		return findThreadSize();
	else
		return findHandleSize();
}

//ɾ���߳�
void ThreadHandle::removeThread(QThread * thread)
{
	auto t = threadSize.find(thread);
	if (t != threadSize.end())
	{
		t.value()--;
		if (type == HANDLESIZE && t.value() == 0 && threadSize.size() > 1)
		{
			threadSize.remove(thread);
			thread->exit();
			thread->wait(3000);
			delete thread;
		}
	}
}

//��ʼ������ѡ�񣿳��Ի��̳߳أ��ͳ�ʼ�������߳�������
void ThreadHandle::initThreadType(ThreadType type, unsigned int max)
{
	//����ǵ�һ�γ�ʼ��initfist
	if (!initFirst)
	{
		this->type = type;
		this->size = max;
		if (this->size == 0)
		{
			//���type���߳����ͳ�ʼ������ô��10���߳�
			if (type == THREADSIZE)
				this->size = 20;
			else
				//���type��HandleSize����ôһ���߳���1000��������
				this->size = 100;
		}

		//�����Ҫ��ʼ���̣߳���ô����initThreadSize����
		if (type == THREADSIZE)
			initThreadSize();
		//�����½�һ���߳�
		else
		{
			QThread * tmp = new QThread;
//#ifndef Q_OS_WIN
//			tmp->setEventDispatcher(new EventDispatcherLibEv());
//#endif
			//������̲߳��뵽�̳߳أ�QHash
			threadSize.insert(tmp, 0);
			tmp->start();  // ���￪ʼ������
		}
	}
	initFirst = true;
}

//��ʼ�����е��̣߳�������10���ͳ�ʼ��10��
void ThreadHandle::initThreadSize() //�������̲߳�������
{
	QThread * tmp;
	for (unsigned int i = 0; i < size;++i)
	{
		tmp = new QThread;
//#ifndef Q_OS_WIN
//		tmp->setEventDispatcher(new EventDispatcherLibEv());
//#endif
		threadSize.insert(tmp, 0);
		tmp->start();
	}
}
//���ҵ��߳����������С�����ֵ�ͷ��ز��ҵ����̵߳ļ�ֵ��key�����Ҳ������½�һ���߳�
//����������Ǹ��������������һ���̵߳�������С��size�ͷ���
QThread * ThreadHandle::findHandleSize() 
{
	for (auto it = threadSize.begin();it != threadSize.end();++it)
	{
		if (it.value() < size)
		{
			it.value()++;
			return it.key();
		}
	}
	QThread * tmp = new QThread;
//#ifndef Q_OS_WIN
//	tmp->setEventDispatcher(new EventDispatcherLibEv());
//#endif
	threadSize.insert(tmp, 1);
	tmp->start();
	return tmp;
}

//���������С��
QThread *ThreadHandle::findThreadSize() //�������������߳�����������С���Ǹ�������
{
	auto it = threadSize.begin();
	auto ite = threadSize.begin();
	for (++it; it != threadSize.end(); ++it)
	{
		if (it.value() < ite.value())
		{
			ite = it;
		}
	}
	ite.value()++;
	return ite.key();
}

void ThreadHandle::clear()//������ռ������̲߳��ͷ�
{
	for (auto it = threadSize.begin();it != threadSize.end();++it)
	{
		it.value() = 0;
	}
}
