//https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp

#ifndef GENERAL_ITERATOR_HPP
# define GENERAL_ITERATOR_HPP

# include <iterator_base.hpp>

namespace ft {
	template< typename T >
	class general_iterator {
	public:
		typedef T													iterator_type;
		typedef typename ft::iterator_traits<T>::iterator_category	iterator_category;
		typedef typename ft::iterator_traits<T>::value_type			value_type;
		typedef typename ft::iterator_traits<T>::difference_type	difference_type;
		typedef typename ft::iterator_traits<T>::pointer			pointer;
		typedef typename ft::iterator_traits<T>::reference			reference;

		general_iterator() {}
		explicit general_iterator( iterator_type x ) : current(x) {}
		template< class U >
		general_iterator( const general_iterator<U>& other ) : current(other.base()) {}

		template< class U >
		general_iterator& operator=( const general_iterator<U>& other ) { current = other.base(); return *this; }

		iterator_type base() const { return current; }

		reference operator*() const { return *(base() - 1); }//
		pointer operator->() const { return &(operator*()); }

		reference operator[]( difference_type n ) const { return base()[-n - 1]; }//

		general_iterator& operator++() { ++current; return *this; }
		general_iterator& operator--() { --current; return *this; }
		general_iterator& operator++( int ) { general_iterator tmp = *this; ++(*this); return tmp; }
		general_iterator& operator--( int ) { general_iterator tmp = *this; --(*this); return tmp; }
	
		general_iterator operator+( difference_type n ) const { return general_iterator(base() + n); }
		general_iterator operator-( difference_type n ) const { return general_iterator(base() - n); }
		general_iterator& operator+=( difference_type n ) { current += n; return *this; }
		general_iterator& operator-=( difference_type n ) { current -= n; return *this; }

	protected:
		iterator_type	current;
	};

	template< class Iterator1, class Iterator2 >
	bool operator==( const ft::general_iterator<Iterator1>& lhs,
		const ft::general_iterator<Iterator2>& rhs ) { return lhs.base() == rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator!=( const ft::general_iterator<Iterator1>& lhs,
		const ft::general_iterator<Iterator2>& rhs ) { return lhs.base() != rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator<( const ft::general_iterator<Iterator1>& lhs,
		const ft::general_iterator<Iterator2>& rhs ) { return lhs.base() < rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator<=( const ft::general_iterator<Iterator1>& lhs,
		const ft::general_iterator<Iterator2>& rhs ) { return lhs.base() <= rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator>( const ft::general_iterator<Iterator1>& lhs,
		const ft::general_iterator<Iterator2>& rhs ) { return lhs.base() > rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator>=( const ft::general_iterator<Iterator1>& lhs,
		const ft::general_iterator<Iterator2>& rhs ) { return lhs.base() >= rhs.base(); }

	template< class Iter >
	general_iterator<Iter> operator+( typename general_iterator<Iter>::difference_type n,
		const general_iterator<Iter>& it ) { return general_iterator<Iter>(it.base() + n); }
			   
	template< class Iterator >
	typename general_iterator<Iterator>::difference_type operator-( const general_iterator<Iterator>& lhs,
		const general_iterator<Iterator>& rhs ) { return lhs.base() - rhs.base() }
}

#endif
