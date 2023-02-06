// https://en.cppreference.com/w/cpp/language/cast_operator
// https://en.cppreference.com/w/cpp/types/integral_constant
// https://en.cppreference.com/w/cpp/types/is_integral
// https://en.cppreference.com/w/cpp/types/enable_if
// https://eli.thegreenplace.net/2014/sfinae-and-enable_if/
// https://stackoverflow.com/questions/72604511/understanding-enable-if-implementation-in-c98

#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

namespace ft {
	template< class T, T v >
	struct integral_constant {
		typedef T						value_type;
		typedef integral_constant<T, v>	type;

		static const T	value = v;

		operator value_type() const { return value; }
	};

	typedef ft::integral_constant<bool, true>	true_type;
	typedef ft::integral_constant<bool, false>	false_type;


	template< class >
    struct is_integral : public ft::false_type { };

	template< class T >
	struct is_integral<const T> : public ft::is_integral<T> { };

	template< class T >
	struct is_integral<const volatile T> : public ft::is_integral<T> { };

	template< class T >
	struct is_integral<volatile T> : public ft::is_integral<T> { };


	template<>
	struct is_integral<bool> : public ft::true_type { };

	template<>
	struct is_integral<char> : public ft::true_type { };

	template<>
	struct is_integral<signed char> : public ft::true_type { };

	template<>
	struct is_integral<unsigned char> : public ft::true_type { };

	template<>
	struct is_integral<wchar_t> : public ft::true_type { };

	template<>
	struct is_integral<short> : public ft::true_type { };

	template<>
	struct is_integral<unsigned short> : public ft::true_type { };

	template<>
	struct is_integral<int> : public ft::true_type { };

	template<>
	struct is_integral<unsigned int> : public ft::true_type { };

	template<>
	struct is_integral<long> : public ft::true_type { };

	template<>
	struct is_integral<unsigned long> : public ft::true_type { };

	template<>
	struct is_integral<long long> : public ft::true_type { };

	template<>
	struct is_integral<unsigned long long> : public ft::true_type { };


	// Primary template.
	template< bool, class T = void >
	struct enable_if { };

	// Partial specialization for true.
	// Define a member typedef `type` only if a boolean constant is true.
	template< class T >
	struct enable_if<true, T> { typedef T type; };
}

#endif
