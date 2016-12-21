#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <algorithm>

template<typename Range, typename Iterator>
Iterator copy(Range const& range, Iterator output)
{
	return std::copy(range.begin(), range.end(), output);
}

template<typename Range, typename Function>
Function for_each(Range const& range, Function func)
{
	return std::for_each(range.begin(), range.end(), func);
}

namespace ranges
{

template<typename DestinationContainer, typename SourceRange>
void push_back(DestinationContainer& destination, SourceRange const& source)
{
    copy(source, std::back_inserter(destination));
}
    
}

#endif
