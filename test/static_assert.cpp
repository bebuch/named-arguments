#include <named_arguments.hpp>


namespace{


	constexpr auto test_fn = [](auto&& v)->decltype(auto){
		return static_cast< decltype(v) >(v);
	};

	void f(){
		using namespace named_arguments::literals;

// 		named_arguments::adapt(test_fn, 5);
// 		named_arguments::adapt(test_fn, "v"_arg);
// 		named_arguments::adapt(test_fn, "v"_arg = 5);
// 		named_arguments::adapt(test_fn, "v"_default_arg);

// 		auto wrap_fn = named_arguments::adapt(test_fn, "v"_no_default_arg);

// 		wrap_fn(5);
// 		wrap_fn("v"_arg);
	}


}
