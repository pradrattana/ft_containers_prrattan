//https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_stack.h
// https://en.cppreference.com/w/cpp/container/stack

#ifndef STACK_HPP
# define STACK_HPP

# include <memory>
# include <cstddef>
# include <iostream>
# include <sstream>
# include <algorithm>
# include "type_traits.hpp"
# include "vector.hpp"
# include "algorithm.hpp"

namespace ft {
	template< class T, class Container = ft::vector<T> >
	class stack {
	public:
		typedef Container							container_type;
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;

		stack() { }

		explicit
		stack( const container_type& cont ) : c(cont) { }

		stack( const stack& other ) : c(other.c) { }

		~stack() { }

		stack& operator=( const stack& other ) { c = other.c; return *this; }

		reference		top() { return c.back(); }
		const_reference	top() const { return c.back(); }

		bool		empty() const { return c.empty(); }
		size_type	size() const { return c.size(); }

		void	push( const_reference value ) { c.push_back(value); }
		void	pop() { c.pop_back(); }

	protected:
		container_type	c;

	private:
		template< class _T, class _Container >
		friend bool	operator==( const ft::stack<_T, _Container>&, const ft::stack<_T, _Container>& );
		
		template< class _T, class _Container >
		friend bool	operator<( const ft::stack<_T, _Container>&, const ft::stack<_T, _Container>& );
	};

	template< class T, class Container >
	bool	operator==( const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs ) {
		return lhs.c == rhs.c;
	}

	template< class T, class Container >
	bool	operator!=( const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs ) {
		return !(lhs == rhs);
	}

	template< class T, class Container >
	bool	operator<( const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs ) {
		return lhs.c < rhs.c;
	}

	template< class T, class Container >
	bool	operator<=( const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs ) {
		return !(rhs < lhs);
	}

	template< class T, class Container >
	bool	operator>( const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs ) {
		return rhs < lhs;
	}

	template< class T, class Container >
	bool	operator>=( const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs ) {
		return !(lhs < rhs);
	}
}

#endif
