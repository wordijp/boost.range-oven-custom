#pragma once

#include "range_outdirect.h"
#include "range_util.h"

#include "detail/shared_range_iterator.h"

#include <boost/shared_ptr.hpp>

// 参考 : pstade outplaced.hpp

namespace range {
	
	// RangeをiteratorのRangeにして返す
	// NOTE : 配列のインスタンスをRangeに持つ

	/// INTERNAL ONLY
	/// 
	// {
		struct _OutPlacedHolder { };
		
		template <class RangePtr>
		inline auto _make_shared_iterator_range(RangePtr pr)
			-> boost::iterator_range<
				typename detail::shared_range_iterator<
					typename boost::shared_ptr< typename std::decay<decltype(*pr)>::type >
				>
			>
		{
			typedef typename std::decay<decltype(*pr)>::type range_type;
			auto spr = boost::shared_ptr<range_type>(pr);
			return {
				detail::shared_range_iterator<decltype(spr)>(std::begin(*spr), spr),
				detail::shared_range_iterator<decltype(spr)>(std::end(*spr), spr),
			};
		}

		template <typename Range>
		struct _iter_sequence {
			typedef std::vector<
				typename boost::range_iterator<Range>::type
			> type;
		};
	// }
	
	_OutPlacedHolder outplaced() { return _OutPlacedHolder(); }
	
	template <typename Range>
	inline auto operator| (Range &r, const _OutPlacedHolder &)
		-> decltype(_make_shared_iterator_range(
			new typename _iter_sequence<Range>::type(
				std::begin(r | range::outdirected()),
				std::end(r | range::outdirected())
			)
		))
	{
		auto q = r | range::outdirected();
		return _make_shared_iterator_range(
			new typename _iter_sequence<Range>::type(std::begin(q), std::end(q))
		);
	}
	
	template <typename Range>
	inline auto operator| (const Range &r, const _OutPlacedHolder &)
		-> decltype(_make_shared_iterator_range(
			new typename _iter_sequence<Range>::type(
				std::begin(r | range::outdirected()),
				std::end(r | range::outdirected())
			)
		))
	{
		auto q = r | range::outdirected();
		return _make_shared_iterator_range(
			new typename _iter_sequence<Range>::type(std::begin(q), std::end(q))
		);
	}
}
