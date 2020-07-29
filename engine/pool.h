#pragma once
#include <memory>
#include <vector>
#include <functional>

class Poolable
{
public:
	virtual void Reset()
	{
		_isAvailable = false;
	}

	virtual void Destroy() 
	{
		_isAvailable = true;
	}

	bool IsAvailable() { return _isAvailable; }

private:
	bool _isAvailable;
};

template<typename T>
using Func = std::function<T()>;

template<typename T>
class Pool
{
public:
	Pool() {}

	static void Initialize(int capacity, bool expandable, Func<std::shared_ptr<T>> newObjectFunction)
	{
		_newObjectFunction = newObjectFunction;
		_expandable = expandable;
		_pool.reserve(capacity);
		for (int i = 0; i < capacity; ++i)
		{
			_pool.push_back(newObjectFunction());
		}
	}

	static std::shared_ptr<T> Get()
	{
		static_assert(std::is_base_of<Poolable, T>::value, "T != Poolable");
		for (std::shared_ptr<T> o : _pool)
		{
			std::shared_ptr<Poolable> poolable = std::static_pointer_cast<Poolable>(o);
			if (poolable && poolable->IsAvailable())
			{
				std::cout << "Found an entity in pool. Returning..." << std::endl;
				poolable->Reset();
				return o;
			}
		}

		if (_expandable)
		{
			std::shared_ptr<T> newObject = _newObjectFunction();
			_pool.push_back(newObject);
			return newObject;
		}

		return nullptr;
	}

private:
	static std::vector<std::shared_ptr<T>> _pool;
	static Func<std::shared_ptr<T>> _newObjectFunction;
	static bool _expandable;
};

template<typename T>
std::vector<std::shared_ptr<T>> Pool<T>::_pool;

template<typename T>
Func<std::shared_ptr<T>>  Pool<T>::_newObjectFunction;

template<typename T>
bool Pool<T>::_expandable;