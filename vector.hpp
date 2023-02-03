//https://stdcxx.apache.org/doc/stdlibug/16-3.html
//https://stackoverflow.com/questions/7758580/writing-your-own-stl-container
//https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3242.pdf
//https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_vector.h
//http://www.fifi.org/doc/gcc-3.0/libstdc++/html_user/classstd_1_1vector.html
//https://linuxhint.com/vector-iterators-c/

// https://krshrimali.github.io/posts/2020/04/understanding-how-vectors-work-in-c-part-2-what-happens-when-you-initialize-a-vector/
// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/vector.tcc


#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <cstddef>
# include <iostream>
# include <sstream>
# include <algorithm>
# include "type_traits.hpp"
# include "vector_iterator.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"

namespace ft {
	template< class T, class Allocator = std::allocator<T> >
	class vector {
	public:
		typedef T					value_type;
		typedef Allocator			allocator_type;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef ft::vector_iterator<pointer>			iterator;
		typedef ft::vector_iterator<const_pointer>		const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		vector() : _alloc(Allocator()) { _M_create_storage(0); }

		explicit
		vector( const allocator_type& alloc ) : _alloc(alloc) { _M_create_storage(0); }

		explicit
		vector( size_type count, const_reference value = T(),
				const allocator_type& alloc = Allocator() ) : _alloc(alloc) {
			_M_create_storage(_S_check_init_len(count));
			_finish = std::__uninitialized_fill_n_a(_start, count, value, _alloc);
		}

		// https://eli.thegreenplace.net/2014/sfinae-and-enable_if/
		// https://stackoverflow.com/a/25483768
		template< class InputIt >
		vector( InputIt first, InputIt last,
				const allocator_type& alloc = Allocator(),
				typename ft::enable_if< !ft::is_integral<InputIt>::value >::type* = 0 )
		 : _alloc(alloc) {
			_M_range_initialize(first, last, typename ft::iterator_traits<InputIt>::iterator_category());
		}

		vector( const vector& other ) : _alloc(other.get_allocator()) {
			_M_create_storage(other.size());
			_finish = std::__uninitialized_copy_a(other.begin(), other.end(), _start, _alloc);
		}

		~vector() {
			clear();
			_M_deallocate(_start, capacity());
		}

		vector&	operator=( const vector& other ) {
			_alloc = other.get_allocator();
			clear();
			if (capacity() < other.capacity())
				reserve(other.size());
			_finish = std::__uninitialized_copy_a(other.begin(), other.end(), _start, _alloc);
			return *this;
		}

		void	assign( size_type count, const_reference value ) {
			if (count > capacity()) {
				vector	tmp(count, value);
				swap(tmp);
			} else if (count > size()) {
				std::fill(_start, _finish, value);
				_finish = std::__uninitialized_fill_n_a(_finish, count - size(), value, _alloc);
			} else {
				std::fill_n(_start, count, value);
				erase(begin() + count, end());
			}
		}

		template< class InputIt >
		void	assign( InputIt first, InputIt last,
				typename ft::enable_if< !ft::is_integral<InputIt>::value >::type* = 0 ) {
			_M_range_assign(first, last, typename ft::iterator_traits<InputIt>::iterator_category());
		}

		allocator_type	get_allocator() const { return _alloc; }

		reference		at( size_type pos ) { _M_range_check(pos); return (*this)[pos]; }
		const_reference	at( size_type pos ) const { _M_range_check(pos); return (*this)[pos]; }
		reference		operator[]( size_type pos ) { return *(_start + pos); }
		const_reference	operator[]( size_type pos ) const { return *(_start + pos); }

		reference		front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference		back() { return *(end() - 1); }
		const_reference	back() const { return *(end() - 1); }

		value_type*			data() { return empty() ? (value_type*)0 : _start.operator->(); /*_first;*/ }
		const value_type*	data() const { return _start; }

		iterator		begin() { return iterator(_start); }
		const_iterator	begin() const { return const_iterator(_start); }
		iterator		end() { return iterator(_finish); }
		const_iterator	end() const { return const_iterator(_finish); }
		reverse_iterator		rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator		rend() { return reverse_iterator(begin()); }
		const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

		bool		empty() const { return begin() == end(); }
		size_type	size() const { return _finish - _start; }
		size_type	max_size() const { return get_allocator().max_size(); }
		size_type	capacity() const { return _end_of_storage - _start; }

