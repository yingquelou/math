#pragma once
#include<tuple>
namespace templateutils
{
	template <typename T, typename... Ts>
	using is_one_of = std::disjunction<std::is_same<T, Ts>...>;
	template <typename T, typename... Ts>
	static constexpr const bool is_one_of_v = is_one_of<T, Ts...>::value;
	template<typename...>
	struct tuple_in;
	template<typename T, typename...Ts>
	struct tuple_in<T, std::tuple<Ts...>> :is_one_of<T, Ts...>
	{

	};
	template<typename...>
	struct tuple_cat;
	template<typename... Ts1, typename...Ts2>
	struct tuple_cat <std::tuple<Ts1...>, std::tuple<Ts2...>>
	{
		using type = std::tuple<Ts1..., Ts2...>;
	};
	template<typename...>
	struct tuple_push_back;
	template<typename T, typename...Ts>
	struct tuple_push_back <std::tuple<Ts...>, T>
	{
		using type = std::tuple<Ts..., T>;
	};
	// ---tuple_init---
	template<typename Rs, typename...>
	struct _tuple_init_imp
	{
		using type = Rs;
	};
	template<typename Rs, typename T, typename... Ts>
	struct _tuple_init_imp<Rs, std::tuple<T, Ts...>>
	{
		using type = std::conditional_t<tuple_in<T, Rs>::value, typename _tuple_init_imp<Rs, std::tuple<Ts...>>::type,
			typename _tuple_init_imp<typename tuple_push_back<Rs, T>::type, std::tuple<Ts...>>::type>;
	};
	// 创建类型集合
	template<typename...Ts>
	struct tuple_init {
		using type = typename _tuple_init_imp<std::tuple<>, std::tuple<std::remove_cv_t<Ts>...>>::type;
	};

	// 并集
	template<typename...>
	struct tuple_u;

	template<template<typename...>typename V1, typename... Ts1, template<typename...>typename V2, typename... Ts2>
	struct tuple_u<V1<Ts1...>, V2<Ts2...>> {
		using type = typename tuple_init<std::remove_cv_t<Ts1>..., std::remove_cv_t<Ts2>...>::type;
	};
	// ---tuple_n---
	template<typename Rs, typename...>
	struct _tuple_n_imp
	{
		using type = Rs;
	};
	template<typename Rs, typename Ss, typename T, typename...Ts>
	struct _tuple_n_imp<Rs, Ss, std::tuple<T, Ts...>>
	{
		using type = std::conditional_t<tuple_in<T, Ss>::value, typename _tuple_n_imp<typename tuple_push_back<Rs, T>::type, Ss, std::tuple<Ts...>>::type,
			typename _tuple_n_imp<Rs, Ss, std::tuple<Ts...>>::type>;
	};
	// 交集
	template<typename...>
	struct tuple_n;
	template<template<typename...>typename V1, typename... Ts1, template<typename...>typename V2, typename... Ts2>
	struct tuple_n<V1<Ts1...>, V2<Ts2...>> {
		using type = typename _tuple_n_imp<std::tuple<>, std::tuple<std::remove_cv_t<Ts1>...>, std::tuple<std::remove_cv_t<Ts2>...>>::type;
	};
	// ---tuple_x---
	template<typename Rs, typename...>
	struct _tuple_x_imp
	{
		using type = Rs;
	};
	template<typename Rs, typename Ss, typename T, typename...Ts>
	struct _tuple_x_imp<Rs, Ss, std::tuple<T, Ts...>>
	{
		using type = std::conditional_t<tuple_in<T, Ss>::value, typename _tuple_x_imp<Rs, Ss, std::tuple<Ts...>>::type,
			typename _tuple_x_imp<typename tuple_push_back<Rs, T>::type, Ss, std::tuple<Ts...>>::type>;
	};
	// 差集
	template<typename...>
	struct tuple_x;
	template<template<typename...>typename V1, typename... Ts1, template<typename...>typename V2, typename... Ts2>
	struct tuple_x<V1<Ts1...>, V2<Ts2...>> {
		using type = typename _tuple_x_imp<std::tuple<>, std::tuple<std::remove_cv_t<Ts2>...>, std::tuple<std::remove_cv_t<Ts1>...>>::type;
	};
}
using templateutils::tuple_init;
using templateutils::tuple_u;
using templateutils::tuple_n;
using templateutils::tuple_x;