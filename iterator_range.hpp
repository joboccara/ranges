#ifndef ITERATOR_RANGE_HPP
#define ITERATOR_RANGE_HPP

#include <iterator>
#include <type_traits>

template<typename Iterator>
struct is_const_iterator
{
    static const bool value = std::is_const<
                                 typename std::remove_reference<
                                    typename std::iterator_traits<
                                       Iterator
                                    >::reference
                                 >::type
                              >::value;
};

template<bool const_iterator_range, typename Iterator>
class iterator_range_impl
{
public:
	iterator_range_impl(Iterator begin, Iterator end) : begin_(begin), end_(end) {}
	Iterator begin() const {return begin_;}
	Iterator end() const {return end_;}
	typename std::iterator_traits<Iterator>::difference_type const size(){return end_ - begin_;}

	using const_iterator = Iterator;
private:
	Iterator begin_, end_;
};

template<typename Iterator>
class iterator_range_impl<false, Iterator>
{
public:
	iterator_range_impl(Iterator begin, Iterator end) : begin_(begin), end_(end) {}
	Iterator begin() const {return begin_;}
	Iterator end() const {return end_;}
	typename std::iterator_traits<Iterator>::difference_type const size(){return end_ - begin_;}

	using iterator = Iterator;
private:
	Iterator begin_, end_;
};

template<typename Iterator>
class iterator_range : public iterator_range_impl<is_const_iterator<Iterator>::value, Iterator>
{
public:
    typedef iterator_range_impl<is_const_iterator<Iterator>::value, Iterator> base;
    iterator_range(Iterator begin, Iterator end) : base(begin, end) {}
};


#endif
