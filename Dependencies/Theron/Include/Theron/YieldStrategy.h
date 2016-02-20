// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_YIELDSTRATEGY_H
#define THERON_YIELDSTRATEGY_H


/**
\file YieldStrategy.h
Defines the YieldStrategy enumerated type.
*/


namespace Theron
{


/**
\brief Enumerates the available worker thread yield strategies.

Each \ref Theron::Framework contains a pool of worker threads that are used to execute
the actors hosted in the framework. The worker threads service a queue, processing actors that
have received messages and executing their registered message handlers.

When constructing a Framework, a \ref Theron::Framework::Parameters object may be provided with
parameters that control the structure and behavior of the the framework's internal threadpool.
This enum defines the available values of the \ref Theron::Framework::Parameters::mYieldStrategy "mYieldStrategy"
member of the Parameters structure.

The mYieldStrategy member defines the strategy that the worker threads use to avoid \em busy \em waiting
on the work queue. The available strategies have different performance characteristics, and are best
suited to different kinds of applications.

As of Theron 6, the default strategy is \ref YIELD_STRATEGY_CONDITION. This strategy causes
threads to wait on condition variables when no work is available. Waiting on conditions is
a traditional approach to scheduling and has the benefit of not consuming CPU cycles when the
threads are not busy. Instead, the CPU usage reflects the actual amount of work being done,
and unused cycles are available for other applications (or other threads in the system) to
use for unrelated work. This feature makes YIELD_STRATEGY_CONDITION a good fit for most
applications and the obvious default strategy.

One potential downside of waiting on condition variables is that it involves system calls
that can themselves consume thousands of cycles, introducing latency. For that reason,
this approach has fallen out of favor in specialized low-latency applications where responding
quickly is vital, even at the expense of consuming unused CPU cycles in the process.

This alternative is embodied by YIELD_STRATEGY_SPIN, which causes waiting threads to
'busy-wait' in tight loops, waiting for work queues to become non-empty. The advantage of this approach
is that threads are quick to pick up work when it becomes available.
The spin loop contains no heavyweight system calls so suffers from little latency.
The obvious disadvantage is that the spinning threads consume all available CPU cycles, wasting
power and potentially starving other threads (including those in other applications). For that
reason YIELD_STRATEGY_SPIN is best used in combination with the use of processor affinity masks
(see \ref Framework::Parameters) to limit the execution of the threads to specific cores.
Those cores are then effectively dedicated to that purpose.

Finally, YIELD_STRATEGY_HYBRID is a compromise which tries to avoid the rampant consumption
of spare CPU cycles caused by busy-waiting, while still avoiding condition variables, by
yielding to other threads with a timeout after some period of spinning. The yield calls re-introduce
some latency, with the result that this approach lacks the extreme low-latency advantages of
YIELD_STRATEGY_SPIN. It also still suffers from the negative that waiting threads will consume
all of the available cycles on the cores on which they are run. For that reason it is best used
in combination with processor affinity masks, like YIELD_STRATEGY_SPIN. It's advantage over
YIELD_STRATEGY_SPIN is that any other threads running on the same cores are less likely to
be starved.

When choosing a yield strategy it pays to consider how important low-latency responses are to your
application. In most applications latencies of a few milliseconds are not significant, and the
default strategy is a reasonable choice.
*/
enum YieldStrategy
{
    YIELD_STRATEGY_CONDITION = 0,       ///< Threads wait on condition variables when no work is available.
    YIELD_STRATEGY_HYBRID,              ///< Threads spin for a while, then yield to other threads, when no work is available.
    YIELD_STRATEGY_SPIN,                ///< Threads busy-wait, without yielding, when no work is available.

    // Legacy section
    YIELD_STRATEGY_BLOCKING = 0,        ///< Deprecated - use YIELD_STRATEGY_CONDITION.
    YIELD_STRATEGY_POLITE = 0,          ///< Deprecated - use YIELD_STRATEGY_CONDITION.
    YIELD_STRATEGY_STRONG = 1,          ///< Deprecated - use YIELD_STRATEGY_HYBRID.
    YIELD_STRATEGY_AGGRESSIVE = 2       ///< Deprecated - use YIELD_STRATEGY_SPIN.
};


} // namespace Theron


#endif // THERON_YIELDSTRATEGY_H
