#ifndef _named_arguments__named_arguments__hpp_INCLUDED_
#define _named_arguments__named_arguments__hpp_INCLUDED_

#include <boost/hana.hpp>


namespace named_arguments{


	namespace hana = boost::hana;
	using namespace hana::literals;


	namespace detail{


		/// \brief An argument name
		template < char ... Chars >
		struct named_param{
			/// \brief Set parameter value
			template< typename T >
			constexpr auto operator=(T&& value){
				constexpr auto name = hana::string_c< Chars ... >;
				return hana::make_pair(name, [&value]()->T&&{
						return static_cast< T&& >(value);
					});
			}
		};

		/// \brief An argument name
		template < char ... Chars >
		struct named_param_no_default{
			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;
		};

		/// \brief An argument name with default value
		template < typename Fn, char ... Chars >
		class named_param_default{
		public:
			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;

			/// \brief Type of the default parameter
			using type = std::invoke_result_t< Fn const >;


			/// \brief Move constructor
			named_param_default(Fn&& fn)
				: fn_(std::move(fn)) {}

			/// \brief Copy constructor
			named_param_default(Fn const& fn)
				: fn_(fn) {}


			/// \brief Getter function
			Fn const& default_value()const{
				return fn_;
			}


		private:
			/// \brief The default value
			Fn fn_;
		};


		/// \brief An argument name with default value
		template < char ... Chars >
		struct named_param_default_initializer{
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
					return key.default_value();
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
		detail::named_param< Chars ... >{
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
	auto adapt = [](auto& f, auto ... input_arg_spec)->decltype(auto){
		auto arg_spec = hana::make_tuple(input_arg_spec ...);
		return [&f, arg_spec](auto ... input_args)->decltype(auto){
			auto args = hana::make_map(input_args ...);
			auto arg_pack = detail::extract(arg_spec, args);
			return hana::unpack(arg_pack,
				[&f](auto ... param)->decltype(auto){
					return f(static_cast<
						std::invoke_result_t< decltype(param) > >(
							param()) ...);
				});
		};
	};


}


#endif
