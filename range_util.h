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
// �z��v�f�̌^�擾
#define rangedecltype(r) decltype(_rangetype(r))

namespace boost {
	// indirected��boost::optional���g����悤�ɂ�����ꉻ
	// http://faithandbrave.hateblo.jp/entry/20110526/1306391843
	template <class T>
	struct pointee<optional<T>> : mpl::identity<T> {};

}
