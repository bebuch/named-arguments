#ifndef _named_arguments__named_arguments__hpp_INCLUDED_
#define _named_arguments__named_arguments__hpp_INCLUDED_

#include <boost/hana.hpp>


namespace named_arguments{


	namespace hana = boost::hana;
	using namespace hana::literals;


	namespace detail{


		struct call_tag;
		struct call_initializer_tag;
		struct config_tag;
		struct config_initializer_tag;


		/// \brief An argument name with value
		template < typename Fn, char ... Chars >
		class named_param{
		public:
			/// \brief Used for hana concepts check
			using hana_tag = call_tag;

			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;


			/// \brief Constructor
			named_param(Fn const& fn)
				: fn_(fn) {}


			/// \brief Getter function
			Fn const& argument_value_fn()const{
				return fn_;
			}


		private:
			/// \brief The argument provider function
			Fn fn_;
		};

		/// \brief An argument name
		template < char ... Chars >
		struct named_param_initializer{
			/// \brief Used for hana concepts check
			using hana_tag = call_initializer_tag;

			/// \brief Set parameter value
			template< typename T >
			constexpr auto operator=(T&& value){
				constexpr auto name = hana::string_c< Chars ... >;
				auto get_reference_fn = [&value]()->T&&{
						return static_cast< T&& >(value);
					};
				return named_param< decltype(get_reference_fn), Chars ... >(
					get_reference_fn);
			}
		};


		/// \brief An argument name
		template < char ... Chars >
		struct named_param_no_default{
			/// \brief Used for hana concepts check
			using hana_tag = config_tag;

			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;
		};


		/// \brief An argument name with default value
		template < typename Fn, char ... Chars >
		class named_param_default{
		public:
			/// \brief Used for hana concepts check
			using hana_tag = config_tag;

			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;

			/// \brief Type of the default parameter
			using type = std::invoke_result_t< Fn const >;


			/// \brief Move in constructor
			named_param_default(Fn&& fn)
				: fn_(std::move(fn)) {}

			/// \brief Copy in constructor
			named_param_default(Fn const& fn)
				: fn_(fn) {}


			/// \brief Getter function
			Fn const& default_value_fn()const{
				return fn_;
			}


		private:
			/// \brief The default value provider function
			Fn fn_;
		};

		/// \brief An argument name with default value
		template < char ... Chars >
		struct named_param_default_initializer{
			/// \brief Used for hana concepts check
			using hana_tag = config_initializer_tag;

			/// \brief Set a default value
			template< typename Fn >
			constexpr auto operator=(Fn&& fn){
				return named_param_default< Fn, Chars ... >(
					static_cast< Fn&& >(fn));
			}
		};


		/// \brief Get argument names as hana::string from specification
		auto extract = [](auto arg_spec, auto args){
			return hana::transform(arg_spec, [&args](auto key){
				if constexpr(auto v = hana::contains(args, key.name); v){
					return args[key.name];
				}else{
					return key.default_value_fn();
				}
			});
		};


	}


	namespace literals{


		// TODO: replace GNU extention by C++20 version when it is implemented
		//       in GCC and clang
		/// \brief The named parameter in a call
		template < typename CharT, CharT ... Chars >
		constexpr auto operator""_arg() ->
		detail::named_param_initializer< Chars ... >{
			return {};
		}

		// TODO: replace GNU extention by C++20 version when it is implemented
		//       in GCC and clang
		/// \brief The named parameter with default value
		template < typename CharT, CharT ... Chars >
		constexpr auto operator""_no_default_arg()
		-> detail::named_param_no_default< Chars ... >{
			return {};
		}

		// TODO: replace GNU extention by C++20 version when it is implemented
		//       in GCC and clang
		/// \brief The named parameter with default value
		template < typename CharT, CharT ... Chars >
		constexpr auto operator""_default_arg()
		-> detail::named_param_default_initializer< Chars ... >{
			return {};
		}


	}


	/// \brief Wrap a function call into an named callable object
	// TODO: owning f
	auto adapt = [](auto& f, auto ... input_arg_spec)->decltype(auto){
		auto arg_spec = hana::make_tuple(input_arg_spec ...);

		auto forgot_assign_default_value = hana::any_of(arg_spec,
			hana::is_a< detail::config_initializer_tag >);
		static_assert(!forgot_assign_default_value,
			"you forgot to assign the default value in named_arguments::adapt "
			"call, use »adapt(f, \"param_name\"_default_arg = "
			"[]{ return value; })« to assign a value to the named parameter "
			"param_name");

		auto wrongly_used_call_literals = hana::any_of(arg_spec,
			[](auto arg){
				return hana::or_(
						hana::is_a< detail::call_tag >(arg),
						hana::is_a< detail::call_initializer_tag >(arg)
					);
			});
		static_assert(!wrongly_used_call_literals,
			"named_arguments::adapt wrongly called with a call literal "
			"argument \"param_name\"_arg, use »\"param_name\"_no_default_args« "
			"or »\"param_name\"_default_arg« instead: use \n»adapt(f, "
			"\"param_name\"_no_default_arg)«\n or \n»adapt(f, "
			"\"param_name\"_default_arg = []{ return value; })«\n instead of \n"
			"»adapt(f, \"param_name\"_arg)«");

		auto all_params_named = hana::all_of(
			arg_spec, hana::is_a< detail::config_tag >);
		static_assert(all_params_named,
			"named_arguments::adapt called with arguments that are neither "
			"»\"param_name\"_no_default_args« nor »\"param_name\"_default_arg«"
			", use »adapt(f, \"param_name_1\"_no_default_arg, "
			"\"param_name_2\"_default_arg = []{ return value; } ...)«");

		return [&f, arg_spec](auto ... input_args)->decltype(auto){
			auto input_tuple = hana::make_tuple(input_args ...);

			auto forgot_assign_value = hana::any_of(input_tuple,
				hana::is_a< detail::call_initializer_tag >);
			static_assert(!forgot_assign_value,
				"you forgot to assign a value in named_arguments adapted "
				"function call, use »f(\"param_name\"_arg = value ...)« to "
				"assign a value to the named parameter param_name");

			auto all_params_named = hana::all_of(
				input_tuple, hana::is_a< detail::call_tag >);
			static_assert(all_params_named,
				"named_arguments adapted function called with not named "
				"arguments, use »f(\"param_name\"_arg = value ...)« to map "
				"your values to the parameter names specified by "
				"named_arguments::adapt");


			auto input_map = hana::to_map(hana::transform(input_tuple,
				[](auto input_arg){
					return hana::make_pair(input_arg.name,
						input_arg.argument_value_fn());
				}));
			auto ordered_input_tuple = detail::extract(arg_spec, input_map);
			return hana::unpack(ordered_input_tuple,
				[&f](auto ... param)->decltype(auto){
					return f(static_cast<
						std::invoke_result_t< decltype(param) > >(
							param()) ...);
				});
		};
	};


}


#endif
