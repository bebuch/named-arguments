#include <named_arguments.hpp>

#include "gtest/gtest.h"

#include <string>


namespace{


	using namespace named_arguments::literals;


	auto test_fn = [](int a, float&& b, std::string const& c)->int{
			EXPECT_EQ(a, 10);
			EXPECT_EQ(b, 0.5f);
			EXPECT_EQ(c, "string");

			return 5;
		};


}


TEST(simple_wrap, with_3_args_default_fff){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_no_default_arg,
		"b"_no_default_arg,
		"c"_no_default_arg);

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);
}

TEST(simple_wrap, with_3_args_default_tff){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_default_arg = []{ return 10; },
		"b"_no_default_arg,
		"c"_no_default_arg);


	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f), 5);
}

TEST(simple_wrap, with_3_args_default_ftf){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_no_default_arg,
		"b"_default_arg = []{ return 0.5f; },
		"c"_no_default_arg);

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10), 5);
}

TEST(simple_wrap, with_3_args_default_fft){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_no_default_arg,
		"b"_no_default_arg,
		"c"_default_arg = []{ return "string"; });

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10), 5);
}

TEST(simple_wrap, with_3_args_default_ttf){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_default_arg = []{ return 10; },
		"b"_default_arg = []{ return 0.5f; },
		"c"_no_default_arg);

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("c"_arg = "string"), 5);
}

TEST(simple_wrap, with_3_args_default_ftt){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_no_default_arg,
		"b"_default_arg = []{ return 0.5f; },
		"c"_default_arg = []{ return "string"; });

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("a"_arg = 10), 5);
}

TEST(simple_wrap, with_3_args_default_tft){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_default_arg = []{ return 10; },
		"b"_no_default_arg,
		"c"_default_arg = []{ return "string"; });

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("b"_arg = 0.5f), 5);
}

TEST(simple_wrap, with_3_args_default_ttt){
	auto named_fn = named_arguments::adapt(test_fn,
		"a"_default_arg = []{ return 10; },
		"b"_default_arg = []{ return 0.5f; },
		"c"_default_arg = []{ return "string"; });

	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("a"_arg = 10, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "a"_arg = 10), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "c"_arg = "string"), 5);
	EXPECT_EQ(named_fn("c"_arg = "string", "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("a"_arg = 10, "b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f, "a"_arg = 10), 5);

	EXPECT_EQ(named_fn("a"_arg = 10), 5);
	EXPECT_EQ(named_fn("b"_arg = 0.5f), 5);
	EXPECT_EQ(named_fn("c"_arg = "string"), 5);
}
