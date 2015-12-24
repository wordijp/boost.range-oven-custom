#pragma once

#include "range_util.h"

#include <boost/range/join.hpp>

namespace range {
	
	// “ñ‚Â‚ÌRange‚ðŒq‚°‚Ä•Ô‚·

	/// INTERNAL ONLY
	/// 
	// {
		template <typename Range>
		struct _JoinHolder {
			_JoinHolder(Range &r) : r(r) {}
			Range &r;
		};
	// }

	template <typename Range>
	_JoinHolder<Range> joined(Range &r) {
		return _JoinHolder<Range>(r);
	}

	template <typename LhsRange, typename RhsRange>
	inline auto operator| (LhsRange &r, const _JoinHolder<RhsRange> &j)
		-> decltype(boost::join(r, j.r))
	{
		return boost::join(r, j.r);
	}

	template <typename LhsRange, typename RhsRange>
	inline auto operator| (const LhsRange &r, const _JoinHolder<RhsRange> &j)
		-> decltype(boost::join(r, j.r))
	{
		return boost::join(r, j.r);
	}
}

