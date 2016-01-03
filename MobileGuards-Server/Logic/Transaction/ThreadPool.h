#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "SyncQueue.h"
#include <thread>
#include <atomic>

using std::thread;
using std::atomic_bool;

template <typename TaskQueueType>
class ThreadPool
{
public:
	using Task = TaskQueueType;
	using ExecCallbackType = std::function < void(const Task &object) >;
	ThreadPool(ExecCallbackType &&execCallback, int numOfThreads = std::thread::hardware_concurrency())
		: CLASS_MEMBER_CONSTRUCTURE(_threadGroup,)
		, CLASS_MEMBER_CONSTRUCTURE(_syncQueue, 2000)
		, CLASS_MEMBER_CONSTRUCTURE(_execCallback, mv(execCallback))
	{
		_tagRunning = true;
		this->start(numOfThreads);
	}
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
		Task task;
		while (_tagRunning) {
			_syncQueue->take(task);
			(*_execCallback)(task);
		}
	}
private:
	atomic_bool _tagRunning;
	SyncQueue<Task> *_syncQueue;
	ExecCallbackType *_execCallback;
	list<std::shared_ptr<std::thread>> *_threadGroup;
};

#endif // THREADPOOL_H