		void	reserve( size_type new_cap ) {
			if (new_cap > max_size())
				throw std::length_error("vector::reserve");
			if (capacity() < new_cap) {
				pointer	new_start(_M_allocate(_S_check_init_len(new_cap)));
				pointer	new_finish(new_start);
				try {
					new_finish = std::__uninitialized_copy_a(_start, _finish, new_start, _alloc);
				} catch (...) {
					_M_destroy(iterator(new_start), iterator(new_finish));
					_M_deallocate(new_start, new_cap);
				}
				clear();
				_M_deallocate(_start, capacity());
				_start = new_start;
				_finish = new_finish;
				_end_of_storage = _start + new_cap;
			}
		}

		void	clear() { erase(begin(), end()); }

		iterator	insert( iterator pos, const_reference value ) {
			const size_type	dist = pos - begin();
			insert(pos, 1, value);
			return begin() + dist;
		}

		void	insert( iterator pos, size_type count, const_reference value ) {
			_M_fill_insert(pos, count, value);
		}

		template< class InputIt >
		void	insert( iterator pos, InputIt first, InputIt last,
				typename ft::enable_if< !ft::is_integral<InputIt>::value >::type* = 0  ) {
			_M_range_insert(pos, first, last, typename ft::iterator_traits<InputIt>::iterator_category());
		}

		iterator	erase( iterator pos ) {
			if (pos < end()) {
				std::copy(pos + 1, end(), pos);
				_alloc.destroy(--_finish);
				return pos;
			}
			return end();
		}

		iterator	erase( iterator first, iterator last ) {
			const size_type	n = last - first;
			if (n > 0) {
				iterator	i(std::copy(last, end(), first));
				_M_destroy(i, end());
				_finish -= n;
			}
			return first;
		}

		void	push_back( const_reference value ) { insert(end(), value); }
		void	pop_back() { if (!empty()) erase(end() - 1); }

		void	resize( size_type count, T value = T() ) {
			if (count > size())
				insert(end(), count - size(), value);
			else if (count < size())
				erase(begin() + count, end());
		}

		void	swap( vector& other ) {
			std::swap(_start, other._start);
			std::swap(_finish, other._finish);
			std::swap(_end_of_storage, other._end_of_storage);
		}

	private:
		allocator_type	_alloc;
		pointer			_start;
		pointer			_finish;
		pointer			_end_of_storage;

		size_type	_S_check_init_len( size_type __n ) {
			if (__n > max_size())
				throw std::length_error("cannot create ft::vector larger than max_size()");
			return __n;
		}

		void	_M_copy_data( const vector& __x ) {
			_start = __x._start;
			_finish = __x._finish;
			_end_of_storage = __x._end_of_storage;
		}

		pointer	_M_allocate( size_type n ) {
			return n ? _alloc.allocate(n) : pointer();
		}

		void	_M_deallocate( pointer p, size_type n ) {
			if (p) _alloc.deallocate(p, n);
		}

		void	_M_create_storage( size_type n ) {
			_start = _M_allocate(n);
			_finish = _start;
			_end_of_storage = _start + n;
		}
		
		void	_M_destroy( iterator first, iterator last ) {
			for (; first != last; first++)
				_alloc.destroy(first.base());
		}

		template< class InputIt >
		void	_M_range_initialize( InputIt first, InputIt last, std::input_iterator_tag ) {
			_M_create_storage(0);
			for (; first != last; ++first)
				push_back(*first);
		}

		template< class ForwardIt >
		void	_M_range_initialize( ForwardIt first, ForwardIt last, std::forward_iterator_tag ) {
			_M_create_storage(_S_check_init_len(std::distance(first, last)));
			_finish = std::__uninitialized_copy_a(first, last, _start, _alloc);
		}

		template< class InputIt >
		void	_M_range_assign( InputIt first, InputIt last, std::input_iterator_tag ) {
			iterator	cur(begin());
			while (first != last && cur != end())
				*cur++ = *first++;
			if (first == last)
				erase(cur, end());
			else
				insert(end(), first, last);
		}

		template< class ForwardIt >
		void	_M_range_assign( ForwardIt first, ForwardIt last, std::forward_iterator_tag ) {
			size_type	len = std::distance(first, last);
			if (len > capacity()) {
				reserve(len);
				_M_range_assign(first, last, typename ft::iterator_traits<ForwardIt>::iterator_category());
			} else if (size() >= len) {
				iterator	new_finish(std::copy(first, last, _start));
				_M_destroy(new_finish, end());
				_finish = new_finish.base();
			} else {
				ForwardIt	mid = first;
				std::advance(mid, size());
				std::copy(first, mid, _start);
				_finish = std::__uninitialized_copy_a(mid, last, _finish, _alloc);
			}
		}

