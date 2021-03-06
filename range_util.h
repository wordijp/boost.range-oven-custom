#pragma once

#include <type_traits>
#include <iterator>

#include <boost/optional.hpp>
#include <boost/pointee.hpp>
#include <boost/mpl/identity.hpp>

template <class Range>
auto _rangetype(Range&& r) ->
	typename std::decay<
		decltype(*std::begin(r))
	>::type;
// 配列要素の型取得
#define rangedecltype(r) decltype(_rangetype(r))

namespace boost {
	// indirectedでboost::optionalを使えるようにする特殊化
	// http://faithandbrave.hateblo.jp/entry/20110526/1306391843
	template <class T>
	struct pointee<optional<T>> : mpl::identity<T> {};

}
