#include "MyThreadHandle.h"
//#include "eventdispatcher_libev/eventdispatcher_libev.h"

ThreadHandle::ThreadHandle()  //构造函数，但是没有在这初始化
{
	initFirst = false;
}

ThreadHandle::~ThreadHandle() //停止所有线程，并释放资源，析构函数
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

ThreadHandle & ThreadHandle::getClass()  //返回一个类？
{
	static ThreadHandle th;
	return th;
}

QThread *ThreadHandle::getThread()   //获得类中的线程
{
	//如果没有初始化则初始化
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

//删除线程
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

//初始化类型选择？初试化线程池，和初始化单个线程连接数
void ThreadHandle::initThreadType(ThreadType type, unsigned int max)
{
	//如果是第一次初始化initfist
	if (!initFirst)
	{
		this->type = type;
		this->size = max;
		if (this->size == 0)
		{
			//如果type是线程类型初始化，那么有10个线程
			if (type == THREADSIZE)
				this->size = 20;
			else
				//如果type是HandleSize，那么一个线程有1000个连接数
				this->size = 100;
		}

		//如果是要初始化线程，那么进入initThreadSize函数
		if (type == THREADSIZE)
			initThreadSize();
		//否则新建一个线程
		else
		{
			QThread * tmp = new QThread;
//#ifndef Q_OS_WIN
//			tmp->setEventDispatcher(new EventDispatcherLibEv());
//#endif
			//将这个线程插入到线程池，QHash
			threadSize.insert(tmp, 0);
			tmp->start();  // 这里开始运行了
		}
	}
	initFirst = true;
}

//初始化所有的线程，比如有10个就初始化10个
void ThreadHandle::initThreadSize() //建立好线程并启动，
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
//查找到线程里的连接数小于最大值就返回查找到的线程的键值（key），找不到就新建一个线程
//这个和下面那个的区别是如果第一个线程的连接数小于size就返回
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

//这个是找最小的
QThread *ThreadHandle::findThreadSize() //遍历查找所有线程中连接数最小的那个，返回
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

void ThreadHandle::clear()//仅仅清空计数，线程不释放
{
	for (auto it = threadSize.begin();it != threadSize.end();++it)
	{
		it.value() = 0;
	}
}
