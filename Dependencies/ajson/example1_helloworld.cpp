#include <iostream>
#include <string>

#include "ajson.hpp"

using namespace std;
using namespace ajson;

struct demo
{
  string hello;
  string world;
};

AJSON(demo,hello,world)

int main(int argc, char * argv[])
{
	char * buff = "{\"hello\" : \"Hello\", \"world\" : \"world.\"}";
	demo the_demo;
	load_from_buff(the_demo,buff);
	cout << the_demo.hello << " " << the_demo.world << std::endl;
	cin.get();
	return 0;
}
