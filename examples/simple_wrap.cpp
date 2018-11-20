#include <named_arguments.hpp>

#include <string>
#include <iostream>


using namespace named_arguments::literals;


float foo(int a, float&& b, std::string const& c){
	return a + b * c.size();
};


auto bar = named_arguments::adapt(
	foo,
	"a"_default_arg = []{ return 10; },
	"b"_no_default_arg,
	"c"_default_arg = []{ return "string"; });


int main(){
	auto v = bar("c"_arg = "string", "b"_arg = 0.5f);
	std::cout << v << '\n';
}
