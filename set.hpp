// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_function.h
//https://gcc.gnu.org/onlinedocs/gcc-4.6.2/libstdc++/api/a01064_source.html

#ifndef SET_HPP
# define SET_HPP

# include <memory>
# include <cstddef>
# include <algorithm>
# include "type_traits.hpp"
# include "rb_tree.hpp"
# include "algorithm.hpp"
# include "utility.hpp"

namespace ft {
	template< class _T >
	struct _Identity : public std::unary_function<_T, _T> {
		_T&	operator()(_T& __x) const
		{ return __x; }

		const _T&	operator()(const _T& __x) const
		{ return __x; }
	};

	template<
		class Key,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<Key>
	> class set {
	public:
		typedef Key			key_type;
		typedef Key			value_type;
		typedef Compare		key_compare;
		typedef Compare		value_compare;
		typedef Allocator	allocator_type;

	private:
		typedef typename Allocator::template rebind<value_type>::other	_Key_alloc_type;

		typedef ft::_Rb_tree<key_type, value_type, ft::_Identity<value_type>,
				key_compare, _Key_alloc_type>	_Rep_type;

	public:
		typedef typename _Rep_type::size_type			size_type;
		typedef typename _Rep_type::difference_type		difference_type;
		// typedef value_type&								reference;
		// typedef const value_type&						const_reference;
		// typedef typename allocator_type::pointer		pointer;
		// typedef typename allocator_type::const_pointer	const_pointer;
		typedef typename _Key_alloc_type::reference			reference;
		typedef typename _Key_alloc_type::const_reference	const_reference;
		typedef typename _Key_alloc_type::pointer			pointer;
		typedef typename _Key_alloc_type::const_pointer		const_pointer;
		typedef typename _Rep_type::iterator				iterator;
		typedef typename _Rep_type::const_iterator			const_iterator;
		typedef typename _Rep_type::reverse_iterator		reverse_iterator;
		typedef typename _Rep_type::const_reverse_iterator	const_reverse_iterator;

		set() {}

		explicit
		set( const Compare& comp, const Allocator& alloc = Allocator() )
		 : _M_t(comp, alloc) { } //: comp(comp), _alloc(alloc) {};

		template< class InputIt >
		set( InputIt first, InputIt last, const Compare& comp = Compare(),
				const Allocator& alloc = Allocator() )
		 : _M_t(comp, alloc) { _M_t.insert_range_unique(first, last); }

		set( const set& other ) : _M_t(other._M_t) { }

		~set() { }

		set& operator=( const set& other ) { _M_t = other._M_t; return *this; }

		allocator_type get_allocator() const { return _M_t.get_allocator(); }

		// Iterators
		iterator		begin() { return _M_t.begin(); }
		const_iterator	begin() const { return _M_t.begin(); }
		iterator		end() { return _M_t.end(); }
		const_iterator	end() const { return _M_t.end(); }
		reverse_iterator		rbegin() { return _M_t.rbegin(); }
		const_reverse_iterator	rbegin() const { return _M_t.rbegin(); }
		reverse_iterator		rend() { return _M_t.rend(); }
		const_reverse_iterator	rend() const { return _M_t.rend(); }

		bool		empty() const { return _M_t.empty(); }
		size_type	size() const { return _M_t.size(); }
		size_type	max_size() const { return _M_t.max_size(); }

		void clear() { _M_t.clear(); }

		ft::pair<iterator, bool>	insert( const value_type& value )
		{ return _M_t.insert_unique(value); }

		iterator	insert( iterator pos, const value_type& value )
		{ return _M_t.insert_unique(pos, value); }

		template< class InputIt >
		void	insert( InputIt first, InputIt last )
		{ _M_t.insert_range_unique(first, last); }

		void		erase( iterator pos ) { return _M_t.erase(pos); }
		void		erase( iterator first, iterator last ) { return _M_t.erase(first, last); }
		size_type	erase( const key_type& key ) { return _M_t.erase(key); }

		void	swap( set& other ) { _M_t.swap(other._M_t); }

		size_type	count( const key_type& key ) const { return _M_t.find(key) == _M_t.end() ? 0 : 1; }

		iterator		find( const key_type& key ) { return _M_t.find(key); }
		const_iterator	find( const key_type& key ) const { return _M_t.find(key); }

		ft::pair<iterator, iterator>				equal_range( const key_type& key ) { return _M_t.equal_range(key); }
		ft::pair<const_iterator, const_iterator>	equal_range( const key_type& key ) const { return _M_t.equal_range(key); }

		iterator		lower_bound( const key_type& key ) { return _M_t.lower_bound(key); }
		const_iterator	lower_bound( const key_type& key ) const { return _M_t.lower_bound(key); }
		iterator		upper_bound( const key_type& key ) { return _M_t.upper_bound(key); }
		const_iterator	upper_bound( const key_type& key ) const { return _M_t.upper_bound(key); }

		key_compare		key_comp() const { return _M_t.key_comp(); }
		value_compare	value_comp() const { return key_comp(); }

	private:
		_Rep_type	_M_t;

		template< class _Key, class _Compare, class _Alloc >
		friend bool	operator==( const ft::set<_Key, _Compare, _Alloc>&,
			const ft::set<_Key, _Compare, _Alloc>& );

		template< class _Key, class _Compare, class _Alloc >
		friend bool	operator<( const ft::set<_Key, _Compare, _Alloc>&,
			const ft::set<_Key, _Compare, _Alloc>& );
	};

	template< class Key, class Compare, class Alloc >
	bool	operator==( const ft::set<Key, Compare, Alloc>& lhs,
			const ft::set<Key, Compare, Alloc>& rhs ) {
		return lhs._M_t == rhs._M_t;
	}

	template< class Key, class Compare, class Alloc >
	bool	operator!=( const ft::set<Key, Compare, Alloc>& lhs,
			const ft::set<Key, Compare, Alloc>& rhs ) {
		return !(lhs == rhs);
	}

	template< class Key, class Compare, class Alloc >
	bool	operator<( const ft::set<Key, Compare, Alloc>& lhs,
			const ft::set<Key, Compare, Alloc>& rhs ) {
		return lhs._M_t < rhs._M_t;
	}

	template< class Key, class Compare, class Alloc >
	bool	operator<=( const ft::set<Key, Compare, Alloc>& lhs,
			const ft::set<Key, Compare, Alloc>& rhs ) {
		return !(rhs < lhs);
	}

	template< class Key, class Compare, class Alloc >
	bool	operator>( const ft::set<Key, Compare, Alloc>& lhs,
			const ft::set<Key, Compare, Alloc>& rhs ) {
		return rhs < lhs;
	}

	template< class Key, class Compare, class Alloc >
	bool	operator>=( const ft::set<Key, Compare, Alloc>& lhs,
			const ft::set<Key, Compare, Alloc>& rhs ) {
		return !(lhs < rhs);
	}

	template< class Key, class Compare, class Alloc >
	void	swap( ft::set<Key, Compare, Alloc>& lhs,
			ft::set<Key, Compare, Alloc>& rhs ) {
		lhs.swap(rhs);
	}


}

#endif
