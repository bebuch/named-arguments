#include <named_arguments.hpp>


namespace{


	using namespace named_arguments::literals;
	namespace hana = boost::hana;


	auto test_fn = [](auto&& v){
		return hana::type_c< decltype(v) >;
	};


	auto wrap_fn = named_arguments::adapt(test_fn, "v"_arg);


	template < typename T1, typename T2 >
	void expect_equal(hana::basic_type< T1 > t1, hana::basic_type< T2 > t2){
		auto valid = t1 == t2;
		static_assert(valid);
	}


	void copy(int v){
		auto ref_type = test_fn(v);
		auto test_type = wrap_fn("v"_arg = v);
		expect_equal(ref_type, test_type);
	}

	void ref(int& v){
		auto ref_type = test_fn(v);
		auto test_type = wrap_fn("v"_arg = v);
		expect_equal(ref_type, test_type);
	}

	void const_ref(int& v){
		auto ref_type = test_fn(v);
		auto test_type = wrap_fn("v"_arg = v);
		expect_equal(ref_type, test_type);
	}

	void move_ref(int&& v){
		auto ref_type = test_fn(std::move(v));
		auto test_type = wrap_fn("v"_arg = std::move(v));
		expect_equal(ref_type, test_type);
	}

	void const_move_ref(int&& v){
		auto ref_type = test_fn(std::move(v));
		auto test_type = wrap_fn("v"_arg = std::move(v));
		expect_equal(ref_type, test_type);
	}


}
