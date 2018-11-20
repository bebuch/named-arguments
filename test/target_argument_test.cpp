#include <named_arguments.hpp>

#include "gtest/gtest.h"


namespace{


	using namespace named_arguments::literals;
	namespace hana = boost::hana;


	constexpr auto value_fn = [](int v){
		EXPECT_EQ(v, 10);
	};

	constexpr auto lvalue_ref_fn = [](int& v){
		EXPECT_EQ(v, 10);
		v = 5;
	};

	constexpr auto lvalue_const_ref_fn = [](int const& v){
		EXPECT_EQ(v, 10);
	};

	constexpr auto rvalue_ref_fn = [](int&& v){
		EXPECT_EQ(v, 10);
		v = 5;
	};

	constexpr auto rvalue_const_ref_fn = [](int const&& v){
		EXPECT_EQ(v, 10);
	};


	constexpr auto wrap_value_fn =
		named_arguments::adapt(value_fn, "v"_no_default_arg);
	constexpr auto wrap_lvalue_ref_fn =
		named_arguments::adapt(lvalue_ref_fn, "v"_no_default_arg);
	constexpr auto wrap_lvalue_const_ref_fn =
		named_arguments::adapt(lvalue_const_ref_fn, "v"_no_default_arg);
	constexpr auto wrap_rvalue_ref_fn =
		named_arguments::adapt(rvalue_ref_fn, "v"_no_default_arg);
	constexpr auto wrap_rvalue_const_ref_fn =
		named_arguments::adapt(rvalue_const_ref_fn, "v"_no_default_arg);


}


TEST(target_argument, value){
	int v = 10;
	value_fn(v);
	wrap_value_fn("v"_arg = v);
}

TEST(target_argument, lvalue_ref){
	int v = 10;
	lvalue_ref_fn(v);
	EXPECT_EQ(v, 5);
	v = 10;
	wrap_lvalue_ref_fn("v"_arg = v);
	EXPECT_EQ(v, 5);
}

TEST(target_argument, lvalue_const_ref){
	int v = 10;
	lvalue_const_ref_fn(v);
	wrap_lvalue_const_ref_fn("v"_arg = v);
}

TEST(target_argument, rvalue_ref){
	int v = 10;
	rvalue_ref_fn(std::move(v));
	EXPECT_EQ(v, 5);
	v = 10;
	wrap_rvalue_ref_fn("v"_arg = std::move(v));
	EXPECT_EQ(v, 5);
}

TEST(target_argument, rvalue_const_ref){
	int v = 10;
	rvalue_const_ref_fn(std::move(v));
	wrap_rvalue_const_ref_fn("v"_arg = std::move(v));
}
