#pragma once

#define STRUCT_INTERNAL(dname) __##dname

#define DEFINE_DELEGATE(dname, ...)\
class dname : public DelegateBase<__VA_ARGS__>\
{\
	typedef void(*DelType)(void *callee, __VA_ARGS__);\
	typedef FDelegateInternal STRUCT_INTERNAL(dname);\
\
	public:\
		dname(){}\
		dname(const STRUCT_INTERNAL(dname) &d)\
		{\
			_boundMethods.push_back(d);\
		}\
		dname(void* callee, DelType function) : DelegateBase(callee, function) {}\
\
		template <class T, void (T::*TMethod)(__VA_ARGS__)>\
		static STRUCT_INTERNAL(dname) from_function(T* callee)\
		{\
			STRUCT_INTERNAL(dname) d(callee, &methodCaller<T, TMethod>);\
			return d;\
		}\
		dname& operator=(const STRUCT_INTERNAL(dname) &rhs)\
		{\
			_boundMethods.clear();\
			_boundMethods.push_back(rhs);\
			return *this;\
		}\
		dname& operator+=(const STRUCT_INTERNAL(dname) &rhs)\
		{\
			_boundMethods.push_back(rhs);\
			return *this;\
		}\
		const bool contains (const STRUCT_INTERNAL(dname)& rhs) const\
		{\
		return std::count(_boundMethods.begin(), _boundMethods.end(), rhs);\
		}\
		const bool operator == (const dname& rhs) const\
		{\
		return (_boundMethods == rhs._boundMethods);\
		}\
		dname& operator-=(const STRUCT_INTERNAL(dname) &rhs)\
		{\
			_boundMethods.erase(std::remove(_boundMethods.begin(), _boundMethods.end(), rhs), _boundMethods.end());\
			return *this;\
		}\
}
// bool operator == (const dname& rhs)																				\
// {																												\
// 	return (_boundMethods == rhs._boundMethods);							\
// }																												\
// dname& operator=(const dname &rhs) 																		\
// {																												\
// _boundMethods.clear();																						\
// _boundMethods.push_back(rhs);																				\
// return *this;																								\
// }																												\
// dname& operator+=(const dname &rhs) 																			\
// {																												\
// _boundMethods.push_back(rhs);																				\
// }																												\
}																												

// 
																										

template<typename ...TArgs>
class DelegateBase
{
	typedef void(*DelType)(void *callee, TArgs ...params);
	
protected:
	struct FDelegateInternal
	{
		FDelegateInternal() = delete;

		FDelegateInternal(void* callee, DelType function)
			: fpCallee(callee)
			, fpCallbackFunction(function)
		{
		}

		void operator()(TArgs ...params) const
		{
			if (fpCallee != nullptr)
			{
				return (*fpCallbackFunction)(fpCallee, params...);
			}
		}

		const bool operator == (const FDelegateInternal& rhs) const
		{
			return (fpCallee == rhs.fpCallee && fpCallbackFunction == rhs.fpCallbackFunction);
		}

		void* getCallee() { return fpCallee; }

	protected:
		void* fpCallee;
		DelType fpCallbackFunction;
	};

public:

	DelegateBase()
	{

	}

	DelegateBase(void* callee, DelType function)
	{
		_boundMethods.push_back(FDelegateInternal(callee, function));
	}

	void invokeSafe(TArgs ...params)
	{
		for (FDelegateInternal method : _boundMethods)
		{
			method(params...);
		}
	}
	bool operator == (const DelegateBase& rhs)
	{
		return (_boundMethods == rhs._boundMethods);
	}

	FDelegateInternal getDelegateInternal() { return _boundMethods[0]; }

protected:

	std::vector<FDelegateInternal> _boundMethods;

	// Helper method casting method to proper type
	template <class T, void (T::*TMethod)(TArgs ...params)>
	static void methodCaller(void* callee, TArgs ...params)
	{
		T* p = (T*)(callee);
		return (p->*TMethod)(params...);
	}
};
