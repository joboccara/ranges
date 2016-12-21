#ifndef VIEW_CARTESIAN_PRODUCT_HPP
#define VIEW_CARTESIAN_PRODUCT_HPP

/* range view adaptor to represent all possible combinations of a number of ranges
   Applying it on 3 ranges containing:
   { a,b }
   { 1,2 }
   { X,Y,Z }
   produces a view seeing:
   {
     std::tuple(a,1,X), std::tuple(a,1,Y), std::tuple(a,1,Z),
     std::tuple(a,2,X), std::tuple(a,2,Y), std::tuple(a,2,Z),
     std::tuple(b,1,X), std::tuple(b,1,Y), std::tuple(b,1,Z),
     std::tuple(b,2,X), std::tuple(b,2,Y), std::tuple(b,2,Z),
   }
*/

#include "iterator_range.hpp"
#include "named_type.hpp"
#include "variadic.hpp"
#include <iterator>

namespace ranges
{

template<typename Iterator>
using BeginIteratorRef = NamedTypeRef<Iterator, struct begin_context>;
template<typename Iterator>
using EndIteratorRef = NamedTypeRef<Iterator, struct end_context>;

template<size_t n, typename... Iterators>
struct Advance_cartesian_impl
{
	void operator()(std::tuple<Iterators...>& iterators, std::tuple<Iterators...> const& begins, std::tuple<Iterators...> const& ends)
	{
		if(++std::get<n>(iterators) == std::get<n>(ends))
		{
			std::get<n>(iterators) = std::get<n>(begins);
			Advance_cartesian_impl<n-1, Iterators...>()(iterators, begins, ends);
		}
	}
};

template<typename... Iterators>
struct Advance_cartesian_impl<0, Iterators...>
{
	void operator()(std::tuple<Iterators...>& iterators, std::tuple<Iterators...> const&, std::tuple<Iterators...> const&)
	{
		++std::get<0>(iterators);
	}
};

template<typename... Iterators>
void advance_cartesian(std::tuple<Iterators...>& iterators, std::tuple<Iterators...> const& begins, std::tuple<Iterators...> const& ends)
{
	Advance_cartesian_impl<sizeof...(Iterators)-1, Iterators...>()(iterators, begins, ends);
}

template<size_t n, typename... Iterators>
struct Distance_cartesian_impl
{
	size_t operator()(std::tuple<Iterators...> const& iterators1, std::tuple<Iterators...> const& iterators2, std::tuple<Iterators...> const& begins, std::tuple<Iterators...> const& ends)
	{
		return Distance_cartesian_impl<n-1, Iterators...>()(iterators1, iterators2, begins, ends) * (std::get<n>(ends) - std::get<n>(begins)) + std::get<n>(iterators2) - std::get<n>(iterators1);
	}
};

template<typename... Iterators>
struct Distance_cartesian_impl<0, Iterators...>
{
	size_t operator()(std::tuple<Iterators...> const& iterators1, std::tuple<Iterators...> const& iterators2, std::tuple<Iterators...> const&, std::tuple<Iterators...> const&)
	{
		return std::get<0>(iterators2) - std::get<0>(iterators1);
	}
};

template<typename... Iterators>
size_t distance_cartesian(std::tuple<Iterators...> const& iterators1, std::tuple<Iterators...> const& iterators2, std::tuple<Iterators...> const& begins, std::tuple<Iterators...> const& ends)
{
	return Distance_cartesian_impl<sizeof...(Iterators)-1, Iterators...>()(iterators1, iterators2, begins, ends);
}

struct begin_tag{};
struct end_tag{};

template<typename... Ranges>
class cartesian_product_iterator
	: public std::iterator<
                              typename std::iterator_traits<get_type_t<0, typename Ranges::const_iterator...>>::iterator_category,
                              std::tuple<typename std::iterator_traits<typename Ranges::const_iterator>::reference...>,
                              typename std::iterator_traits<get_type_t<0, typename Ranges::const_iterator...>>::difference_type,
                              const std::tuple<typename std::iterator_traits<typename Ranges::const_iterator>::reference...>*,
                              const std::tuple<typename std::iterator_traits<typename Ranges::const_iterator>::reference...>&
                          >
{
public:
	cartesian_product_iterator(BeginIteratorRef<Ranges const>... ranges) // can also be implemented with begin_tag instead of strong type wrapper
		: iterators_(std::make_tuple(ranges.get().begin()...))
		, begins_(std::make_tuple(ranges.get().begin()...))
		, ends_(std::make_tuple(ranges.get().end()...)) {}
	cartesian_product_iterator(EndIteratorRef<Ranges const>... ranges)
		: iterators_(std::make_tuple(ranges.get().begin()...))
		, begins_(std::make_tuple(ranges.get().begin()...))
		, ends_(std::make_tuple(ranges.get().end()...)) {std::get<0>(iterators_) = get_value<0>(ranges.get()...).end();}
	cartesian_product_iterator operator++() {advance_cartesian(iterators_, begins_, ends_); return *this;}
	auto operator*() { return transform_tuple(iterators_, [](auto const& it){return *it;});}
	bool operator==(const cartesian_product_iterator& other){ return iterators_ == other.iterators_; }
	bool operator!=(const cartesian_product_iterator& other){ return !(*this == other); }
	auto operator-(const cartesian_product_iterator& other) { return distance_cartesian(other.iterators_, iterators_, begins_, ends_);}

private:
	std::tuple<typename Ranges::const_iterator...> iterators_;
	std::tuple<typename Ranges::const_iterator...> begins_;
	std::tuple<typename Ranges::const_iterator...> ends_;
};

namespace view
{

template<typename... Ranges>
auto cartesian_product(Ranges const&... ranges)
{
	return iterator_range<cartesian_product_iterator<Ranges...>>(
		cartesian_product_iterator<Ranges...>(BeginIteratorRef<Ranges const>(ranges)...),
		cartesian_product_iterator<Ranges...>(EndIteratorRef<Ranges const>(ranges)...)
		);
}
    
} // namespace view
} // namespace ranges

#endif
