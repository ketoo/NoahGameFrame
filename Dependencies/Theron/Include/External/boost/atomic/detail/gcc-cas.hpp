//  Copyright (c) 2011 Helge Bahmann
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// Use the gnu builtin __sync_val_compare_and_swap to build
// atomic operations for 32 bit and smaller.

#ifndef BOOST_DETAIL_ATOMIC_GENERIC_CAS_HPP
#define BOOST_DETAIL_ATOMIC_GENERIC_CAS_HPP

#define BOOST_ATOMIC_CHAR_LOCK_FREE 2
#define BOOST_ATOMIC_SHORT_LOCK_FREE 2
#define BOOST_ATOMIC_INT_LOCK_FREE 2
#define BOOST_ATOMIC_LONG_LOCK_FREE (sizeof(long) <= 4 ? 2 : 0)
#define BOOST_ATOMIC_LLONG_LOCK_FREE (sizeof(long long) <= 4 ? 2 : 0)
#define BOOST_ATOMIC_ADDRESS_LOCK_FREE (sizeof(void *) <= 4 ? 2 : 0)
#define BOOST_ATOMIC_BOOL_LOCK_FREE 2

namespace boost {

#define BOOST_ATOMIC_THREAD_FENCE 2
inline void
atomic_thread_fence(memory_order order)
{
	switch(order) {
		case memory_order_relaxed:
			break;
		case memory_order_release:
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_acq_rel:
		case memory_order_seq_cst:
			__sync_synchronize();
			break;
	}
}

namespace detail {
namespace atomic {

static inline void
platform_fence_before(memory_order)
{
	/* empty, as compare_and_swap is synchronizing already */
}

static inline void
platform_fence_after(memory_order)
{
	/* empty, as compare_and_swap is synchronizing already */
}

static inline void
platform_fence_before_store(memory_order order)
{
	switch(order) {
		case memory_order_relaxed:
		case memory_order_acquire:
		case memory_order_consume:
			break;
		case memory_order_release:
		case memory_order_acq_rel:
		case memory_order_seq_cst:
			__sync_synchronize();
			break;
	}
}

static inline void
platform_fence_after_store(memory_order order)
{
	if (order == memory_order_seq_cst)
		__sync_synchronize();
}

static inline void
platform_fence_after_load(memory_order order)
{
	switch(order) {
		case memory_order_relaxed:
		case memory_order_release:
			break;
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_acq_rel:
		case memory_order_seq_cst:
			__sync_synchronize();
			break;
	}
}

template<typename T>
bool
platform_cmpxchg32_strong(T & expected, T desired, volatile T * ptr)
{
	T found = __sync_val_compare_and_swap(ptr, expected, desired);
	bool success = (found == expected);
	expected = found;
	return success;
}

}
}
}

#include <boost/atomic/detail/cas32strong.hpp>

#endif
