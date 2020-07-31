#pragma once

#define DEFINE_DELEGATE(dname, ...) class dname : public DelegateBase<__VA_ARGS__> {							\
typedef void(*DelType)(void *callee, __VA_ARGS__);																\
public: dname() : DelegateBase(nullptr, nullptr) {}																\
dname(void* callee, DelType function) : DelegateBase(callee, function) {}										\
template <class T, void (T::*TMethod)(__VA_ARGS__)>																\
static dname from_function(T* callee)																			\
{																												\
	dname d(callee, &methodCaller<T, TMethod>);																	\
	return d;																									\
}																												\
bool operator == (const dname& rhs)																				\
{																												\
	return (fpCallbackFunction == rhs.fpCallbackFunction && fpCallee == rhs.fpCallee);							\
}																												\
}



template<typename ...TArgs>
class DelegateBase
{
	typedef void(*DelType)(void *callee, TArgs ...params);

public:
	DelegateBase(void* callee, DelType function)
		: fpCallee(callee)
		, fpCallbackFunction(function) {}

// 	// Initialize the delegate with a method
// 	template <class T, void (T::*TMethod)(TArgs ...params)>
// 	static DelegateBase from_function(T* callee)
// 	{
// 		DelegateBase d(callee, &methodCaller<T, TMethod>);
// 		return d;
// 	}

	void invokeSafe(TArgs ...params) const
	{
		if (fpCallee != nullptr && fpCallbackFunction != nullptr)
		{
			return (*fpCallbackFunction)(fpCallee, params...);
		}
	}

	void operator()(TArgs ...params) const
	{
		return (*fpCallbackFunction)(fpCallee, params...);
	}

	bool operator == (const DelegateBase& rhs)
	{
		return (fpCallbackFunction == rhs.fpCallbackFunction && fpCallee == rhs.fpCallee);
	}
protected:
	void* fpCallee;
	DelType fpCallbackFunction;

	// Helper method casting method to proper type
	template <class T, void (T::*TMethod)(TArgs ...params)>
	static void methodCaller(void* callee, TArgs ...params)
	{
		T* p = static_cast<T*>(callee);
		return (p->*TMethod)(params...);
	}
};

class NewDelegate : public DelegateBase<>
{

};