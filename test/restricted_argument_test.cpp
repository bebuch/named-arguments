#include <named_arguments.hpp>


namespace{


	using namespace named_arguments::literals;
	namespace hana = boost::hana;


	struct move_only_arg{
		move_only_arg(move_only_arg&&) = default;
	};

	struct not_movable_arg{
		not_movable_arg(not_movable_arg&&) = delete;
	};


	constexpr auto test_fn = [](auto&& v){
		return hana::type_c< decltype(v) >;
	};


	constexpr auto no_default_wrap_fn =
		named_arguments::adapt(test_fn, "v"_no_default_arg);

	auto move_only_default_wrap_fn =
		named_arguments::adapt(test_fn,
			"v"_default_arg = []{ return move_only_arg{}; });

	auto not_movable_default_wrap_fn =
		named_arguments::adapt(test_fn,
			"v"_default_arg = []{ return not_movable_arg{}; });


	template < typename T1, typename T2 >
	void expect_equal(hana::basic_type< T1 > t1, hana::basic_type< T2 > t2){
		auto valid = t1 == t2;
		static_assert(valid);
	}

	void move_only_arg_test(){
		auto ref_type = test_fn(move_only_arg{});
		auto test_type = no_default_wrap_fn("v"_arg = move_only_arg{});
		expect_equal(ref_type, test_type);
	}

	void not_movable_arg_test(){
		auto ref_type = test_fn(not_movable_arg{});
		auto test_type = no_default_wrap_fn("v"_arg = not_movable_arg{});
		expect_equal(ref_type, test_type);
	}

	void move_only_default_arg_test(){
		auto ref_type = test_fn(move_only_arg{});
		auto test_type = move_only_default_wrap_fn();
		expect_equal(ref_type, test_type);
	}

	void not_movable_default_arg_test(){
		auto ref_type = test_fn(not_movable_arg{});
		auto test_type = not_movable_default_wrap_fn();
		expect_equal(ref_type, test_type);
	}


}
