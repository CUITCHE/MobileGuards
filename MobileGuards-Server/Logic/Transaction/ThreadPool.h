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
		:_tagRunning(0)
		, _threadGroup(new remove_pointer<decltype(_threadGroup)>::type)
		, _syncQueue(new SyncQueue(2000))
		, _execCallback(new ExecCallbackType(mv(execCallback)))
	{}
	~ThreadPool()
	{
		this->stop();
	}

	void stop()
	{
		_syncQueue->stop();
		if (!_tagRunning) {
			return;
		}
		_tagRunning = false;
		for (auto t : *_threadGroup) {
			if (t) {
				t->join();
			}
		}
		_threadGroup->clear();
	}
protected:
	void start(int numThreads)
	{
		_tagRunning = 1;
		while (numThreads --> 0) {
			_threadGroup->emplace_back(std::make_shared<std::thread>(&ThreadPool::exec, this));
		}
	}

	// execute thread function
	void exec()
	{
		_Task task;
		while (_tagRunning) {
			_syncQueue->take(task);
			(*_execCallback)(task);
		}
	}
private:
	std::atomic_bool _tagRunning;
	SyncQueue<_Task> *_syncQueue;
	ExecCallbackType *_execCallback;
	list<std::shared_ptr<std::thread>> *_threadGroup;
};

#endif // THREADPOOL_H
