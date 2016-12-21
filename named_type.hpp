#ifndef NAMED_TYPE_HPP
#define NAMED_TYPE_HPP

#include <functional>

template<typename T, typename Context>
class NamedType
{
public:
	explicit NamedType(T t) : t_(std::move(t)){}
	T& get() {return t_;}
private:
	T t_;
};

template<typename T, typename Context>
class NamedTypeRef
{
public:
	explicit NamedTypeRef(T& t) : t_(std::ref(t)){}
	T& get() {return t_.get();}
private:
	std::reference_wrapper<T> t_;
};


#endif

