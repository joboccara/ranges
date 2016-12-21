#ifndef ADJACENT_HPP
#define ADJACENT_HPP

/* range view adaptor to represent pairs of consecutive elements:
   Applying it on range containing:
   {a, b, c, d, e, f}
   produces a view seeing:
   {
    std::pair(a,b),
    std::pair(b,c),
    std::pair(c,d),
    std::pair(d,e),
    std::pair(e,f)
   }
*/

#include "iterator_range.hpp"
#include "named_type.hpp"

#include <iterator>
#include <utility>

namespace ranges
{

template<typename Iterator>
using BeginIterator = NamedType<Iterator, struct begin_context>;
template<typename Iterator>
using EndIterator = NamedType<Iterator, struct end_context>;

template<typename UnderlyingIterator>
class adjacent_iterator
	: public std::iterator<
                              typename std::iterator_traits<UnderlyingIterator>::iterator_category,
                              std::pair<typename std::iterator_traits<UnderlyingIterator>::value_type, typename std::iterator_traits<UnderlyingIterator>::value_type>,
                              typename std::iterator_traits<UnderlyingIterator>::difference_type,
                              const std::pair<typename std::iterator_traits<UnderlyingIterator>::value_type, typename std::iterator_traits<UnderlyingIterator>::value_type>*,
                              const std::pair<typename std::iterator_traits<UnderlyingIterator>::value_type, typename std::iterator_traits<UnderlyingIterator>::value_type>&
                          >
{
public:
	explicit adjacent_iterator(BeginIterator<UnderlyingIterator> begin, EndIterator<UnderlyingIterator> end) : iterator_(begin.get()), next_(begin.get() != end.get() ? std::next(begin.get()) : begin.get()), isEnd_(false) {}
	explicit adjacent_iterator(EndIterator<UnderlyingIterator> end) : iterator_(end.get()), next_(end.get()), isEnd_(true) {}
	adjacent_iterator operator++() {++iterator_; ++next_; return *this;}
	auto operator*() { return std::make_pair(*iterator_, *next_);}
	bool operator==(const adjacent_iterator& other)
    {
        if (isEnd_ == other.isEnd_) return iterator_ == other.iterator_;
        if (other.isEnd_) return next_ == other.iterator_;
        if (isEnd_) return iterator_ == other.next_;   
    }
	bool operator!=(const adjacent_iterator& other){ return !(*this == other); }
	auto operator-(const adjacent_iterator& other) { return next_ - other.next_;}

private:
	UnderlyingIterator iterator_;
	UnderlyingIterator next_;
    bool isEnd_;
};

namespace view
{

class adjacent_adaptor {};
adjacent_adaptor adjacent;

template<typename Range>
auto operator|(Range& range, adjacent_adaptor) -> iterator_range<adjacent_iterator<decltype(range.begin())>>
{
	using UnderlyingIterator = decltype(range.begin());
	return iterator_range<adjacent_iterator<UnderlyingIterator>>(
		adjacent_iterator<UnderlyingIterator>(BeginIterator<UnderlyingIterator>(range.begin()), EndIterator<UnderlyingIterator>(range.end())),
		adjacent_iterator<UnderlyingIterator>(EndIterator<UnderlyingIterator>(range.end())));
}
    
} // namespace view
} // namespace ranges


#endif
