#ifndef VARIADIC_HPP
#define VARIADIC_HPP

#include <tuple>
#include <utility>

template<typename Res, typename Arg1, typename Arg2>
auto paired_args(Res(*func)(Arg1, Arg2))
{
	return [func](std::pair<const Arg1&, const Arg2&> args){return func(args.first, args.second);};
}

template<typename Function, typename Args, size_t... index>
auto tupled_args_impl(Function func, Args const& args, std::index_sequence<index...>)
{
    return func(std::get<index>(args)...);
}

template<typename Res, typename... Args>
auto tupled_args(Res(*func)(Args...))
{
	return [func](std::tuple<const Args&...> args)
	{
	    return tupled_args_impl(func, args, std::make_index_sequence<sizeof...(Args)>{});
	};
}

template<typename Tuple, typename Function, size_t... index>
auto transform_tuple_impl(Tuple const& input, Function func, std::index_sequence<index...>)
{
	return std::make_tuple(func(std::get<index>(input))...);
}

template<typename Function, typename... TupleElements>
auto transform_tuple(std::tuple<TupleElements...> const& input, Function func)
{
	return transform_tuple_impl(input, func, std::make_index_sequence<sizeof...(TupleElements)>());
}

template<size_t n, typename Function, typename... TupleElements>
struct For_each_tuple_impl
{
    Function operator()(std::tuple<TupleElements...>& input, Function& func)
    {
        func(std::get<n-1>(input));
        For_each_tuple_impl<n-1, Function, TupleElements...>()(input, func);
        return func;
    }
};

template<typename Function, typename... TupleElements>
struct For_each_tuple_impl<1, Function, TupleElements...>
{
    Function operator()(std::tuple<TupleElements...>& input, Function& func)
    {
        func(std::get<0>(input));
        return func;
    }
};

template<typename Function, typename... TupleElements>
struct For_each_tuple_impl<0, Function, TupleElements...>
{
    Function operator()(std::tuple<TupleElements...>&, Function& func)
    {
        return func;
    }
};

template<typename Function, typename... TupleElements>
Function for_each_tuple(std::tuple<TupleElements...>& input, Function func)
{
    For_each_tuple_impl<sizeof...(TupleElements), Function, TupleElements...>()(input, func);
    return func;
}

template<size_t n, typename T1, typename... Ts>
struct get_type
{
    using type = typename get_type<n-1, Ts...>::type;
};

template<typename T1, typename... Ts>
struct get_type<0, T1, Ts...>
{
    using type = T1;
};

template<size_t n, typename... Ts>
using get_type_t = typename get_type<n, Ts...>::type;

template<size_t n, typename T, typename... Ts>
struct Get_value
{
	auto& operator()(T&&, Ts&&... ts)
	{
		return Get_value<n-1, Ts...>()(std::forward<Ts>(ts)...);
	}
};

template<typename T, typename... Ts>
struct Get_value<0, T, Ts...>
{
	auto& operator()(T&& t, Ts&&...)
	{
		return t;
	}
};

template<size_t n, typename T, typename... Ts>
auto& get_value(T&& t, Ts&&... ts)
{
	return Get_value<n, T, Ts...>()(std::forward<T>(t), std::forward<Ts>(ts)...);
}


#endif
