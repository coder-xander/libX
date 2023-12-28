#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <optional>
template <typename T>
class XQueue
{
private:
	std::deque<T> deque_;
	mutable std::mutex mutex_;
	std::condition_variable condVar_;

public:
	XQueue() = default;
	XQueue(const XQueue &other)
	{
		std::lock_guard<std::mutex> lock(other.mutex_);
		deque_ = other.deque_;
	}

	void enqueue(const T &value)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		deque_.push_back(std::move(value));
		condVar_.notify_one();
	}

	void waitAndPop(T &value)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		condVar_.wait(lock, [this]
					  { return !deque_.empty(); });
		value = std::move(deque_.front());
		deque_.pop_front();
	}

	std::optional<T> tryPop()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (deque_.empty())
		{
			return std::nullopt;
		}
		auto value = std::move(deque_.front());
		deque_.pop_front();
		return value;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return deque_.empty();
	}

	void clear()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		deque_.clear();
	}

	size_t size()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return deque_.size();
	}

	T find(std::function<bool(const T &)> adptor)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		for (T item : deque_)
		{
			if (adptor(item))
				return item;
		}
		return T();
	}
	bool removeOne(std::function<bool(const T &)> adptor)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		for (const T &item : deque_)
		{
			if (adptor(item))
				deque_.erase(std::remove(deque_.begin(), deque_.end(), item), deque_.end());
			return true;
		}
		return false;
	}
};
