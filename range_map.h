#pragma once

#include <map>
#include <unordered_map>

#include <boost/container/flat_map.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <boost/type_traits.hpp>

#include "range_util.h"

namespace range {
	// Rangeをmap化するto_mapを提供する

	/// INTERNAL ONLY
	///
	// {
		template <typename T>
		struct _get_direct {
			const T &operator()(const T &t) const { return t; }
		};

		// 要素をstd::pairに変換する関数オブジェクト
		// NOTE : transformedにラムダ式を渡せるようになれば要らなくなる
		template <
			typename T,
			typename FirstSelector,
			typename SecondSelector
		>
		struct _to_pair {

			FirstSelector fs;
			SecondSelector ss;

			// NOTE : transformedに型を教えるのに必要
			typedef std::pair<
				decltype(fs(std::declval<T>())),
				decltype(ss(std::declval<T>()))
			> result_type;

			_to_pair(FirstSelector fs, SecondSelector ss)
				: fs(fs), ss(ss)
			{}

			inline auto operator()(const T &x) const
				-> std::pair<decltype(fs(x)), decltype(ss(x))>
			{
				return { fs(x), ss(x) };
			}
		};
		
#define DEFINE_TYPE(range, selector) typename std::decay<decltype(selector(*std::begin(range)))>::type
		
		// std::mapに変換する
		template <
			typename Range,
			typename KeySelector = _get_direct<rangedecltype(std::declval<Range>())>,
			typename ValueSelector = _get_direct<rangedecltype(std::declval<Range>())>
		>
		inline auto _to_map(const Range &r, KeySelector ks = KeySelector(), ValueSelector vs = ValueSelector())
		-> std::map<DEFINE_TYPE(r, ks), DEFINE_TYPE(r, vs)>
		{
			auto pairs = r
				| boost::adaptors::transformed(
					_to_pair<rangedecltype(r), decltype(ks), decltype(vs)>(ks, vs)
				);
			return { std::begin(pairs), std::end(pairs) };
		}
		// std::mapに変換する
		// @throw : keyの重複時にinvalid_argument
		template <
			typename Range,
			typename KeySelector = _get_direct<rangedecltype(std::declval<Range>())>,
			typename ValueSelector = _get_direct<rangedecltype(std::declval<Range>())>
		>
		inline auto _to_map_throw_duplicate(const Range &r, KeySelector ks = KeySelector(), ValueSelector vs = ValueSelector())
			-> decltype(_to_map(r, ks, vs))
		{
			auto m = _to_map(r, ks, vs);
			if (m.size() != r.size()) throw std::invalid_argument("key is duplication");
			return m;
		}

		// std::multimapに変換する
		template <
			typename Range,
			typename KeySelector = _get_direct<rangedecltype(std::declval<Range>())>,
			typename ValueSelector = _get_direct<rangedecltype(std::declval<Range>())>
		>
		inline auto _to_multimap(const Range &r, KeySelector ks = KeySelector(), ValueSelector vs = ValueSelector())
			-> std::multimap<DEFINE_TYPE(r, ks), DEFINE_TYPE(r, vs)>
		{
			auto pairs = r
				| boost::adaptors::transformed(
					_to_pair<rangedecltype(r), decltype(ks), decltype(vs)>(ks, vs)
				);
			return { std::begin(pairs), std::end(pairs) };
		}
		// std::unordered_mapに変換する
		template <
			typename Range,
			typename KeySelector = _get_direct<rangedecltype(std::declval<Range>())>,
			typename ValueSelector = _get_direct<rangedecltype(std::declval<Range>())>
		>
		inline auto _to_unordered_map(const Range &r, KeySelector ks = KeySelector(), ValueSelector vs = ValueSelector())
			-> std::unordered_map<DEFINE_TYPE(r, ks), DEFINE_TYPE(r, vs)>
		{

			auto pairs = r
				| boost::adaptors::transformed(
					_to_pair<rangedecltype(r), decltype(ks), decltype(vs)>(ks, vs)
				);
			return { std::begin(pairs), std::end(pairs) };
		}
		// std::unordered_mapに変換する
		// @throw : keyの重複時にinvalid_argument
		template <
			typename Range,
			typename KeySelector = _get_direct<rangedecltype(std::declval<Range>())>,
			typename ValueSelector = _get_direct<rangedecltype(std::declval<Range>())>
		>
		inline auto _to_unordered_map_throw_duplicate(const Range &r, KeySelector ks = KeySelector(), ValueSelector vs = ValueSelector())
			-> decltype(_to_unordered_map(r, ks, vs))
		{
			auto m = _to_unordered_map(r, ks, vs);
			if (m.size() != r.size()) throw std::invalid_argument("key is duplication");
			return m;
		}

