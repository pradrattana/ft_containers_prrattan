//https://en.cppreference.com/w/cpp/iterator/reverse_iterator

#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterator_traits.hpp"

namespace ft {
	template< class Iter >
	class reverse_iterator {
	public:
		typedef Iter													iterator_type;
		typedef typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
		typedef typename ft::iterator_traits<Iter>::value_type			value_type;
		typedef typename ft::iterator_traits<Iter>::difference_type		difference_type;
		typedef typename ft::iterator_traits<Iter>::pointer				pointer;
		typedef typename ft::iterator_traits<Iter>::reference			reference;

		reverse_iterator() {}

		explicit
		reverse_iterator( iterator_type x ) : current(x) {}

		template< class U >
		reverse_iterator( const reverse_iterator<U>& other ) : current(other.base()) {}

		template< class U >
		reverse_iterator&	operator=( const reverse_iterator<U>& other ) {
			current = other.base();
			return *this;
		}

		iterator_type	base() const { return current; }

		reference	operator*() const { return *(base() - 1); }
		pointer		operator->() const { return &(operator*()); }

		reference	operator[]( difference_type n ) const { return base()[-n - 1]; }

		reverse_iterator&	operator++() { --current; return *this; }
		reverse_iterator&	operator--() { ++current; return *this; }
		reverse_iterator	operator++( int ) { reverse_iterator tmp = *this; ++(*this); return tmp; }
		reverse_iterator	operator--( int ) { reverse_iterator tmp = *this; --(*this); return tmp; }
	
		reverse_iterator	operator+( difference_type n ) const { return reverse_iterator(base() - n); }
		reverse_iterator	operator-( difference_type n ) const { return reverse_iterator(base() + n); }
		reverse_iterator&	operator+=( difference_type n ) { current -= n; return *this; }
		reverse_iterator&	operator-=( difference_type n ) { current += n; return *this; }

	protected:
		iterator_type	current;
	};

	template< class Iter1, class Iter2 >
	bool operator==( const ft::reverse_iterator<Iter1>& lhs, const ft::reverse_iterator<Iter2>& rhs ) {
		return lhs.base() == rhs.base();
	}

	template< class Iter1, class Iter2 >
	bool operator!=( const ft::reverse_iterator<Iter1>& lhs, const ft::reverse_iterator<Iter2>& rhs ) {
		return !(lhs == rhs);
	}

	template< class Iter1, class Iter2 >
	bool operator<( const ft::reverse_iterator<Iter1>& lhs, const ft::reverse_iterator<Iter2>& rhs ) {
		return lhs.base() > rhs.base();
	}

	template< class Iter1, class Iter2 >
	bool operator<=( const ft::reverse_iterator<Iter1>& lhs, const ft::reverse_iterator<Iter2>& rhs ) {
		return !(rhs < lhs);
	}

	template< class Iter1, class Iter2 >
	bool operator>( const ft::reverse_iterator<Iter1>& lhs, const ft::reverse_iterator<Iter2>& rhs ) {
		return rhs < lhs;
	}

	template< class Iter1, class Iter2 >
	bool operator>=( const ft::reverse_iterator<Iter1>& lhs, const ft::reverse_iterator<Iter2>& rhs ) {
		return !(lhs < rhs);
	}

	template< class Iter >
	reverse_iterator<Iter> operator+( typename reverse_iterator<Iter>::difference_type n,
			const reverse_iterator<Iter>& it ) {
		return reverse_iterator<Iter>(it.base() - n);
	}
			   
	// template< class Iter >
	// typename reverse_iterator<Iter>::difference_type operator-( const reverse_iterator<Iter>& lhs,
	// 		const reverse_iterator<Iter>& rhs ) {
	// 	return rhs.base() - lhs.base();
	// }
			   
	template< class Iter1, class Iter2 >
	typename reverse_iterator<Iter1>::difference_type operator-( const reverse_iterator<Iter1>& lhs,
			const reverse_iterator<Iter2>& rhs ) {
		return rhs.base() - lhs.base();
	}
}

#endif
