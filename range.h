#pragma once

#include <vector>
#include <boost/range/adaptors.hpp>
#include "range_util.h"
	
namespace range {
	using namespace boost::adaptors;

	//------------------------------
	// std::vector‚É•ÏŠ·‚·‚é
	/// INTERNAL ONLY
	/// 
	// {
		struct _VectorHolder {};
	// }
	
	_VectorHolder to_vector() { return _VectorHolder(); }
	
	template <typename Range>
	inline auto operator| (Range &r, const _VectorHolder &)
		-> std::vector<rangedecltype(r)>
	{
		return { std::begin(r), std::end(r) };
	}

	template <typename Range>
	inline auto operator| (const Range &r, const _VectorHolder &)
		-> std::vector<rangedecltype(r)>
	{
		return { std::begin(r), std::end(r) };
	}
	
	//------------------------------
	// true”»’è‚ğæ‚èo‚·
	/// INTERNAL ONLY
	/// 
	// {
		struct _FilteredTrueHolder {};
		
		template <typename T>
		struct _is_true {
			inline bool operator()(const T &t) const {
				return (t) ? true : false;
			}
		};
	// }

	_FilteredTrueHolder filtered_true() { return _FilteredTrueHolder(); }

	template <typename Range>
	inline auto operator| (Range &r, const _FilteredTrueHolder &)
		-> decltype(r | boost::adaptors::filtered(_is_true<rangedecltype(r)>()))
	{
		return r | boost::adaptors::filtered(_is_true<rangedecltype(r)>());
	}

	template <typename Range>
	inline auto operator| (const Range &r, const _FilteredTrueHolder &)
		-> decltype(r | boost::adaptors::filtered(_is_true<rangedecltype(r)>()))
	{
		return r | boost::adaptors::filtered(_is_true<rangedecltype(r)>());
	}

}

