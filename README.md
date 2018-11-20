# named-arguments

A C++17 (with GNU extensions) header-only named arguments library based on an idea from Richard Powell presented on CppCon 2018 “Named Arguments from Scratch”.

https://youtu.be/Grveezn0zhU

Dependencies:
- Boost Hana

## Compiler Support

- GCC 8
- clang 7

## Usage

Example:

```cpp
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
```

Use `named_arguments::literals` to import 3 user defined literals: `"param_name"_default_arg`, `"param_name"_no_default_arg` and `"param_name"_arg`.

Then you need a callable `foo` that you wrap into a new callable `bar` by `named_arguments::adapt`. The `adapt` call maps the parameters from its original call order to some parameter names. You can use `"param_name"_no_default_arg` if you just want to specify a name for the parameter or `"param_name"_default_arg` if you also want to set a default value. Default values must be specified by another callable without parameters! Most likly a lambda as in the example. This way the default value dosn't need to be copy- or movable. You must map all parameters, only a subset is invalid.

Then you can call `bar` by assign the values to its names. The names in a call must be declared with `"param_name"_arg`.

All parameters and the return value are perfect forwareded.

```
float v;
bar("b"_arg = std::move(v));
```

## Build and execute the Tests

To build the unit tests you need CMake 3.12.4.

Build and exec tests with:

```bash
cd test
mkdir build
cd build
cmake ..
make
./simple_wrap
```
