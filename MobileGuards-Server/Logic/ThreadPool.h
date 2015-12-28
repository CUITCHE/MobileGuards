#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "SyncQueue.h"
#include <thread>
#include <atomic>
using std::thread;

template <typename TaskQueueType>
class ThreadPool
{
public:
	using _Task = TaskQueueType;
	using ExecCallbackType = std::function < void(const _Task &object) >;
	ThreadPool(ExecCallbackType &&execCallback, int numOfThreads = std::thread::hardware_concurrency(), QObject *parent = 0)
		:tagRunning(0)
		, threadGroup(new remove_pointer<decltype(threadGroup)>::type)
		, SyncQueue(new SyncQueue(2000))
		, execCallback(new ExecCallbackType(mv(execCallback)))
	{}
	~ThreadPool()
	{
		this->stop();
	}

	void stop()
	{
		syncQueue->stop();
		if (!tagRunning) {
			return;
		}
		tagRunning = false;
		for (auto t : *threadGroup) {
			if (t) {
				t->join();
			}
		}
		threadGroup->clear();
	}
protected:
	void start(int numThreads)
	{
		tagRunning = 1;
		while (numThreads --> 0) {
			threadGroup->push_back(std::make_shared<std::thread>(&ThreadPool::exec, this));
		}
	}

	// execute thread function
	void exec()
	{
		_Task task;
		while (tagRunning) {
			syncQueue->take(task);
			(*execCallback)(task);
		}
	}
private:
	std::atomic_bool tagRunning;
	SyncQueue<_Task> *syncQueue;
	ExecCallbackType *execCallback;
	list<std::shared_ptr<std::thread>> *threadGroup;
};

#endif // THREADPOOL_H
