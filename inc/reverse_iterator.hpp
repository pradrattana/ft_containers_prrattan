//https://en.cppreference.com/w/cpp/iterator/reverse_iterator

#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include <iterator_base.hpp>

namespace ft {
	template< class Iter >
	/*class reverse_iterator : public ft::iterator<
		typename ft::iterator_traits<Iter>::iterator_category,
		typename ft::iterator_traits<Iter>::value_type,
		typename ft::iterator_traits<Iter>::difference_type,
		typename ft::iterator_traits<Iter>::pointer
		typename ft::iterator_traits<Iter>::reference
	> {*/
	class reverse_iterator {
	public:
		typedef Iter													iterator_type;
		typedef typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
		typedef typename ft::iterator_traits<Iter>::value_type			value_type;
		typedef typename ft::iterator_traits<Iter>::difference_type		difference_type;
		typedef typename ft::iterator_traits<Iter>::pointer				pointer;
		typedef typename ft::iterator_traits<Iter>::reference			reference;
	
	//public:
		reverse_iterator() {}
		explicit reverse_iterator( iterator_type x ) : current(x) {}
		template< class U >
		reverse_iterator( const reverse_iterator<U>& other ) : current(other.base()) {}

		template< class U >
		reverse_iterator& operator=( const reverse_iterator<U>& other ) { current = other.base(); return *this; }

		iterator_type	base() const { return current; }

		reference operator*() const { return *(base() - 1); }
		pointer operator->() const { return &(operator*()); }

		reference operator[]( difference_type n ) const { return base()[-n - 1]; }

		reverse_iterator& operator++() { --current; return *this; }
		reverse_iterator& operator--() { ++current; return *this; }
		reverse_iterator& operator++( int ) { reverse_iterator tmp = *this; ++(*this); return tmp; }
		reverse_iterator& operator--( int ) { reverse_iterator tmp = *this; --(*this); return tmp; }
	
		reverse_iterator operator+( difference_type n ) const { return reverse_iterator(base() - n); }
		reverse_iterator operator-( difference_type n ) const { return reverse_iterator(base() + n); }
		reverse_iterator& operator+=( difference_type n ) { current -= n; return *this; }
		reverse_iterator& operator-=( difference_type n ) { current += n; return *this; }

	protected:
		iterator_type	current;
	};

	template< class Iterator1, class Iterator2 >
	bool operator==( const ft::reverse_iterator<Iterator1>& lhs,
		const ft::reverse_iterator<Iterator2>& rhs ) { return lhs.base() == rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator!=( const ft::reverse_iterator<Iterator1>& lhs,
		const ft::reverse_iterator<Iterator2>& rhs ) { return lhs.base() != rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator<( const ft::reverse_iterator<Iterator1>& lhs,
		const ft::reverse_iterator<Iterator2>& rhs ) { return lhs.base() > rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator<=( const ft::reverse_iterator<Iterator1>& lhs,
		const ft::reverse_iterator<Iterator2>& rhs ) { return lhs.base() >= rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator>( const ft::reverse_iterator<Iterator1>& lhs,
		const ft::reverse_iterator<Iterator2>& rhs ) { return lhs.base() < rhs.base(); }

	template< class Iterator1, class Iterator2 >
	bool operator>=( const ft::reverse_iterator<Iterator1>& lhs,
		const ft::reverse_iterator<Iterator2>& rhs ) { return lhs.base() <= rhs.base(); }

	template< class Iter >
	reverse_iterator<Iter> operator+( typename reverse_iterator<Iter>::difference_type n,
		const reverse_iterator<Iter>& it ) { return reverse_iterator<Iter>(it.base() - n); }
			   
	template< class Iterator >
	typename reverse_iterator<Iterator>::difference_type operator-( const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs ) { return rhs.base() - lhs.base() }
}

#endif
