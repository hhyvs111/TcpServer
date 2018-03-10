#include "MyThreadHandle.h"
//#include "eventdispatcher_libev/eventdispatcher_libev.h"

ThreadHandle::ThreadHandle()  //���캯��������û�������ʼ��
{
	initfist = false;
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
	if (!initfist)		
	{
		initThreadType(THREADSIZE, 10);
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

void ThreadHandle::initThreadType(ThreadType type, unsigned int max)
{
	if (!initfist)
	{
		this->type = type;
		this->size = max;
		if (this->size == 0)
		{
			if (type == THREADSIZE)
				this->size = 10;
			else
				this->size = 1000;
		}

		if (type == THREADSIZE)
			initThreadSize();
		else
		{
			QThread * tmp = new QThread;
#ifndef Q_OS_WIN
			tmp->setEventDispatcher(new EventDispatcherLibEv());
#endif
			threadSize.insert(tmp, 0);
			tmp->start();
		}
	}
	initfist = true;
}

void ThreadHandle::initThreadSize() //�������̲߳�������
{
	QThread * tmp;
	for (unsigned int i = 0; i < size;++i)
	{
		tmp = new QThread;
#ifndef Q_OS_WIN
		tmp->setEventDispatcher(new EventDispatcherLibEv());
#endif
		threadSize.insert(tmp, 0);
		tmp->start();
	}
}

QThread * ThreadHandle::findHandleSize() //���ҵ��߳����������С�����ֵ�ͷ��ز��ҵ��ģ��Ҳ������½�һ���߳�
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
#ifndef Q_OS_WIN
	tmp->setEventDispatcher(new EventDispatcherLibEv());
#endif
	threadSize.insert(tmp, 1);
	tmp->start();
	return tmp;
}

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
