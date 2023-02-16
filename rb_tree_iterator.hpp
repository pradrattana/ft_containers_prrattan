//https://gcc.gnu.org/onlinedocs/gcc-4.6.2/libstdc++/api/a01067_source.html

#ifndef RB_TREE_ITERATOR_HPP
# define RB_TREE_ITERATOR_HPP

# include <cstddef>
# include "iterator_traits.hpp"
# include "rb_tree_node.hpp"

namespace ft {
	template< typename T >
	ft::_Rb_tree_node<T>*	_Rb_tree_increment( ft::_Rb_tree_node<T>* x ) {
		if (x->_right) {
			x = ft::_Rb_tree_node<T>::_S_minimum(x->_right);
		} else {
			ft::_Rb_tree_node<T>*	parent = x->_parent;
			if (parent) {
				while (x == parent->_right) {
					x = parent;
					parent = x->_parent;
				}
			}
			if (x->_right != parent)
				x = parent;
		}
		return x;
	}
	
	template< typename T >
	ft::_Rb_tree_node<T>*	_Rb_tree_decrement( ft::_Rb_tree_node<T>* x ) {
		ft::_Rb_tree_node<T>*	parent = x->_parent;
		if (parent) {
			if (x->_color == ft::_S_red && parent->_parent == x)
				x = x->_right;
			else if (x->_left)
				x = ft::_Rb_tree_node<T>::_S_maximum(x->_left);
			else {
				while (x == parent->_left) {
					x = parent;
					parent = x->_parent;
				}
				x = parent;
			}
		} else
			x = x->_left ? ft::_Rb_tree_node<T>::_S_maximum(x->_left) : parent;
		return x;
	}


	template< typename T >
	struct _Rb_tree_iterator {
		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::ptrdiff_t					difference_type;
		typedef value_type&						reference;
		typedef value_type*						pointer;
		typedef ft::_Rb_tree_node<T>*			node_pointer;

		_Rb_tree_iterator() : _M_node() { }

		explicit
		_Rb_tree_iterator( node_pointer x ) : _M_node(x) { }

		reference	operator*() const { return _M_node->_value_field; }
		pointer		operator->() const { return &(operator*()); }

		_Rb_tree_iterator&	operator++() { _M_node = ft::_Rb_tree_increment(_M_node); return *this; }
		_Rb_tree_iterator&	operator--() { _M_node = ft::_Rb_tree_decrement(_M_node); return *this; }
		_Rb_tree_iterator	operator++( int ) { _Rb_tree_iterator tmp = *this; ++(*this); return tmp; }
		_Rb_tree_iterator	operator--( int ) { _Rb_tree_iterator tmp = *this; --(*this); return tmp; }

		_Rb_tree_iterator		operator-( difference_type n ) const {
			node_pointer	tmp = _M_node;
			for (difference_type i = 0; i < n; ++i)
				tmp = ft::_Rb_tree_decrement(tmp);
			return _Rb_tree_iterator(tmp);
		}

		bool	operator==( const _Rb_tree_iterator& other ) const { return _M_node == other._M_node; }
		bool	operator!=( const _Rb_tree_iterator& other ) const { return !(operator==(other)); }

		node_pointer	_M_node;
	};

	template< typename T >
	struct _Rb_tree_const_iterator {
		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::ptrdiff_t					difference_type;
		typedef const value_type&				reference;
		typedef const value_type*				pointer;
		typedef const ft::_Rb_tree_node<T>*		node_pointer;
		typedef ft::_Rb_tree_iterator<T>		iterator;

		_Rb_tree_const_iterator() : _M_node() { }

		explicit
		_Rb_tree_const_iterator( node_pointer x ) : _M_node(x) { }

		_Rb_tree_const_iterator( const iterator& it ) : _M_node(it._M_node) { }

		iterator	_M_const_cast() const {
			return iterator(const_cast<typename iterator::node_pointer>(_M_node));
		}

		reference	operator*() const { return _M_node->_value_field; }
		pointer		operator->() const { return &(operator*()); }

		_Rb_tree_const_iterator&	operator++() { _M_node = ft::_Rb_tree_increment(const_cast<typename iterator::node_pointer>(_M_node)); return *this; }
		_Rb_tree_const_iterator&	operator--() { _M_node = ft::_Rb_tree_decrement(const_cast<typename iterator::node_pointer>(_M_node)); return *this; }
		_Rb_tree_const_iterator		operator++(int) { _Rb_tree_const_iterator tmp = *this; ++(*this); return tmp; }
		_Rb_tree_const_iterator		operator--(int) { _Rb_tree_const_iterator tmp = *this; --(*this); return tmp; }

		_Rb_tree_const_iterator		operator-( difference_type n ) const {
			typename iterator::node_pointer	tmp = const_cast<typename iterator::node_pointer>(_M_node);
			for (difference_type i = 0; i < n; ++i)
				tmp = ft::_Rb_tree_decrement(tmp);
			return _Rb_tree_const_iterator(tmp);
		}

		bool	operator==(const _Rb_tree_const_iterator& other) const { return _M_node == other._M_node; }
		bool	operator!=(const _Rb_tree_const_iterator& other) const { return !(operator==(other)); }

		node_pointer	_M_node;
	};

	template< class T >
	bool operator==( const ft::_Rb_tree_iterator<T>& lhs, const ft::_Rb_tree_const_iterator<T>& rhs ) {
		return lhs._M_node == rhs._M_node;
	}

	template< class T >
	bool operator!=( const ft::_Rb_tree_iterator<T>& lhs, const ft::_Rb_tree_const_iterator<T>& rhs ) {
		return !(lhs == rhs);
	}
}

#endif
