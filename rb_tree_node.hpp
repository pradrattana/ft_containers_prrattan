//https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_tree.h
//https://www.boost.org/doc/libs/1_42_0/doc/html/boost/intrusive/rbtree.html
//https://gcc.gnu.org/onlinedocs/gcc-4.6.2/libstdc++/api/a01067_source.html

#ifndef RB_TREE_NODE_HPP
# define RB_TREE_NODE_HPP

# include "utility.hpp"

namespace ft {
	enum _Rb_tree_color { _S_red = false, _S_black = true };

	template< typename T >
	class _Rb_tree_node {
	public:
		typedef _Rb_tree_node<T>* 		node_pointer;
		typedef const _Rb_tree_node<T>*	const_node_pointer;

		ft::_Rb_tree_color	_color;
		node_pointer		_parent;
		node_pointer		_left;
		node_pointer		_right;
		T					_value_field;

		_Rb_tree_node( const ft::_Rb_tree_color& c,
			node_pointer p,
			node_pointer l,
			node_pointer r )
		: _color(c), _parent(p), _left(l), _right(r) { }

		static node_pointer	_S_minimum( node_pointer x ) {
			return (x->_left ? _S_minimum(x->_left) : x);
		}

		static node_pointer	_S_maximum( node_pointer x ) {
			return (x->_right ? _S_maximum(x->_right) : x);
		}
	};
}

#endif
