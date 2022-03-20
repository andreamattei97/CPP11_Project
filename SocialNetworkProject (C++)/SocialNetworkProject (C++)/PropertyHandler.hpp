#ifndef PROPERTY_HANDLER_HPP
#define PROPERTY_HANDLER_HPP

#include <functional>

using namespace std;

template<typename T>
using SetMethod = function<bool(const T&)>;

template<typename T>
using GetMethod = function<T()>;

using IsInitializedMethod = function<bool()>;

template<typename T>
class PropertyHandler //used to wrap safely an attribute
{
public:
	PropertyHandler(SetMethod<T> setMethod,GetMethod<T> getMethod,IsInitializedMethod isInitializedMethod);
	PropertyHandler();

	bool Set(const T& value);
	T Get() const;
	bool IsInitialized() const;

	bool IsErrorHandler() const;
private:
	SetMethod<T> setMethod;
	GetMethod<T> getMethod;
	IsInitializedMethod isInitializedMethod;

	bool error;
};

template<typename T>
inline PropertyHandler<T>::PropertyHandler(SetMethod<T> setMethod, GetMethod<T> getMethod, IsInitializedMethod isInitializedMethod):setMethod(setMethod),getMethod(getMethod),isInitializedMethod(isInitializedMethod)
{
	error = false;
}

template<typename T>
inline PropertyHandler<T>::PropertyHandler()
{
	setMethod = [](const T& value) {return false; };
	getMethod = []() {return T(); };
	isInitializedMethod = []() {return false; };
	error = true;
}

template<typename T>
inline bool PropertyHandler<T>::Set(const T & value)
{
	return setMethod(value);
}

template<typename T>
inline T PropertyHandler<T>::Get() const
{
	return getMethod();
}

template<typename T>
inline bool PropertyHandler<T>::IsInitialized() const
{
	return isInitializedMethod();
}

template<typename T>
inline bool PropertyHandler<T>::IsErrorHandler() const
{
	return error;
}

#endif