#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "iterator_range.hpp"

#include <iterator>
#include <type_traits>

namespace ranges
{

template<typename UnderlyingIterator, typename Function>
class transform_iterator
	: public std::iterator<
                              typename std::iterator_traits<UnderlyingIterator>::iterator_category,
                              std::result_of_t<Function(typename std::iterator_traits<UnderlyingIterator>::value_type)>,
                              typename std::iterator_traits<UnderlyingIterator>::difference_type,
                              const std::result_of_t<Function(typename std::iterator_traits<UnderlyingIterator>::value_type)>*,
                              const std::result_of_t<Function(typename std::iterator_traits<UnderlyingIterator>::value_type)>&
                          >
{
public:
	transform_iterator(UnderlyingIterator iterator, Function func) : iterator_(iterator), func_(func) {}
	transform_iterator operator++() {++iterator_; return *this;}
	auto operator*() { return func_(*iterator_);}
	transform_iterator& operator=(const transform_iterator& other) { iterator_ = other.iterator_; func_ = other.func_;}
	bool operator==(const transform_iterator& other){ return iterator_ == other.iterator_; }
	bool operator!=(const transform_iterator& other){ return !(*this == other); }
	auto operator-(const transform_iterator& other) { return iterator_ - other.iterator_;}

private:
	UnderlyingIterator iterator_;
	Function func_;
};

template<typename Iterator, typename Function>
transform_iterator<Iterator, Function> make_transform_iterator(Iterator underlyingIterator, Function func)
{
	return transform_iterator<Iterator, Function>(underlyingIterator, func);
}

namespace view
{

template<typename Function>
class transform_adaptor
{
public:
	explicit transform_adaptor(Function func) : func_(func) {}
	Function getFunction() const {return func_;}
private:
	Function func_;
};

template<typename Function>
transform_adaptor<Function> transform(Function func)
{
	return transform_adaptor<Function>(func);
}

template<typename Range, typename Function>
iterator_range<transform_iterator<typename Range::const_iterator, Function>> operator|(Range const& range, transform_adaptor<Function> adaptor)
{
	return iterator_range<transform_iterator<typename Range::const_iterator, Function>>(
		make_transform_iterator(range.begin(), adaptor.getFunction()),
		make_transform_iterator(range.end(), adaptor.getFunction()));
}
    
} // namespace view
} // namespace ranges

#endif
