#include <stdlib.h>

#include <string>
#include <iostream>

#include <boost/date_time.hpp>

using namespace std;

#define ASSERT_EQUAL(x,y) assert_equal(x, y, __LINE__)
#define ASSERT_NOT_EQUAL(x,y) assert_not_equal(x, y, __LINE__)
#define ASSERT_GT(x,y) assert_gt(x, y, __LINE__)

template <typename T>
void assert_equal(const T & actual, const T & expected, int lineno)
{
  #ifndef NDEBUG
  //cerr << "assert_equal('" << expected << "', '" << actual << "')" << endl;
  #endif
  
  if (expected != actual)
  {
    cerr << "expected '" << expected << "' got '" << actual << "'" << endl
    << "failing test called from line " << lineno << endl;
    
    exit(1);
  }
  
  #ifndef NDEBUG
  //cerr << "... OK" << endl;
  #endif
}

template <typename T>
void assert_not_equal(const T & a, const T & b, int lineno)
{
  if (a == b)
  {
    cerr << "expected inequality" << endl
    << "failing test called from line " << lineno << endl;
    
    exit(1);
  }
}

template <typename T>
void assert_gt(const T & a, const T & b, int lineno)
{
  #ifndef NDEBUG
  //cerr << "assert_gt('" << a << "', '" << b << "')" << endl;
  #endif
  
  if (a <= b)
  {
    cerr << "expected '" << a << "' > '" << b << "'" << endl
    << "failing test called from line " << lineno << endl;
    
    exit(1);
  }
  
  #ifndef NDEBUG
  //cerr << "... OK" << endl;
  #endif
}

void test(const string & name);

inline boost::posix_time::ptime now()
{
  return boost::posix_time::microsec_clock::local_time();
}

class block_duration
{
public:
  explicit block_duration(const std::string & job_name, size_t count = 0)
  : start_(now()), job_name_(job_name), count_(count)
  {
    cerr << "------------------------------" << endl;
  }
  
  ~block_duration()
  {
    boost::posix_time::ptime end = now();
    cerr << job_name_ << " took " << (end-start_) << " (" << count_ << " items)" << endl;
    if(count_ > 0)
    {
      cerr << (end-start_).total_milliseconds()/(double)count_ << "ms per item" << endl;
      cerr << (double)count_/(end-start_).total_milliseconds()*1000 << " items per second" << endl;
    }
    cerr << "------------------------------" << endl;
  }
  
private:
  boost::posix_time::ptime start_;
  std::string job_name_;
  size_t count_;
};
