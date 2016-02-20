#ifndef BOOST_DETAIL_ATOMIC_INTERLOCKED_HPP
#define BOOST_DETAIL_ATOMIC_INTERLOCKED_HPP

//  Copyright (c) 2009 Helge Bahmann
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/detail/interlocked.hpp>

#define BOOST_ATOMIC_CHAR_LOCK_FREE 2
#define BOOST_ATOMIC_SHORT_LOCK_FREE 2
#define BOOST_ATOMIC_INT_LOCK_FREE 2
#define BOOST_ATOMIC_LONG_LOCK_FREE (sizeof(long) <= 4 ? 2 : 0)
#define BOOST_ATOMIC_LLONG_LOCK_FREE (sizeof(long long) <= 4 ? 2 : 0)
#define BOOST_ATOMIC_ADDRESS_LOCK_FREE (sizeof(void *) <= 4 ? 2 : 0)
#define BOOST_ATOMIC_BOOL_LOCK_FREE 2

namespace boost {
namespace detail {
namespace atomic {

static inline void
x86_full_fence(void)
{
	long tmp;
	BOOST_INTERLOCKED_EXCHANGE(&tmp, 0);
}

static inline void
platform_fence_before(memory_order)
{
}

static inline void
platform_fence_after(memory_order)
{
}

static inline void
platform_fence_before_store(memory_order)
{
}

static inline void
platform_fence_after_store(memory_order order)
{
	if (order == memory_order_seq_cst)
		x86_full_fence();
}

static inline void
platform_fence_after_load(memory_order order)
{
	if (order == memory_order_seq_cst) {
		x86_full_fence();
	}
}

template<typename T>
bool
platform_cmpxchg32_strong(T & expected, T desired, volatile T * ptr)
{
	T prev = expected;
	expected = (T)BOOST_INTERLOCKED_COMPARE_EXCHANGE((long *)(ptr), (long)desired, (long)expected);
	bool success = (prev==expected);
	return success;
}

#if defined(_WIN64)
template<typename T>
bool
platform_cmpxchg64_strong(T & expected, T desired, volatile T * ptr)
{
	T prev = expected;
	expected = (T) _InterlockedCompareExchange64((long long *)(ptr), (long long)desired, (long long)expected);
	bool success = (prev==expected);
	return success;
}
#endif

}
}

#define BOOST_ATOMIC_THREAD_FENCE 2
inline void
atomic_thread_fence(memory_order order)
{
	if (order == memory_order_seq_cst) {
		detail::atomic::x86_full_fence();
	}
}

}

#include <boost/atomic/detail/cas32strong.hpp>

#endif
