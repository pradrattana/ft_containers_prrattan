#ifndef VECTOR_ITERATOR_HPP
# define VECTOR_ITERATOR_HPP

# include "iterator_traits.hpp"

namespace ft {
	template< class Iter >
	class vector_iterator {
	public:
		typedef Iter													iterator_type;
		typedef typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
		typedef typename ft::iterator_traits<Iter>::value_type			value_type;
		typedef typename ft::iterator_traits<Iter>::difference_type		difference_type;
		typedef typename ft::iterator_traits<Iter>::pointer				pointer;
		typedef typename ft::iterator_traits<Iter>::reference			reference;

		vector_iterator() { }

		explicit
		vector_iterator( iterator_type x ) : current(x) { }

		template< class U >
		vector_iterator( const vector_iterator<U>& other ) : current(other.base()) { }

		template< class U >
		vector_iterator&	operator=( const vector_iterator<U>& other ) {
			current = other.base();
			return *this;
		}

		iterator_type	base() const { return current; }

		reference	operator*() const { return *base(); }
		pointer		operator->() const { return &(operator*()); }

		reference	operator[]( difference_type n ) const { return base()[n]; }

		vector_iterator&	operator++() { ++current; return *this; }
		vector_iterator&	operator--() { --current; return *this; }
		vector_iterator		operator++( int ) { vector_iterator tmp = *this; ++(*this); return tmp; }
		vector_iterator		operator--( int ) { vector_iterator tmp = *this; --(*this); return tmp; }

		vector_iterator		operator+( difference_type n ) const { return vector_iterator(base() + n); }
		vector_iterator		operator-( difference_type n ) const { return vector_iterator(base() - n); }
		vector_iterator&	operator+=( difference_type n ) { current += n; return *this; }
		vector_iterator&	operator-=( difference_type n ) { current -= n; return *this; }

	protected:
		iterator_type	current;
	};

	template< class Iter1, class Iter2 >
	bool operator==( const ft::vector_iterator<Iter1>& lhs, const ft::vector_iterator<Iter2>& rhs ) {
		return lhs.base() == rhs.base();
	}

	template< class Iter1, class Iter2 >
	bool operator!=( const ft::vector_iterator<Iter1>& lhs, const ft::vector_iterator<Iter2>& rhs ) {
		return !(lhs == rhs);
	}

	template< class Iter1, class Iter2 >
	bool operator<( const ft::vector_iterator<Iter1>& lhs, const ft::vector_iterator<Iter2>& rhs ) {
		return lhs.base() < rhs.base();
	}

	template< class Iter1, class Iter2 >
	bool operator<=( const ft::vector_iterator<Iter1>& lhs, const ft::vector_iterator<Iter2>& rhs ) {
		return !(rhs < lhs);
	}

	template< class Iter1, class Iter2 >
	bool operator>( const ft::vector_iterator<Iter1>& lhs, const ft::vector_iterator<Iter2>& rhs ) {
		return rhs < lhs;
	}

	template< class Iter1, class Iter2 >
	bool operator>=( const ft::vector_iterator<Iter1>& lhs, const ft::vector_iterator<Iter2>& rhs ) {
		return !(lhs < rhs);
	}

	template< class Iter >
	vector_iterator<Iter> operator+( typename vector_iterator<Iter>::difference_type n,
			const vector_iterator<Iter>& it ) {
		return vector_iterator<Iter>(it.base() + n);
	}
			   
	// template< class Iter >
	// typename vector_iterator<Iter>::difference_type operator-( const vector_iterator<Iter>& lhs,
	// 		const vector_iterator<Iter>& rhs ) {
	// 	return lhs.base() - rhs.base();
	// }
			   
	template< class Iter1, class Iter2 >
	typename vector_iterator<Iter1>::difference_type operator-( const vector_iterator<Iter1>& lhs,
			const vector_iterator<Iter2>& rhs ) {
		return lhs.base() - rhs.base();
	}
}

#endif
