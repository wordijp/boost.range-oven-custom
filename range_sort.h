#pragma once

#include <algorithm>
#include <type_traits>

#include "range_outplace.h"
#include "range_util.h"

#include "detail/shared_range_iterator.h"

#include <boost/range/adaptor/indirected.hpp>

namespace range {
	
	// Rangeをsortして返す
	/// INTERNAL ONLY
	/// 
	// {
		template <typename T>
		struct _less {
			inline bool operator()(const T &lhs, const T &rhs) const {
				return lhs < rhs;
			}
		};
		
		template <typename It, class Compare>
		struct _compare_out {

			_compare_out(Compare cmp)
				: cmp(cmp)
			{}

			inline bool operator()(const It &lhs, const It &rhs) const {
				return cmp(**lhs, **rhs);
			}
			Compare cmp;
		};

		// Rangeをソートして返す(不安定ソート)
		template <
			typename Range,
			typename Compare = _less<rangedecltype(std::declval<Range>())>
		>
		inline auto _sorted(Range &r, Compare c = Compare())
			-> decltype(r | range::outplaced() | range::outplaced() | range::indirected | range::indirected)
		{
			auto q = r | range::outplaced() | range::outplaced();
			std::sort(std::begin(q), std::end(q), _compare_out<rangedecltype(q), decltype(c)>(c));
			return typename std::remove_const<decltype(q)>::type(q) | range::indirected | indirected;
		}
		// Rangeをソートして返す(安定ソート)
		template <
			typename Range,
			typename Compare = _less<rangedecltype(std::declval<Range>())>
		>
		inline auto _stable_sorted(Range &r, Compare c = Compare())
			-> decltype(r | range::outplaced() | range::outplaced() | range::indirected | range::indirected)
		{
			auto q = r | range::outplaced() | range::outplaced();
			std::stable_sort(std::begin(q), std::end(q), _compare_out<rangedecltype(q), decltype(c)>(c));
			return typename std::remove_const<decltype(q)>::type(q) | range::indirected | indirected;
		}
	// }
		
#define DEFINE_SORTED(function_name, __holder_name__, to_sort_function_name) \
	/** INTERNAL ONLY */                                                     \
	/** { */                                                                 \
		/* operator| に渡す為のHolderを定義する */                           \
		template <class Compare>                                             \
		struct _ ## __holder_name__ {                                        \
			_ ## __holder_name__(Compare cmp) : cmp(cmp) {}                  \
			Compare cmp;                                                     \
		};                                                                   \
																			 \
		struct _ ## __holder_name__ ## Null {};                              \
	/** } */                                                                 \
	/* 実際に使う関数を定義する */                                                 \
	/* operator| に渡す為のHolderを返す */                                         \
	template <class Compare>                                                       \
	inline _ ## __holder_name__<Compare> function_name(Compare &&cmp)              \
	{                                                                              \
		return _ ## __holder_name__<Compare>(cmp);                                 \
	}                                                                              \
																				   \
	inline _ ## __holder_name__ ## Null function_name()                            \
	{                                                                              \
		return _ ## __holder_name__ ## Null();                                     \
	}                                                                              \
	/* operator| の実処理 */                                                       \
	/* Holderを受け取りoperator| の左辺値をsortする関数を呼び出す */               \
	template <typename Range, class Compare>                                       \
	inline auto operator| (Range &r, const _ ## __holder_name__<Compare> &v)       \
		-> decltype(to_sort_function_name(r, v.cmp))                               \
	{                                                                              \
		return to_sort_function_name(r, v.cmp);                                    \
	}                                                                              \
	template <typename Range, class Compare>                                       \
	inline auto operator| (const Range &r, const _ ## __holder_name__<Compare> &v) \
		-> decltype(to_sort_function_name(r, v.cmp))                               \
	{                                                                              \
		return to_sort_function_name(r, v.cmp);                                    \
	}                                                                              \
																				   \
	template <typename Range>                                                      \
	inline auto operator| (Range &r, const _ ## __holder_name__ ## Null &)         \
		-> decltype(to_sort_function_name(r))                                      \
	{                                                                              \
		return to_sort_function_name(r);                                           \
	}                                                                              \
	template <typename Range>                                                      \
	inline auto operator| (const Range &r, const _ ## __holder_name__ ## Null &)   \
		-> decltype(to_sort_function_name(r))                                      \
	{                                                                              \
		return to_sort_function_name(r);                                           \
	}
		
	DEFINE_SORTED(       sorted,       SortHolder,        _sorted)
	DEFINE_SORTED(stable_sorted, StableSortHolder, _stable_sorted)
	
#undef DEFINE_SORTED
}
