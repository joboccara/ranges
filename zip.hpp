#ifndef ZIP_HPP
#define ZIP_HPP

/* range view adaptor to represent groupings of elements from a number of ranges
   Applying it on 3 ranges containing:
   { a,b }
   { 1,2 }
   { X,Y,Z }
   produces a view seeing:
   {
     std::tuple(a,1,X),
     std::tuple(b,2,Y)
   }
*/

#include "iterator_range.hpp"
#include "variadic.hpp"

#include <iterator>
#include <tuple>

namespace ranges
{

template<typename... UnderlyingIterators>
class zip_iterator
	: public std::iterator<
                              typename std::iterator_traits<get_type_t<0, UnderlyingIterators...>>::iterator_category,
                              std::tuple<typename std::iterator_traits<UnderlyingIterators>::reference...>,
                              typename std::iterator_traits<get_type_t<0, UnderlyingIterators...>>::difference_type,
                              const std::tuple<typename std::iterator_traits<UnderlyingIterators>::reference...>*,
                              const std::tuple<typename std::iterator_traits<UnderlyingIterators>::reference...>&
                          >
{
public:
	zip_iterator(UnderlyingIterators... iterators) : iterators_(std::make_tuple(iterators...)) {}
	zip_iterator operator++() {for_each_tuple(iterators_, [](auto& it){++it;}); return *this;}
	zip_iterator& operator=(const zip_iterator& other) { iterators_ = other.iterators_;}
	auto operator*() { return transform_tuple(iterators_, [](auto const& it){return *it;});}
	bool operator==(const zip_iterator& other){ return iterators_ == other.iterators_;}
	bool operator!=(const zip_iterator& other){ return !(*this == other); }
	auto operator-(const zip_iterator& other) { return std::get<0>(iterators_) - std::get<0>(other.iterators_);}

private:
	std::tuple<UnderlyingIterators...> iterators_;
};

template<typename... Iterators>
zip_iterator<Iterators...> make_zip_iterator(Iterators... underlyingIterators)
{
	return zip_iterator<Iterators...>(underlyingIterators...);
}

namespace view
{

template<typename... Ranges>
iterator_range<zip_iterator<typename Ranges::const_iterator...>> zip(Ranges const&... ranges)
{
	return iterator_range<zip_iterator<typename Ranges::const_iterator...>>(
			make_zip_iterator(ranges.begin()...),
			make_zip_iterator(ranges.end()...)
		);
}
    
} // namespace view
} // namespace ranges


#endif
