#pragma once

#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include "range_util.h"

namespace range {
	
	// Rangeの要素を、iteratorにして返す
	// usage)
	//   std::vector<int> v = {1, 2, 3};
	//   for (std::vector<int>::iterator it : v | range::outdirected()) { }

	/// INTERNAL ONLY
	/// 
	// {
		struct _OutDirectHolder { };
	// }
	
	_OutDirectHolder outdirected() { return _OutDirectHolder(); }
	
	template <typename Range>
	inline auto operator| (Range &r, const _OutDirectHolder &)
		-> boost::iterator_range<decltype(boost::make_counting_iterator(std::begin(r)))>
	{
		return {
			boost::make_counting_iterator(std::begin(r)),
			boost::make_counting_iterator(std::end(r))
		};
	}

	template <typename Range>
	inline auto operator| (const Range &r, const _OutDirectHolder &)
		-> boost::iterator_range<decltype(boost::make_counting_iterator(std::begin(r)))>
	{
		return {
			boost::make_counting_iterator(std::begin(r)),
			boost::make_counting_iterator(std::end(r))
		};
	}
}
