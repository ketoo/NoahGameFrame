#if 0
// not yet working correctly!

#include "../redisclient.h"

#include "functions.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/array.hpp>

#define THREAD_COUNT 5
#define INCREM_COUNT 1000

inline boost::xtime delay(int secs, int msecs=0, int nsecs=0)
{
  const int MILLISECONDS_PER_SECOND = 1000;
  const int NANOSECONDS_PER_SECOND = 1000000000;
  const int NANOSECONDS_PER_MILLISECOND = 1000000;
  
  boost::xtime xt;
  if (boost::TIME_UTC != boost::xtime_get (&xt, boost::TIME_UTC))
    ASSERT_EQUAL(false, true); // boost::xtime_get != boost::TIME_UTC
  
  nsecs += xt.nsec;
  msecs += nsecs / NANOSECONDS_PER_MILLISECOND;
  secs += msecs / MILLISECONDS_PER_SECOND;
  nsecs += (msecs % MILLISECONDS_PER_SECOND) *
  NANOSECONDS_PER_MILLISECOND;
  xt.nsec = nsecs % NANOSECONDS_PER_SECOND;
  xt.sec += secs + (nsecs / NANOSECONDS_PER_SECOND);
  
  return xt;
}

template <typename M>
void test_lock(M & mutex)
{
  typedef M mutex_type;
  typedef typename M::scoped_lock lock_type;
  
  boost::condition condition;
  
  // Test the lock's constructors.
  {
    lock_type lock(mutex, boost::defer_lock);
    ASSERT_EQUAL( (bool) lock, false);
  }
  lock_type lock(mutex);
  ASSERT_EQUAL( (bool) lock, true);
  
  // Construct and initialize an xtime for a fast time out.
  boost::xtime xt = delay(0, 100);
  
  // Test the lock and the mutex with condition variables.
  // No one is going to notify this condition variable.  We expect to
  // time out.
  ASSERT_EQUAL(condition.timed_wait(lock, xt), false);
  ASSERT_EQUAL( (bool) lock, true );
  
  // Test the lock and unlock methods.
  lock.unlock();
  ASSERT_EQUAL( (bool) lock, false );
  lock.lock();
  ASSERT_EQUAL( (bool) lock, true );
}

#define INT_VARIABLE "mutex_protected_int"

struct unsave_increment_in_mutex
{
  unsave_increment_in_mutex(const redis::client & c, const std::string & mutex_name, int count)
  : shr_c( c.clone() ),
    count(count),
    mutex(mutex_name, *shr_c)
  {
    ASSERT_EQUAL(shr_c->connections().size(), c.connections().size());
    if( shr_c->connections() != c.connections() )
      ASSERT_EQUAL(false, true);
  }
  
  void operator()()
  {
    redis::distributed_int dist_int(INT_VARIABLE, 0, *shr_c);
    
    for(int i=0; i < count; i++)
    {
      redis::distributed_mutex::scoped_lock lock(mutex);
      redis::client::int_type local_int = dist_int.to_int() + 1;
      dist_int = local_int;

      if( local_int % (THREAD_COUNT*10-1) == 0 )
      {
        //cout << boost::this_thread::get_id() << ": " << local_int << endl;
      }
    }

    ASSERT_EQUAL(dist_int.to_int() >= count, true);
  }

  boost::shared_ptr<redis::client> shr_c;
  int count;
  redis::distributed_mutex mutex;
};

void test_distributed_mutexes(redis::client & c)
{
  redis::distributed_int dist_int(INT_VARIABLE, c);
  dist_int = 0;
  ASSERT_EQUAL(dist_int.to_int(), 0);
  
  ostringstream os;
  os << "incremented in " << THREAD_COUNT << " threads with distributed mutexes";
  
  {
    block_duration dur(os.str(), THREAD_COUNT*INCREM_COUNT);
    
    boost::array< boost::optional<unsave_increment_in_mutex>, THREAD_COUNT > incrementors;
    boost::array< boost::thread, THREAD_COUNT > threads;
    
    for(int i=0; i < THREAD_COUNT; i++)
      incrementors[i] = unsave_increment_in_mutex(c, "mutex1", INCREM_COUNT);
    
    for(int i=0; i < THREAD_COUNT; i++)
      threads[i] = boost::thread( *incrementors[i] );
    
    for(int i=0; i < THREAD_COUNT; i++)
      threads[i].join();
  }
  
  ASSERT_EQUAL( boost::lexical_cast<int>( c.get(INT_VARIABLE) ), THREAD_COUNT*INCREM_COUNT);
  ASSERT_EQUAL(dist_int.to_int(), THREAD_COUNT*INCREM_COUNT);
}

#endif // 0
