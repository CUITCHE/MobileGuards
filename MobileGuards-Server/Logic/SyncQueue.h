#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include "defs.h"
#include <list>
#include <condition_variable>
using namespace std;

template<typename T>
class SyncQueue
{
public:
	SyncQueue(int queueMaxSize = 10)
		:_needStop(0)
		, _queueMaxSize(queueMaxSize)
		, _queueList(new remove_pointer<decltype(_queueList)>::type)
		, _mutex(new mutex)
		, _queueConditionNotEmpty(new condition_variable)
		, _queueConditionoNotFull(new condition_variable)
	{
		_queueList->reserve(queueMaxSize);
	}
	~SyncQueue();
	void put(T &task)
	{
		addTask(task);
	}
	void put(T &&task)
	{
		addTask(std::forward<T>(task));
	}

	void take(QList<T> &tasks)
	{
		this->takeTask([this, &tasks]{tasks = mv(*_queueList)});
	}
	void take(T &task)
	{
		this->takeTask([this, &task]{task = _queueList->front(); _queueList->pop_front(); });
	}

	void stop()
	{
		{
			std::lock_guard<mutex> locker(*_mutex);
			_needStop = true;
		}
		_queueConditionoNotFull->notify_all();
		_queueConditionNotEmpty->notify_all();
	}

	// true if queue list is empty.
	bool isEmpty()
	{
		return _queueList->empty();
	}
	// true if queue list is full.
	bool isFull()
	{
		return _queueList->size() >= _queueMaxSize;
	}

	size_t size() const
	{
		return _queueList->size();
	}
	size_t count() const
	{
		return _queueList->size();
	}
protected:
	template <typename _F>
	void addTask(_F &&task)
	{
		unique_lock<mutex> locker(*mutex);
		_queueConditionoNotFull->wait(locker, [this] {return !this->isFull() || _needStop; });
		do {
			if (_needStop) {
				break;
			}
			_queueList->emplace(std::forward<_F>(task));
			_queueConditionNotEmpty->notify_one();
		} while (0);
	} 

	template <typename _Pred>
	void takeTask(_Pred pred)
	{
		std::unique_ptr<mutex> locaker(*_mutex);
		_queueConditionNotEmpty->wait(locaker, [this] {return !this->isEmpty() || _needStop; });
		do {
			if (_needStop) {
				break;
			}
			pred();
			_queueConditionoNotFull->notify_one();
		} while (0);
	}
private:
	quint32 _needStop : 1;
	const quint32 _queueMaxSize : 31;
	list<T> *_queueList;
	mutex *_mutex;
	condition_variable *_queueConditionNotEmpty;
	condition_variable *_queueConditionoNotFull;
};

#endif // SYNCQUEUE_H
