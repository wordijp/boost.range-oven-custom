#pragma once

// p-stade oven detail shared_range_iterator.hpp
// http://p-stade.sourceforge.net/

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/pointee.hpp>
#include <boost/range/iterator.hpp>

namespace rng { namespace detail {


template< class Ptr >
struct shared_range_iterator;


template< class Ptr >
struct shared_range_iterator_super
{
    typedef typename
        boost::pointee<Ptr>::type
    rng_t;

    typedef
        boost::iterator_adaptor<
            shared_range_iterator<Ptr>,
            typename boost::range_iterator<rng_t>::type
        >
    type;
};


template< class Ptr >
struct shared_range_iterator :
    shared_range_iterator_super<Ptr>::type
{
private:
    typedef typename shared_range_iterator_super<Ptr>::type super_t;
    typedef typename super_t::base_type iter_t;

public:
    shared_range_iterator()
    { }

    shared_range_iterator(iter_t it, Ptr prng) :
        super_t(it), m_prng(prng)
    { }

template< class > friend struct shared_range_iterator;
    template< class P >
    shared_range_iterator(shared_range_iterator<P> const& other,
        typename boost::enable_if_convertible<typename shared_range_iterator<P>::iter_t, iter_t>::type * = 0,
        typename boost::enable_if_convertible<
            // Use raw pointer type; 'boost::shared_ptr' convertibility is over-optimistic.
            typename boost::pointee<P>::type *, typename boost::pointee<Ptr>::type *
        >::type * = 0
    ) :
        super_t(other.base()), m_prng(other.m_prng)
    { }

    typename boost::pointee<Ptr>::type& base_range() const
    {
        return *m_prng;
    }

private:
    Ptr m_prng;
};

} } // namespace rng::detail

