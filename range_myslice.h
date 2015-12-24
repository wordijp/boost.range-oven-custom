#pragma once

#include <iterator>

#include <boost/range/iterator_range.hpp>

#include "range_util.h"

namespace range {
	
	// Rangeから[begin, end)の範囲を取り出す
	// NOTE : slicedが適用出来ない時に使う
	//        (filtered後等)
	
	// usage)
	//   std::vector<int> v = {1, 2, 3};
	//   auto q = v | range::mysliced(1, 3);  // {2, 3}

	/// INTERNAL ONLY
	/// 
	// {
		struct _SliceHolder {
			_SliceHolder(size_t begin, size_t end)
				: begin(begin)
				, end(end)
			{}
			
			size_t begin;
			size_t end;
		};
		
		template <typename Range>
		inline auto _sliced(Range &r, size_t begin, size_t end)
			-> 	boost::iterator_range<decltype(std::begin(r))>
		{
			auto itBegin = std::begin(r);
			auto itEnd = std::begin(r);
			std::advance(itBegin, begin);
			std::advance(itEnd, end);

			return { itBegin, itEnd	};
		}

	// }
	
	// TODO : 使う時は名前を変える
	_SliceHolder mysliced(size_t begin, size_t end) { return _SliceHolder(begin, end); }
	
	template <typename Range>
	inline auto operator| (Range &r, const _SliceHolder &s)
		-> decltype(_sliced(r, s.begin, s.end))
	{
		return _sliced(r, s.begin, s.end);
	}

	template <typename Range>
	inline auto operator| (const Range &r, const _SliceHolder &s)
		-> decltype(_sliced(r, s.begin, s.end))
	{
		return _sliced(r, s.begin, s.end);
	}
}
