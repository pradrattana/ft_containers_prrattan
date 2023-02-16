
//https://gcc.gnu.org/onlinedocs/gcc-4.6.2/libstdc++/api/a01056_source.html
//https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.1/stl__function_8h-source.html#l00542

#ifndef MAP_HPP
# define MAP_HPP

# include <memory>
# include <cstddef>
# include <algorithm>
# include "type_traits.hpp"
# include "rb_tree.hpp"
# include "algorithm.hpp"
# include "utility.hpp"

namespace ft {
	template< class _Pair >
	struct _Select1st : public std::unary_function<_Pair, typename _Pair::first_type> {
		typename _Pair::first_type&	operator()(_Pair& __x) const
		{ return __x.first; }

		const typename _Pair::first_type&	operator()(const _Pair& __x) const
		{ return __x.first; }
	};

	template<
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<ft::pair<const Key, T> >
	> class map {
	public:
		typedef Key						key_type;
		typedef T						mapped_type;
		typedef ft::pair<const Key, T>	value_type;
		// typedef std::size_t				size_type;
		// typedef std::ptrdiff_t			difference_type;
		typedef Compare					key_compare;
		typedef Allocator				allocator_type;
		// typedef value_type&				reference;
		// typedef const value_type&		const_reference;
		// typedef typename Allocator::pointer				pointer;
		// typedef typename Allocator::const_pointer		const_pointer;
		// typedef ft::reverse_iterator<iterator>			reverse_iterator;
		// typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		typedef typename Allocator::template rebind<value_type>::other	_Pair_alloc_type;

		typedef ft::_Rb_tree<key_type, value_type, ft::_Select1st<value_type>,
				key_compare, _Pair_alloc_type>	_Rep_type;
	
	public:
		typedef typename _Rep_type::size_type				size_type;
		typedef typename _Rep_type::difference_type			difference_type;
		typedef typename _Pair_alloc_type::reference		reference;
		typedef typename _Pair_alloc_type::const_reference	const_reference;
		typedef typename _Pair_alloc_type::pointer			pointer;
		typedef typename _Pair_alloc_type::const_pointer	const_pointer;
		typedef typename _Rep_type::iterator				iterator;
		typedef typename _Rep_type::const_iterator			const_iterator;
		typedef typename _Rep_type::reverse_iterator		reverse_iterator;
		typedef typename _Rep_type::const_reverse_iterator	const_reverse_iterator;

		class value_compare : public std::binary_function<value_type, value_type, bool> {
		public:
			bool operator()( const value_type& lhs, const value_type& rhs ) const {
				return comp(lhs.first, rhs.first);
			}

		protected:
			Compare	comp;
	
			value_compare( Compare c ) : comp(c) {}

		private:
			friend class map;
		};

		map() { }

		explicit
		map( const Compare& comp, const Allocator& alloc = Allocator() )
		 : _M_t(comp, alloc) { }

		template< class InputIt >
		map( InputIt first, InputIt last, const Compare& comp = Compare(),
				const Allocator& alloc = Allocator() )
		 : _M_t(comp, alloc) { _M_t.insert_range_unique(first, last); }

		map( const map& other ) : _M_t(other._M_t) { }

		~map() { }

		map& operator=( const map& other ) { _M_t = other._M_t; return *this; }

		allocator_type get_allocator() const { return _M_t.get_allocator(); }

		mapped_type&		at( const key_type& key ) {
			iterator	i = lower_bound(key);
			if (i == end() || key_comp()(key, i->first))
				throw std::out_of_range("map::at");
			return i->second;
		}

		const mapped_type&	at( const key_type& key ) const {
			const_iterator	i = lower_bound(key);
			if (i == end() || key_comp()(key, i->first))
				throw std::out_of_range("map::at");
			return i->second;
		}

		mapped_type&		operator[]( const key_type& key ) {
			iterator __i = lower_bound(key);
			if (__i == end() || key_comp()(key, __i->first))
				__i = insert(__i, value_type(key, mapped_type()));
			return __i->second;
		}

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

		void		erase( iterator pos ) { _M_t.erase(pos); }
		void		erase( iterator first, iterator last ) { _M_t.erase(first, last); }
		size_type	erase( const key_type& key ) { return _M_t.erase(key); }

		void	swap( map& other ) { _M_t.swap(other._M_t); }

		size_type	count( const key_type& key ) const { return find(key) == end() ? 0 : 1; }

		iterator		find( const key_type& key ) { return _M_t.find(key); }
		const_iterator	find( const key_type& key ) const { return _M_t.find(key); }

		ft::pair<iterator, iterator>				equal_range( const key_type& key ) { return _M_t.equal_range(key); }
		ft::pair<const_iterator, const_iterator>	equal_range( const key_type& key ) const { return _M_t.equal_range(key); }

		iterator		lower_bound( const key_type& key ) { return _M_t.lower_bound(key); }
		const_iterator	lower_bound( const key_type& key ) const { return _M_t.lower_bound(key); }
		iterator		upper_bound( const key_type& key ) { return _M_t.upper_bound(key); }
		const_iterator	upper_bound( const key_type& key ) const { return _M_t.upper_bound(key); }

		key_compare		key_comp() const { return _M_t.key_comp(); }
		value_compare	value_comp() const { return value_compare(_M_t.key_comp()); }

	private:
		_Rep_type	_M_t;

		template< class _Key, class _T, class _Compare, class _Alloc >
		friend bool	operator==( const ft::map<_Key, _T, _Compare, _Alloc>&,
			const ft::map<_Key, _T, _Compare, _Alloc>& );

		template< class _Key, class _T, class _Compare, class _Alloc >
		friend bool	operator<( const ft::map<_Key, _T, _Compare, _Alloc>&,
			const ft::map<_Key, _T, _Compare, _Alloc>& );
	};

	template< class Key, class T, class Compare, class Alloc >
	bool	operator==( const ft::map<Key, T, Compare, Alloc>& lhs,
			const ft::map<Key, T, Compare, Alloc>& rhs ) {
		return lhs._M_t == rhs._M_t;
	}

	template< class Key, class T, class Compare, class Alloc >
	bool	operator!=( const ft::map<Key, T, Compare, Alloc>& lhs,
			const ft::map<Key, T, Compare, Alloc>& rhs ) {
		return !(lhs == rhs);
	}

	template< class Key, class T, class Compare, class Alloc >
	bool	operator<( const ft::map<Key, T, Compare, Alloc>& lhs,
			const ft::map<Key, T, Compare, Alloc>& rhs ) {
		return lhs._M_t < rhs._M_t;
	}

	template< class Key, class T, class Compare, class Alloc >
	bool	operator<=( const ft::map<Key, T, Compare, Alloc>& lhs,
			const ft::map<Key, T, Compare, Alloc>& rhs ) {
		return !(rhs < lhs);
	}

	template< class Key, class T, class Compare, class Alloc >
	bool	operator>( const ft::map<Key, T, Compare, Alloc>& lhs,
			const ft::map<Key, T, Compare, Alloc>& rhs ) {
		return rhs < lhs;
	}

	template< class Key, class T, class Compare, class Alloc >
	bool	operator>=( const ft::map<Key, T, Compare, Alloc>& lhs,
			const ft::map<Key, T, Compare, Alloc>& rhs ) {
		return !(lhs < rhs);
	}

	template< class Key, class T, class Compare, class Alloc >
	void	swap( ft::map<Key, T, Compare, Alloc>& lhs,
			ft::map<Key, T, Compare, Alloc>& rhs ) {
		lhs.swap(rhs);
	}

}

#endif
