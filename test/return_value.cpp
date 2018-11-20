#include <named_arguments.hpp>

#include <type_traits>


namespace{


	using namespace named_arguments::literals;


	constexpr auto test_fn = [](auto&& v)->decltype(auto){
		return static_cast< decltype(v) >(v);
	};

	constexpr auto wrap_fn =
		named_arguments::adapt(test_fn, "v"_no_default_arg);


	void ref(int v){
		static_assert(std::is_same_v<
			decltype(test_fn(v)), int& >);
		static_assert(std::is_same_v<
			decltype(test_fn(v)), decltype(wrap_fn("v"_arg = v)) >);
	}

	void const_ref(int const v){
		static_assert(std::is_same_v<
			decltype(test_fn(v)), int const& >);
		static_assert(std::is_same_v<
			decltype(test_fn(v)), decltype(wrap_fn("v"_arg = v)) >);
	}

	void move_ref(int v){
		static_assert(std::is_same_v<
			decltype(test_fn(std::move(v))), int&& >);
		static_assert(std::is_same_v<
			decltype(test_fn(std::move(v))),
			decltype(wrap_fn("v"_arg = std::move(v))) >);
	}

	void const_move_ref(int const v){
		static_assert(std::is_same_v<
			decltype(test_fn(std::move(v))), int const&& >);
		static_assert(std::is_same_v<
			decltype(test_fn(std::move(v))),
			decltype(wrap_fn("v"_arg = std::move(v))) >);
	}


}
