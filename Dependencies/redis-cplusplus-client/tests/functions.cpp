
#include "functions.h"

void test(const string & name)
{
  #ifndef NDEBUG
  cerr << "------------------------------" << endl
  << "starting test: "                << name << endl;
  #else
  (void) name;
  #endif
}
