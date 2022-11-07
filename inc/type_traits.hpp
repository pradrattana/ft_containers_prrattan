//https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-3.3/type__traits_8h-source.html

#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

namespace ft {
	struct __true_type {};
	struct __false_type {};

	template< class T > struct _Is_integer {
		typedef __false_type	_Integral;
	};

	template<> struct _Is_integer<bool> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<char> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<signed char> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<unsigned char> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<wchar_t> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<short> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<unsigned short> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<int> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<unsigned int> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<long> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<unsigned long> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<long long> {
		typedef __true_type	_Integral;
	};

	template<> struct _Is_integer<unsigned long long> {
		typedef __true_type	_Integral;
	};

	/*template< typename T >
	struct _Is_normal_iterator {
		typedef __false_type	_Normal;
	};

	template< typename Iterator, typename Container > class __normal_iterator;

	template< typename Iterator, typename Container >
	struct _Is_normal_iterator< __normal_iterator< Iterator, Container > > {
		typedef __true_type	_Normal;
	};*/
}

#endif
