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
			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;

			/// \brief Set parameter value
			template< typename T >
			constexpr auto operator=(T&& value){
				return hana::make_pair(name, static_cast< T&& >(value));
			}
		};

		/// \brief An argument name with default value
		template < typename T, char ... Chars >
		class named_default_param{
		public:
			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;


			static_assert(!std::is_reference_v< T >,
				"default value must not be a reference");


			/// \brief Move constructor
			named_default_param(T&& default_value)
				: default_value_(std::move(default_value)) {}

			/// \brief Copy constructor
			named_default_param(T const& default_value)
				: default_value_(default_value) {}


			/// \brief Getter function
			T default_value()const{
				return default_value_;
			}


		private:
			/// \brief The default value
			T default_value_;
		};


		/// \brief An argument name with default value
		template < char ... Chars >
		struct named_default_param_creator{
			/// \brief Argument name as hana::string
			static constexpr auto name = hana::string_c< Chars ... >;

			/// \brief Set a default value
			template< typename T >
			constexpr auto operator=(T&& value){
				return named_default_param< std::remove_cv_t<
					std::remove_reference_t< T > >, Chars ... >(
						static_cast< T&& >(value));
			}
		};

		/// \brief Get argument names as hana::string from specification
		constexpr auto extract = [](auto const& arg_spec, auto&& args){
			return hana::transform(arg_spec, [&args](auto const& key)->decltype(auto){
				if constexpr(auto v = hana::contains(
					static_cast< decltype(args)&& >(args), key.name);
					v
				){
					return std::move(args[key.name]);
				}else{
					return key.default_value();
				}
			});
		};


	}


	namespace literals{


		// TODO: replace GNU extention by C++20 version when it is implemented in
		//       GCC and clang
		/// \brief The named parameter in a call
		template < typename CharT, CharT ... Chars >
		constexpr auto operator""_arg() ->
		detail::named_param< Chars ... >{
			return {};
		}

		// TODO: replace GNU extention by C++20 version when it is implemented in
		//       GCC and clang
		/// \brief The named parameter with default value
		template < typename CharT, CharT ... Chars >
		constexpr auto operator""_defaultarg()
		-> detail::named_default_param_creator< Chars ... >{
			return {};
		}


	}


	/// \brief Wrap a function call into an named callable object
	constexpr auto adapt = [](auto& f, auto ... input_arg_spec){
		auto arg_spec = hana::make_tuple(input_arg_spec ...);
		return [&f, &arg_spec](auto&& ... input_args){
			auto args = hana::make_map(
				static_cast< decltype(input_args)&& >(input_args) ...);

			auto arg_pack = detail::extract(arg_spec, std::move(args));

			return hana::unpack(std::move(arg_pack), f);
		};
	};


}


#endif