		// boost::container::flat_mapに変換する
		template <
			typename Range,
			typename KeySelector = _get_direct<rangedecltype(std::declval<Range>())>,
			typename ValueSelector = _get_direct<rangedecltype(std::declval<Range>())>
		>
		inline auto _to_flat_map(const Range &r, KeySelector ks = KeySelector(), ValueSelector vs = ValueSelector())
			-> boost::container::flat_map<DEFINE_TYPE(r, ks), DEFINE_TYPE(r, vs)>
		{
			auto pairs = r
				| boost::adaptors::transformed(
					_to_pair<rangedecltype(r), decltype(ks), decltype(vs)>(ks, vs)
				);
			return { std::begin(pairs), std::end(pairs) };
		}
		// boost::container::flat_mapに変換する
		// @throw : keyの重複時にinvalid_argument
		template <
			typename Range,
			typename KeySelector = _get_direct<rangedecltype(std::declval<Range>())>,
			typename ValueSelector = _get_direct<rangedecltype(std::declval<Range>())>
		>
		inline auto _to_flat_map_throw_duplicate(const Range &r, KeySelector ks = KeySelector(), ValueSelector vs = ValueSelector())
			-> decltype(_to_flat_map(r, ks, vs))
		{
			auto m = _to_flat_map(r, ks, vs);
			if (m.size() != r.size()) throw std::invalid_argument("key is duplication");
			return m;
		}

#undef DEFINE_TYPE
	// }