		template< class InputIt >
		void	_M_range_insert( iterator pos, InputIt first, InputIt last, std::input_iterator_tag ) {
			for (; first != last; ++first, ++pos)
				pos = insert(pos, *first);
		}

		template< class ForwardIt >
		void	_M_range_insert( iterator pos, ForwardIt first, ForwardIt last, std::forward_iterator_tag ) {
			if (first != last) {
				size_type	n = std::distance(first, last);
				if (size_type(_end_of_storage - _finish) >= n) {
					const size_type	elems_after = end() - pos;
					iterator		old_finish(_finish);
					if (elems_after > n) {
						_finish = std::__uninitialized_copy_a(_finish - n, _finish, _finish, _alloc);
						std::copy_backward(pos, old_finish - n, old_finish);
					//	std::copy(first, last, pos);
						std::copy_backward(first, last, pos + n);
					} else {
						ForwardIt	mid = first;
						std::advance(mid, elems_after);
						_finish = std::__uninitialized_copy_a(mid, last, _finish, _alloc);
						_finish = std::__uninitialized_copy_a(pos, old_finish, _finish, _alloc);
						std::copy_backward(first, mid, old_finish);
					}
				} else {
					_M_check_len(n, "vector::_M_range_insert");
					const size_type	dist = pos - begin();
					reserve(size() + std::max(size(), n));
					_M_range_insert(begin() + dist, first, last, typename ft::iterator_traits<ForwardIt>::iterator_category());
				}
			}
		}

		void	_M_fill_insert( iterator pos, size_type n, const_reference x ) {
			if (n != 0) {
				if (size_type(_end_of_storage - _finish) >= n) {
					const size_type	elems_after = end() - pos;
					iterator		old_finish(_finish);
					if (elems_after > n) {
						_finish = std::__uninitialized_copy_a(_finish - n, _finish, _finish, _alloc);
						std::copy_backward(pos, old_finish - n, old_finish);
						std::fill(pos, pos + n, x);
					} else {
						_finish = std::__uninitialized_fill_n_a(_finish, n - elems_after, x, _alloc);
						_finish = std::__uninitialized_copy_a(pos, old_finish, _finish, _alloc);
						std::fill(pos, old_finish, x);
					}
				} else {
					_M_check_len(n, "vector::_M_fill_insert");
					const size_type	dist = pos - begin();
					reserve(size() + std::max(size(), n));
					_M_fill_insert(begin() + dist, n, x);
				}
			}
		}

		void	_M_range_check( size_type n ) const {
			std::string	n_str = static_cast<std::ostringstream*>( &(std::ostringstream() << n) )->str();
			std::string	size_str = static_cast<std::ostringstream*>( &(std::ostringstream() << size()) )->str();
			if (n >= size())
				throw std::out_of_range("vector::_M_range_check: __n (which is "
						+ n_str + ") >= this->size() (which is " + size_str + ")");
		}

		size_type	_M_check_len( size_type n, const std::string& s ) const {
			if (max_size() - size() < n)
				throw std::length_error(s);
			const size_type	len = size() + std::max(size(), n);
			return (len < size() || len > max_size()) ? max_size() : len;
		}

		// template< class InputIt, class NoThrowForwardIt >
		// NoThrowForwardIt	_uninitialized_copy( InputIt first, InputIt last, NoThrowForwardIt d_first ) {
		// 	for (; first != last; ++first, ++d_first)
		// 		_alloc.construct(d_first, typename ft::iterator_traits<InputIt>::value_type(*first));
		// 	return d_first;
		// }

		// template< class ForwardIt >
		// ForwardIt	_uninitialized_fill_n( ForwardIt d_first, size_type count, const_reference value ) {
		// 	for (size_type i = 0; i < count; ++i, ++d_first)
		// 		_alloc.construct(d_first, value);
		// 	return d_first;
		// }
	};

	template< class T, class Alloc >
	bool	operator==( const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs ) {
		return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template< class T, class Alloc >
	bool	operator!=( const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs ) {
		return !(lhs == rhs);
	}

	template< class T, class Alloc >
	bool	operator<( const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs ) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template< class T, class Alloc >
	bool	operator<=( const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs ) {
		return !(rhs < lhs);
	}

	template< class T, class Alloc >
	bool	operator>( const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs ) {
		return rhs < lhs;
	}

	template< class T, class Alloc >
	bool	operator>=( const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs ) {
		return !(lhs < rhs);
	}

	template< class T, class Alloc >
	void	swap( ft::vector<T, Alloc>& lhs, ft::vector<T, Alloc>& rhs ) {
		lhs.swap(rhs);
	}
}

#endif
