//https://stdcxx.apache.org/doc/stdlibug/16-3.html
//https://stackoverflow.com/questions/7758580/writing-your-own-stl-container
//https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3242.pdf
//https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_vector.h
//http://www.fifi.org/doc/gcc-3.0/libstdc++/html_user/classstd_1_1vector.html
//https://linuxhint.com/vector-iterators-c/


#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <cstddef>
# include <algorithm>
# include "general_iterator.hpp"
# include "reverse_iterator.hpp"
# include "type_traits.hpp"

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
		typedef ft::vector_iterator<value_type>			iterator;
		typedef ft::vector_iterator<const value_type>	const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		vector() {}

		explicit
		vector( const allocator_type& alloc ) : _alloc(alloc) { }

		explicit
		vector( size_type count, const_reference value = T(),
				const allocator_type& alloc = Allocator() ) : _alloc(alloc) {
			_M_create_storage(_S_check_init_len(count));
			_M_fill_initialize(count, value);
		}

		template< class InputIt >
		vector( InputIt first, InputIt last,
				const allocator_type& alloc = Allocator() ) : _alloc(alloc) {
			typedef typename ft::_Is_integer<InputIt>::_Integral	_Integral;
			if (_Integral() == ft::__true_type) { //like in case vect<int>(5, 10) https://stackoverflow.com/questions/70227294/c98-use-iterator-constructor-only-if-inputit-is-an-iterator-of-type-t
				_M_create_storage(_S_check_init_len(first));
				_M_fill_initialize(first, last);
			} else {
				_M_create_storage(_S_check_init_len(std::distance(first, last)));
				_M_range_initialize(first, last, std::__iterator_category(first));
			}
		}

		vector( const vector& other ) : _alloc(other.get_allocator()) {
			_M_create_storage(other.size());
			_M_copy_data(other);
		}

		~vector() {
			_M_destroy(begin(), end());
			_M_deallocate(_start, _end_of_storage - _start);
		}

		vector& operator=( const vector& other ) { _alloc = other.get_allocator(); return *this; }

		void assign( size_type count, const_reference value ) {
			_M_fill_assign(count, value);
		}

		template< class InputIt >
		void assign( InputIt first, InputIt last ) {
			typedef typename ft::_Is_integer<InputIt>::_Integral	_Integral;
			//_M_assign_dispatch(first, last, _Integral());
			if (_Integral() == ft::__true_type)
				_M_fill_assign(first, last);
			else {
				//_M_assign_aux(first, last);
				iterator	cur(begin());
				for (; first != last && cur != end(); first++) {
					*cur = *first;
					cur++;
				}
				if (first == last)
					erase(cur, end());
				else
					insert(end(), cur, last);
			}
		}

		allocator_type get_allocator() const { return _alloc; }

		reference		at( size_type pos ) { _M_range_check(pos); return (*this)[pos]; }
		const_reference	at( size_type pos ) const { _M_range_check(pos); return (*this)[pos]; }
		reference		operator[]( size_type pos ) { return *(_start + pos); }
		const_reference	operator[]( size_type pos ) const { return *(_start + pos); }

		reference		front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference		back() { return *(end() - 1); }
		const_reference	back() const { return *(end() - 1); }

		value_type*			data() { return empty() ? (value_type*)0 : _first.operator->(); /*_first;*/ }
		const value_type*	data() const { return _first; }

		// Iterators
		iterator		begin() { return iterator(_start); }
		const_iterator	begin() const { return const_iterator(_start); }
		iterator		end() { return iterator(_finish); }
		const_iterator	end() const { return const_iterator(_finish); }
		reverse_iterator		rbegin() { return reverse_iterator(end()); }
		const_reverse_iiterator	rbegin() const { return const_reverse_iiterator(end()); }
		reverse_iiterator		rend() { return reverse_iterator(begin()); }
		const_reverse_iiterator	rend() const { return const_reverse_iiterator(begin()); }

		bool		empty() const { return begin() == end(); }
		size_type	size() const { return std::distance(begin(), end()); }
		size_type	max_size() const { return get_allocator().max_size(); }
		size_type	capacity() const { return _end_of_storage - _start; }

		void		reserve( size_type new_cap ) {
			if (capacity() < new_cap) {
				iterator	new_start(_M_allocate(_S_check_init_len(new_cap)));
				iterator	new_finish(new_start);
				try {
					new_finish = std::uninitialized_copy(begin(), end(), new_start);
				} catch (...) {
					_M_destroy(new_start, new_finish);
					_M_deallocate(new_start.base(), new_cap);
				}
				_M_destroy(begin(), end());
				_M_deallocate(_start, _end_of_storage - _start);
				_start = new_start.base();
				_finish = new_finish.base();
				_end_of_storage = new_start.base() + new_cap;
			}
		}
		

		void clear() { erase(begin(), end()); }

		iterator	insert( const_iterator pos, const_reference value ) {
			//size_type	__n = pos - begin();
			if (_finish != _end_of_storage && pos == end())
				_alloc.construct(_finish++, value);
			else {
			//	_M_insert_aux(pos, value);
				if (_finish != _end_of_storage) {
					_alloc.construct(_finish, *(_finish - 1));
					++_finish;
					std::copy_backward(pos, end() - 2, end() - 1);
					*pos = value;
				} else {
					reserve(size() != 0 ? size() * 2 : 1);
					insert(pos, value);
			}
			return pos; //begin() + __n;
		}

		iterator	insert( const_iterator pos, size_type count, const_reference value ) {
			_M_fill_insert(pos, count, value);
			return pos;
		}

		template< class InputIt >
		iterator	insert( const_iterator pos, InputIt first, InputIt last )  {
			typedef typename ft::_Is_integer<InputIt>::_Integral	_Integral;
			if (_Integral() == ft::__true_type)
				_M_fill_insert(pos, first, last);
			else
				_M_range_insert(pos, first, last);
			return pos;
		}

		iterator	erase( iterator pos ) { return _M_erase(pos, end()); }
		iterator	erase( iterator first, iterator last ) { return _M_erase(first, last); }

		void	push_back( const_reference value ) { insert(end(), value); }
		void	pop_back() { if (!empty()) erase(end() - 1); }

		void	resize( size_type count, T value = T() ) {
			if (count > size())
				_M_fill_insert(end(), count - size(), value);
			else if (count < size())
				_M_erase(begin() + count, end());
				//_M_erase_at_end(_start + count);
		}

		void	swap( vector& other ) {
			_M_swap_data(other);
		}

	private:
		allocator_type	_alloc;
		pointer			_start;
		pointer			_finish;
		pointer			_end_of_storage;

		static size_type	_S_check_init_len( size_type __n ) {
			if (__n > max_size())
				throw std::length_error("cannot create ft::vector larger than max_size()");
			return __n;
		}

		void	_M_copy_data( const vector& __x ) {
			_start = __x._start;
			_finish = __x._finish;
			_end_of_storage = __x._end_of_storage;
		}

		void	_M_swap_data( vector& __x ) {
			vector __tmp;
			__tmp._M_copy_data(*this);
			_M_copy_data(__x);
			__x._M_copy_data(__tmp);
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

		/*pointer	_M_allocate_and_copy( size_type __n, pointer __first, pointer __last ) {
			pointer __result = _M_allocate(__n);
			try {
				std::uninitialized_copy(__first, __last, __result);
				return __result;
			} catch (...) {
				_M_deallocate(__result, __n);
			}
		}*/

		void	_M_fill_initialize( size_type n, const_reference value ) {
			//_finish = std::uninitialized_fill_n(_start, __n, __value);
			std::uninitialized_fill_n(_start, n, value);
			_finish = _start + n;
		}

		template< typename InputIter >
		void	_M_range_initialize( InputIter first, InputIter last, std::input_iterator_tag ) {
			try {
				for (; first != last; first++)
					push_back(*first);
			} catch (...) {
				clear();
			}
		}

		template< class ForwardIter >
		void	_M_range_initialize( ForwardIter first, ForwardIter last, std::forward_iterator_tag ) {
			//const size_type	n = std::distance(first, last);
			//_start = _M_allocate(_S_check_init_len(n));
			//_end_of_storage = _start + n;
			_finish = std::uninitialized_copy(first, last, _start);
		}

		void	_M_fill_assign( size_type n, const_reference val ) {
			const size_type	old_size = size();
			if (n > capacity()) {
				ft::vector<value_type>	tmp(n, val);
				tmp.swap(*this);
			} else if (n > old_size) {
				std::fill(begin(), end(), val);
				//_finish = std::uninitialized_fill_n(_finish, n - size(), val);
				std::uninitialized_fill_n(_finish, n - old_size, val);
				_finish = _finish + n - old_size;
			} else {
				//erase(std::fill_n(begin(), n, val), end());
				std::fill_n(begin(), n, val);
				erase(begin() + n, end());
			}
		}

		/*void	_M_assign_aux( iterator first, iterator last ) {
			iterator	cur(begin());
			for (; first != last && cur != end(); first++) {
				*cur = *first;
				cur++;
			}
			if (first == last)
				erase(cur, end());
			else
				insert(end(), cur, last);
				//insert(end(), first, last);
		}*/

		template< class ForwardIter >
		void	_M_realloc_insert( iterator pos, ForwardIter first, ForwardIter last ) {
			const size_type	len = size() + std::max(size(), std::distance(first, last));
			iterator		new_start(_M_allocate(len));
			iterator		new_finish(new_start);
			try {
				new_finish = std::uninitialized_copy(begin(), pos, new_start);
				new_finish = std::uninitialized_copy(first, last, new_finish);
				new_finish = std::uninitialized_copy(pos, end(), new_finish);
			} catch (...) {
				/*if (!new_finish)
					std::_Destroy(new_start + elems_before, new_start + elems_before + n);
				else
					std::_Destroy(new_start, new_finish);*/
				_M_destroy(new_start, new_finish);
				_M_deallocate(new_start.base(), len);
			}
			_M_destroy(begin(), end());
			_M_deallocate(_start, _end_of_storage - _start);
			_start = new_start.base();
			_finish = new_finish.base();
			_end_of_storage = new_start.base() + len;
		}

		/*void	_M_insert_aux( iterator pos, const_reference x ) {
			if (_finish != _end_of_storage) {
				_alloc.construct(_finish, *(_finish - 1));
				++_finish;
				std::copy_backward(pos, end() - 2, end() - 1);
				*pos = value_type(x);
			} else {
				const size_type	len = size() ? 2 * size() : 1;
				iterator		new_start(_M_allocate(len));
				iterator		new_finish(new_start);
				try {
					new_finish = std::uninitialized_copy(begin(), pos, new_start);
					_alloc.construct((new_finish++).base(), x);
					new_finish = std::uninitialized_copy(pos, end(), new_finish);
				} catch (...) {
					_M_destroy(new_start, new_finish);
					_M_deallocate(new_start.base(), len);
				}
				_M_destroy(begin(), end());
				_M_deallocate(_start, _end_of_storage - _start);
				_start = new_start.base();
				_finish = new_finish.base();
				_end_of_storage = new_start.base() + len;
			}
		}*/

		void	_M_fill_insert( iterator pos, size_type n, const_reference x ) {
			if (n != 0) {
				if (size_type(_end_of_storage - _finish) >= n) {
					const size_type	elems_after = end() - pos;
					iterator		old_finish(_finish);
					if (elems_after > n) {
						_finish = std::uninitialized_copy(_finish - n, _finish, _finish);
						std::copy_backward(pos, old_finish - n, old_finish);
						std::fill(pos, pos + n, x);
					} else {
						std::uninitialized_fill_n(_finish, n - elems_after, x);
						_finish += n - elems_after;
						//std::uninitialized_copy(pos, old_finish, _finish);
						//_finish += elems_after;
						_finish = std::uninitialized_copy(pos, old_finish, _finish);
						std::fill(pos, old_finish, x);
					}
				} else {
					reserve(size() + std::max(size(), n));
					_M_fill_insert(pos, n, x);
					//ft::vector<value_type>	tmp(n, x);
					//_M_realloc_insert(pos, tmp.begin(), tmp.end());
				}
			}
		}

		template< class InputIter >
		void	_M_range_insert( iterator pos, InputIter first, InputIter last, std::input_iterator_tag ) {
			for (; first != last; first++) {
				pos = insert(pos, *first);
				pos++;
			}
		}

		template< class ForwardIter >
		void	_M_range_insert( iterator pos, ForwardIter first, ForwardIter last, std::forward_iterator_tag ) {
			if (first != last) {
				size_type	n = std::distance(first, last);
				if (size_type(_end_of_storage - _finish) >= n) {
					const size_type	elems_after = end() - pos;
					iterator		old_finish(_finish);
					if (elems_after > n) {
						_finish = std::uninitialized_copy(_finish - n, _finish, _finish);
						std::copy_backward(pos, old_finish - n, old_finish);
						std::copy(first, last, pos);
					} else {
						ForwardIter	mid = first + elems_after;
						std::uninitialized_copy(mid, last, _finish);
						_finish += n - elems_after;
						std::uninitialized_copy(pos, old_finish, _finish);
						_finish += elems_after;
						std::copy(first, mid, pos);
					}
				} else {
					reserve(size() + std::max(size(), n));
					_M_range_insert(pos, first, last);
					//_M_realloc_insert(pos, first, last);
				}
			}
		}

		/*void	_M_erase_at_end( pointer __pos ) {
			if (size_type __n = _finish - __pos) {
				std::_Destroy(__pos, _finish);
				_finish = __pos;
			}
		}*/
		
		void	_M_destroy( iterator first, iterator last ) {
			for (; first != last; first++)
				_alloc.destroy(first.base());
		}

		iterator	_M_erase( iterator first, iterator last ) {
			const size_type	n = last - first;
			if (n > 0) {
				iterator	__i(std::copy(last, end(), first));
				//std::_Destroy(__i, end());
				_M_destroy(__i, end());
				_finish -= n;
			}
			return first;
		}

		void	_M_range_check( size_type __n ) const {
			if (__n >= size())
				throw std::out_of_range("vector::_M_range_check: __n (which is "
						+ __n + ") >= this->size() (which is " + size() + ")");
		}
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
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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