	// mapped関数の実装に必要な定義を行う
	// function_name        : to_map関数名
	// __holder_name__      : operator| と関数の橋渡しをするHolder名
	// to_map_function_name : 実際に変換を行う関数
#define DEFINE_MAPPED(function_name, __holder_name__, to_map_function_name)                  \
	/** INTERNAL ONLY */                                                                     \
	/** { */                                                                                 \
		/* operator| に渡す為のHolderを定義する */                                           \
		template <class KeySelector, class ValueSelector>                                    \
		struct _ ## __holder_name__ ## KV {                                                  \
			_ ## __holder_name__ ## KV(KeySelector ks, ValueSelector vs) : ks(ks), vs(vs) {} \
																							 \
			KeySelector ks;                                                                  \
			ValueSelector vs;                                                                \
		};                                                                                   \
																							 \
		template <class KeySelector>                                                         \
		struct _ ## __holder_name__ ## K {                                                   \
			_ ## __holder_name__ ## K(KeySelector ks) : ks(ks) {}                            \
			KeySelector ks;                                                                  \
		};                                                                                   \
																							 \
		struct _ ## __holder_name__ ## Null {};                                              \
	/** } */                                                                                 \
	/* 実際に使う関数を定義する */                                                                                    \
	/* operator| に渡す為のHolderを返す */                                                                            \
	template <class KeySelector, class ValueSelector>                                                                 \
	inline _ ## __holder_name__ ## KV<KeySelector, ValueSelector> function_name(KeySelector &&ks, ValueSelector &&vs) \
	{                                                                                                                 \
		return _ ## __holder_name__ ## KV<KeySelector, ValueSelector>(ks, vs);                                        \
	}                                                                                                                 \
																											          \
	template <class KeySelector>                                                                                      \
	inline _ ## __holder_name__ ## K<KeySelector> function_name(KeySelector &&ks)                                     \
	{                                                                                                                 \
		return _ ## __holder_name__ ## K<KeySelector>(ks);                                                            \
	}                                                                                                                 \
																											          \
	inline _ ## __holder_name__ ## Null function_name()                                                               \
	{                                                                                                                 \
		return _ ## __holder_name__ ## Null();                                                                        \
	}                                                                                                                 \
	/* operator| の実処理 */                                                                                          \
	/* Holderを受け取りoperator| の左辺値をmapに変換する関数を呼び出す */                                             \
	template <typename Range, class KeySelector, class ValueSelector>                                                 \
	inline auto operator| (Range &r, const _ ## __holder_name__ ## KV<KeySelector, ValueSelector> &v)                 \
		-> decltype(to_map_function_name(r, v.ks, v.vs))                                                              \
	{                                                                                                                 \
		return to_map_function_name(r, v.ks, v.vs);                                                                   \
	}                                                                                                                 \
	template <typename Range, class KeySelector, class ValueSelector>                                                 \
	inline auto operator| (const Range &r, const _ ## __holder_name__ ## KV<KeySelector, ValueSelector> &v)           \
		-> decltype(to_map_function_name(r, v.ks, v.vs))                                                              \
	{                                                                                                                 \
		return to_map_function_name(r, v.ks, v.vs);                                                                   \
	}                                                                                                                 \
																											          \
	template <typename Range, class KeySelector>                                                                      \
	inline auto operator| (Range &r, const _ ## __holder_name__ ## K<KeySelector> &v)                                 \
		-> decltype(to_map_function_name(r, v.ks))                                                                    \
	{                                                                                                                 \
		return to_map_function_name(r, v.ks);                                                                         \
	}                                                                                                                 \
	template <typename Range, class KeySelector>                                                                      \
	inline auto operator| (const Range &r, const _ ## __holder_name__ ## K<KeySelector> &v)                           \
		-> decltype(to_map_function_name(r, v.ks))                                                                    \
	{                                                                                                                 \
		return to_map_function_name(r, v.ks);                                                                         \
	}                                                                                                                 \
																											          \
	template <typename Range>                                                                                         \
	inline auto operator| (Range &r, const _ ## __holder_name__ ## Null &)                                            \
		-> decltype(to_map_function_name(r))                                                                          \
	{                                                                                                                 \
		return to_map_function_name(r);                                                                               \
	}                                                                                                                 \
	template <typename Range>                                                                                         \
	inline auto operator| (const Range &r, const _ ## __holder_name__ ## Null &)                                      \
		-> decltype(to_map_function_name(r))                                                                          \
	{                                                                                                                 \
		return to_map_function_name(r);                                                                               \
	}

	// rangeをそのまま各mapに変換する実装
	// NOTE : コンストラクタにbeginとendを渡してる
	DEFINE_MAPPED(          to_map,          MapHolder,           _to_map)
	DEFINE_MAPPED(     to_multimap,     MultiMapHolder,      _to_multimap)
	DEFINE_MAPPED(to_unordered_map, UnorderedMapHolder, _to_unordered_map)
	DEFINE_MAPPED(     to_flat_map,      FlatMapHolder,      _to_flat_map)

	// rangeを各mapに変換する際、keyの重複があったら例外を投げる実装
	DEFINE_MAPPED(          to_map_throw_duplicate,          MapHolderThrowDuplicate,           _to_map_throw_duplicate)
	DEFINE_MAPPED(to_unordered_map_throw_duplicate, UnorderedMapHolderThrowDuplicate, _to_unordered_map_throw_duplicate)
	DEFINE_MAPPED(     to_flat_map_throw_duplicate,      FlatMapHolderThrowDuplicate,      _to_flat_map_throw_duplicate)

#undef DEFINE_MAPPED
}

