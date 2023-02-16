//https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_tree.h
//https://www.boost.org/doc/libs/1_42_0/doc/html/boost/intrusive/rbtree.html
//https://gcc.gnu.org/onlinedocs/gcc-4.6.2/libstdc++/api/a01067_source.html

#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <memory>
# include <cstddef>
# include <iostream>
# include <sstream>
# include <algorithm>
# include "rb_tree_node.hpp"
# include "type_traits.hpp"
# include "rb_tree_iterator.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"
# include "utility.hpp"

namespace ft {
	//enum _Rb_tree_color { ft::_S_red = false, ft::_S_black = true };

	/*struct _Rb_tree_node_base {
		typedef _Rb_tree_node_base* 		_Base_ptr;
		typedef const _Rb_tree_node_base*	_Const_Base_ptr;

		_Rb_tree_color	_color;
		_Base_ptr		_parent;
		_Base_ptr		_left;
		_Base_ptr		_right;

		_Rb_tree_node_base() : _color(ft::_S_red), _parent(0), _left(this), _right(this) { }

		static _Base_ptr	_S_minimum(_Base_ptr x) {
			return (x->_left ? _S_minimum(x->_left) : x);
		}
		static _Base_ptr	_S_maximum(_Base_ptr x) {
			return (x->_right ? _S_maximum(x->_right) : x);
		}
	};*/

	/*template< typename T >
	struct _Rb_tree_node {
		typedef _Rb_tree_node<T>* 		node_pointer;
		typedef const _Rb_tree_node<T>*	const_node_pointer;

		_Rb_tree_color	_color;
		node_pointer	_parent;
		node_pointer	_left;
		node_pointer	_right;
		T				_value_field;

		_Rb_tree_node(const _Rb_tree_color& c,
			const_node_pointer& p,
			const_node_pointer& l,
			const_node_pointer& r)
		: _color(c), _parent(p), _left(l), _right(r) { }

		static node_pointer	_S_minimum(node_pointer x) {
			return (x->_left ? _S_minimum(x->_left) : x);
		}
		static node_pointer	_S_maximum(node_pointer x) {
			return (x->_right ? _S_maximum(x->_right) : x);
		}
	};*/

	template<
		class Key,
		class T,
		class KeyOfValue,
		class Compare,
		class Allocator = std::allocator<T>
	> class _Rb_tree {
	public:
		typedef Key					key_type;
		typedef T					value_type;
		typedef Allocator			allocator_type;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;

		typedef ft::_Rb_tree_node<T>	node_type;
		typedef node_type*				node_pointer;
		typedef const node_type*		const_node_pointer;

		typedef ft::_Rb_tree_iterator<value_type>		iterator;
		typedef ft::_Rb_tree_const_iterator<value_type>	const_iterator;
		//typedef ft::_Rb_tree_iterator<const value_type>	const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		typedef typename Allocator::template rebind<node_type>::other	_Node_allocator;
		//https://stackoverflow.com/questions/22384231/is-use-of-allocatorrebind-required-by-c-standard

		_Rb_tree()
		// : _header(ft::_S_red, 0, _M_end(), _M_end()), _node_count(0) { }
		 : _header(ft::_S_red, 0, _M_end(), 0), _node_count(0) { }

		_Rb_tree(const Compare& __comp, const allocator_type& __a = allocator_type())
		 : _key_comp(__comp), _alloc(__a), _header(ft::_S_red, 0, _M_end(), 0), _node_count(0) { }

		_Rb_tree(const _Rb_tree& other)
		 : _key_comp(other._key_comp), _alloc(other._alloc), _header(ft::_S_red, 0, _M_end(), 0), _node_count(0) {
			if (other._M_root())
	  			_M_root() = _M_copy(other);
		}

		~_Rb_tree() { /*printHelper(_M_root(), "", 1);*/ clear(); }

		_Rb_tree&	operator=(const _Rb_tree& other) {
			_key_comp = other._key_comp;
			_alloc = other._alloc;
			if (other._M_root()) {
				clear();
	  			_M_root() = _M_copy(other);
			}
			return *this;
		}

		allocator_type get_allocator() const { return _alloc; }

		iterator		begin() { return iterator(_M_leftmost()); }
		const_iterator	begin() const { return const_iterator(_M_leftmost()); }
		iterator		end() { return iterator(_M_end()); }
		const_iterator	end() const { return const_iterator(_M_end()); }
		reverse_iterator		rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator		rend() { return reverse_iterator(begin()); }
		const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

		bool		empty() const { return size() == 0; }
		size_type	size() const { return _node_count; }
		size_type	max_size() const { return _alloc.max_size(); }

		void	clear() { _M_erase(_M_begin()); _M_reset(); }

		ft::pair<iterator, bool>	insert_unique( const_reference value ) { //{ return _M_insert_unique_(value); }
			ft::pair<node_pointer, node_pointer>	res
					= _M_get_insert_unique_pos(KeyOfValue()(value));
			if (!res.second)
				return ft::pair<iterator, bool>(iterator(res.first), false);
			return ft::pair<iterator, bool>(_M_insert_(res.first, res.second, value), true);
		}
	
		iterator	insert_unique( iterator pos, const_reference value ) {
			ft::pair<node_pointer, node_pointer>	res
					= _M_get_insert_hint_unique_pos(pos, KeyOfValue()(value));
			if (!res.second)
				return iterator(res.first);
			return _M_insert_(res.first, res.second, value);
		}

		const_iterator	insert_unique( const_iterator pos, const_reference value ) {
			return insert_unique(pos._M_const_cast(), value);
		}

		template< class InputIt >
		void	insert_range_unique( InputIt first, InputIt last ) {
			while (first != last)
				insert_unique(end(), *first++);
				//_M_insert_unique_(end(), *first++);
		}
		
		/*iterator	insert_equal( const_reference value ) {
			ft::pair<node_pointer, node_pointer>	res
					= _M_get_insert_equal_pos(KeyOfValue()(value));
			return _M_insert_(res.first, res.second, value);
		}
		iterator	insert_equal( iterator pos, const_reference value ) {
			ft::pair<node_pointer, node_pointer>	res
					= _M_get_insert_hint_equal_pos(pos, KeyOfValue()(value));
			if (!res.second) {
				res = _M_get_insert_equal_lower_pos(KeyOfValue()(value));
				return _M_insert_lower(res.first, res.second, value);
			}
			return _M_insert_(res.first, res.second, value); 
		}
		template< class InputIt >
		void	insert_range_equal( InputIt first, InputIt last ) {
			while (first != last)
				insert_equal(end(), *first++);
		}*/

		void	erase( iterator pos ) {
			//_M_erase_(pos._M_node);

			node_pointer	del_node = pos._M_node, x = 0, y = del_node;
			ft::_Rb_tree_color	y_orig_color = y->_color;

			if (!del_node->_left) {
				x = del_node->_right;
				_M_transplant(del_node, del_node->_right);
			} else if (!del_node->_right) {
				x = del_node->_left;
				_M_transplant(del_node, del_node->_left);
			} else {
				y = ft::_Rb_tree_node<T>::_S_minimum(del_node->_right);
				y_orig_color = y->_color;
				x = y->_right;
				if (del_node == y->_parent) {
					if (x) x->_parent = y;
				} else {
					_M_transplant(y, y->_right);
					y->_right = del_node->_right;
					y->_right->_parent = y;
				}
				_M_transplant(del_node, y);
				y->_left = del_node->_left;
				y->_left->_parent = y;
				y->_color = del_node->_color;
			}
			if (y_orig_color == ft::_S_black)
				_M_erase_rebalance(x);
			//if (y == del_node) {
				// _M_transplant(del_node, x);

			if (del_node == _M_leftmost())
				_M_leftmost() = !del_node->_right ?
						del_node->_parent : ft::_Rb_tree_node<T>::_S_minimum(x);
			if (del_node == _M_rightmost())
				_M_rightmost() = !del_node->_left ?
						del_node->_parent : ft::_Rb_tree_node<T>::_S_maximum(x);
			//}

			_M_delete_node(del_node);
			_node_count--;

		}
		void	erase( const_iterator pos ) {
			erase(pos._M_const_cast());
		}

		void	erase( iterator first, iterator last ) {
			if (first == begin() && last == end())
				clear();
			else
				while (first != last)
					erase(first++);
		}

		void	erase( const_iterator first, const_iterator last ) {
			erase(first._M_const_cast(), last._M_const_cast());
		}

		size_type	erase(const key_type& key) {
			ft::pair<iterator, iterator>	p = equal_range(key);
			const size_type	old_size = size();
			erase(p.first, p.second);
			return old_size - size();
		}

		void	swap(_Rb_tree& other) {
			if (!_M_root()) {
				if (other._M_root())
					this->_M_move_data(other);
			} else if (!other._M_root()) {
				other._M_move_data(*this);
			} else {
				//std::swap(_header._color, other._header._color);
				std::swap(_M_root(), other._M_root());
				std::swap(_M_leftmost(), other._M_leftmost());
				std::swap(_M_rightmost(), other._M_rightmost());
				_M_root()->_parent = _M_end();
				other._M_root()->_parent = other._M_end();
				std::swap(_node_count, other._node_count);
			}
			// No need to swap header's color as it does not change.
			std::swap(_key_comp, other._key_comp);

			//std::__alloc_swap<_Node_allocator>::
			//_S_do_it(_M_get_Node_allocator(), __t._M_get_Node_allocator());
		}

		iterator		find( const key_type& key ) {
			iterator	j = lower_bound(key);
			return (j == end() || _key_comp(key, _S_key(j._M_node))) ? end() : j;
		}
		const_iterator	find( const key_type& key ) const {
			const_iterator	j = lower_bound(key);
			return (j == end() || _key_comp(key, _S_key(j._M_node))) ? end() : j;
		}

		size_type	count(const key_type& k) const {
			ft::pair<const_iterator, const_iterator>	p = equal_range(k);
			return std::distance(p.first, p.second);
		}

		// return >=key
		iterator		lower_bound(const key_type& key) { return _M_lower_bound(_M_begin(), _M_end(), key); }
		const_iterator	lower_bound(const key_type& key) const { return _M_lower_bound(_M_begin(), _M_end(), key); }
		// return >key
		iterator		upper_bound(const key_type& key) { return _M_upper_bound(_M_begin(), _M_end(), key); }
		const_iterator	upper_bound(const key_type& key) const { return _M_upper_bound(_M_begin(), _M_end(), key); }

		ft::pair<iterator, iterator>	equal_range(const key_type& key) {
			return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
		}

		ft::pair<const_iterator, const_iterator>	equal_range(const key_type& key) const {
			return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
		}

		Compare	key_comp() const { return _key_comp; }

		void printHelper(node_pointer root, std::string indent, bool last) {
			if (root) {
				std::cout << indent;
				if (last) {
					std::cout << "R----";
					indent += "   ";
				} else {
					std::cout << "L----";
					indent += "|  ";
				}

				std::string sColor = root->_color ? "RED" : "BLACK";
				std::cout << root->_value_field.first << "(" << sColor << ")" << std::endl;
				printHelper(root->_left, indent, false);
				printHelper(root->_right, indent, true);
			}
		}


	private:
		// attributes
		Compare			_key_comp;
		_Node_allocator	_alloc;
		node_type		_header;
		size_type		_node_count;

		static const key_type&	_S_key( const_node_pointer x )
		{ return KeyOfValue()(x->_value_field); } // with map, KeyOfValue is Select1st
												// so this function will return key

		node_pointer	_M_create_node( const_reference x ) {
			node_pointer	tmp = _alloc.allocate(1);
			try {
				get_allocator().construct(&tmp->_value_field, x); // with map, _valur_field is ft::pair
			} catch (...) {
				_alloc.deallocate(tmp, 1);
			}
			return tmp;
		}

		void	_M_delete_node( node_pointer p ) {
			get_allocator().destroy(&p->_value_field);
			_alloc.deallocate(p, 1);
		}

		node_pointer	_M_clone_node( const_node_pointer p ) {
			node_pointer tmp = _M_create_node(p->_value_field);
			tmp->_color = p->_color;
			tmp->_parent = 0;
			tmp->_left = 0;
			tmp->_right = 0;
			return tmp;
		}

		void	_M_reset() { // reset red black tree
			_M_root() = 0;
			_M_leftmost() = _M_end(); // important when add first node
			_M_rightmost() = 0; //_M_end();
			_node_count = 0;
		}

		void	_M_move_data( _Rb_tree& from ) {
			_header._color = from._header._color;
			_M_root() = from._M_root();
			_M_leftmost() = from._M_leftmost();
			_M_rightmost() = from._M_rightmost();
			_M_root()->_parent = _M_end(); // ???
			_node_count = from._node_count;
			from._M_reset();
		}

		ft::pair<node_pointer, node_pointer>	_M_get_insert_unique_pos(const key_type& key) {
			node_pointer	x = _M_begin(); // root
			node_pointer	y = _M_end();
			bool			__comp = true;
			while (x) {
				y = x;
				__comp = _key_comp(key, _S_key(x));
				x = __comp ? x->_left : x->_right;
			}
			iterator	__j = iterator(y);
			if (__comp) {
				if (__j-- == begin()) // left most
					return ft::pair<node_pointer, node_pointer>(x, y);
			}
			if (_key_comp(_S_key(__j._M_node), key))
				return ft::pair<node_pointer, node_pointer>(x, y);
			return ft::pair<node_pointer, node_pointer>(__j._M_node, 0);
		}

		ft::pair<node_pointer, node_pointer>	_M_get_insert_hint_unique_pos(const_iterator position, const key_type& key) {
			typedef ft::pair<node_pointer, node_pointer>	_Res;
			iterator	pos = position._M_const_cast();
			if (pos._M_node == _M_end()) {
				if (size() > 0 && _key_comp(_S_key(_M_rightmost()), key))
					return _Res(0, _M_rightmost());
			} else if (_key_comp(key, _S_key(pos._M_node))) {
				if (pos._M_node == _M_leftmost())
					return _Res(_M_leftmost(), _M_leftmost());
				iterator	before = pos;
				if (_key_comp(_S_key((--before)._M_node), key)) { // bef < k < pos
					if (!before._M_node->_right)
						return _Res(0, before._M_node);
					return _Res(pos._M_node, pos._M_node);
				}
			} else if (_key_comp(_S_key(pos._M_node), key)) {
				if (pos._M_node == _M_rightmost())
					return _Res(0, _M_rightmost());
				iterator	after = pos;
				if (_key_comp(key, _S_key((++after)._M_node))) { // pos < k < aft
					if (!pos._M_node->_right)
						return _Res(0, pos._M_node);
					return _Res(after._M_node, after._M_node);
				}
			} else
				return _Res(pos._M_node, 0);
			return _M_get_insert_unique_pos(key);
		}

		ft::pair<node_pointer, node_pointer>	_M_get_insert_equal_pos(const key_type& key) {
			node_pointer	x = _M_begin();
			node_pointer	y = _M_end();
			while (x) {
				y = x;
				x = _key_comp(key, _S_key(x)) ? x->_left : x->_right;
			}
			return ft::pair<node_pointer, node_pointer>(x, y);
		}

		ft::pair<node_pointer, node_pointer>	_M_get_insert_hint_equal_pos(const_iterator __position, const key_type& key) {
			typedef ft::pair<node_pointer, node_pointer>	_Res;
			if (__position._M_node == _M_end()) {
				if (size() > 0 && !_key_comp(key, _S_key(_M_rightmost())))
					return _Res(0, _M_rightmost());
			} else if (!_key_comp(_S_key(__position._M_node), key)) {
				if (__position._M_node == _M_leftmost())
					return _Res(_M_leftmost(), _M_leftmost());
				const_iterator	__before = __position - 1;
				if (!_key_comp(key, _S_key(__before._M_node))) { // bef < k < pos
					if (!__before._M_node->_right)
						return _Res(0, __before._M_node);
					return _Res(__position._M_node, __position._M_node);
				}
			} else {
				if (__position._M_node == _M_rightmost())
					return _Res(0, _M_rightmost());
				const_iterator	__after = __position + 1;
				if (!_key_comp(_S_key(__after._M_node), key)) { // pos < k < aft
					if (!__position._M_node->_right)
						return _Res(0, __position._M_node);
					return _Res(__after._M_node, __after._M_node);
				} else
					return _Res(0, 0);
			}
			return _M_get_insert_equal_pos(key);
		}

		ft::pair<node_pointer, node_pointer>	_M_get_insert_equal_lower_pos(const key_type& key) {
			node_pointer	x = _M_begin();
			node_pointer	y = _M_end();
			while (x) {
				y = x;
				x = !_key_comp(_S_key(x), key) ? x->_left : x->_right;
			}
			return ft::pair<node_pointer, node_pointer>(x, y);
		}

		iterator	_M_insert_( const_node_pointer x, node_pointer parent, const_reference val ) {
			node_pointer	node = _M_create_node(val);
			_node_count++;

			node->_color = ft::_S_red;
			node->_parent = parent;
			node->_left = 0;
			node->_right = 0;

			// if (x OR nullTree OR nodeKey < parentKey)
			if (x || parent == _M_end() || _key_comp(_S_key(node), _S_key(parent))) {
				parent->_left = node; // when parent == _M_end(), _M_leftmost() = node;
				if (parent == _M_end()) {
					_M_root() = node;
					_M_rightmost() = node;
				} else if (parent == _M_leftmost())
					_M_leftmost() = node;
			} else {
				parent->_right = node;
				if (parent == _M_rightmost())
					_M_rightmost() = node;
			}
			_M_insert_rebalance(node);
			return iterator(node);
		}

		iterator	_M_insert_lower( const_node_pointer x, node_pointer parent, const_reference val ) {
			node_pointer	node = _M_create_node(val);
			_node_count++;
	
			node->_color = ft::_S_red;
			node->_parent = parent;
			node->_left = 0;
			node->_right = 0;

			// if (x OR nullTree OR nodeKey <= parentKey)
			if (x || parent == _M_end() || !_key_comp(_S_key(parent), _S_key(node))) {
				parent->_left = node;
				if (parent == _M_end()) {
					_M_root() = node;
					_M_rightmost() = node;
				} else if (parent == _M_leftmost())
					_M_leftmost() = node;
			} else {
				parent->_right = node;
				if (parent == _M_rightmost())
					_M_rightmost() = node;
			}
			_M_insert_rebalance(node);
			return iterator(node);
		}

		/*void	_M_erase_( node_pointer z ) {
			node_pointer	x = 0, y = z;
			bool	y_orig_color = y->_color;

			if (!z->_left) {
				x = z->_right
			} else if (!z->_right) {
				x = z->_left;
			} else {
				y = _Rb_tree_node::_S_minimum(z->_right);
				y_orig_color = y->_color;
				x = y->_right;
				if (z == y->_parent) {
					x->_parent = y;
				} else {
					_M_transplant(y, x);
					y->_right = z->_right;
					y->_right->_parent = y;
				}
				_M_transplant(z, y);
				y->_left = z->_left;
				y->_left->_parent = y;
				y->_color = z->_color;
			}
			if (y == z) {
				_M_transplant(z, x);
				if (_M_leftmost() == z)
					_M_leftmost() = !z->_right ? z->_parent : _Rb_tree_node::_S_minimum(x);
				if (_M_rightmost() == z)
					_M_rightmost() = !z->_left ? z->_parent : _Rb_tree_node::_S_maximum(x);
			}

			//delete z;
			_M_delete_node(z);
			_node_count--;

			if (y_orig_color == ft::_S_black)
				_M_erase_rebalance(x);
		}*/

		void	_left_rotate( node_pointer less ) {
			if (less) {
				node_pointer	greater = less->_right;
				if (greater) {
					less->_right = greater->_left;	// move 'greater' left child to become 'less' right child
					if (greater->_left)
						greater->_left->_parent = less;
					_M_transplant(less, greater);	// move 'greater' to 'less' postion
					greater->_left = less;			// move 'less' to become 'greater' left child
					less->_parent = greater;
				}
			}
		}

		void	_right_rotate( node_pointer greater ) {
			if (greater) {
				node_pointer	less = greater->_left;
				if (less) {
					greater->_left = less->_right;
					if (less->_right)
						less->_right->_parent = greater;
					_M_transplant(greater, less);
					less->_right = greater;
					greater->_parent = less;
				}
			}
		}

		void	_M_transplant( node_pointer parent, node_pointer child ) {
			if (parent == _M_root())
				_M_root() = child;
			else if (parent == parent->_parent->_left)
				parent->_parent->_left = child;
			else
				parent->_parent->_right = child;
			if (child)
				child->_parent = parent->_parent;
		}

		node_pointer	_rebalance_left_parent( node_pointer node ) {
			node_pointer	uncle = node->_parent->_parent->_right;

			bool	is_uncle_red = false;
			if (uncle)
				is_uncle_red = uncle->_color == ft::_S_red;

			if (is_uncle_red) {
				uncle->_color = ft::_S_black;
				node->_parent->_color = ft::_S_black;
				node->_parent->_parent->_color = ft::_S_red;
				node = node->_parent->_parent;
			} else {
				if (node == node->_parent->_right) {
					node = node->_parent;
					_left_rotate(node);
				}
				node->_parent->_color = ft::_S_black;
				node->_parent->_parent->_color = ft::_S_red;
				_right_rotate(node->_parent->_parent);
			}
			return node;
		}

		node_pointer	_rebalance_right_parent( node_pointer node ) {
			node_pointer	uncle = node->_parent->_parent->_left;

			bool	is_uncle_red = false;
			if (uncle)
				is_uncle_red = uncle->_color == ft::_S_red;

			if (is_uncle_red) {
				uncle->_color = ft::_S_black;
				node->_parent->_color = ft::_S_black;
				node->_parent->_parent->_color = ft::_S_red;
				node = node->_parent->_parent;
			} else {
				if (node == node->_parent->_left) {
					node = node->_parent;
					_right_rotate(node);
				}
				node->_parent->_color = ft::_S_black;
				node->_parent->_parent->_color = ft::_S_red;
				_left_rotate(node->_parent->_parent);
			}
			return node;
		}

		void	_M_insert_rebalance( node_pointer node ) {
			if (!node)
				return ;
			if (node == _M_root() || node->_parent->_color == ft::_S_black) {
				_M_root()->_color = ft::_S_black;
				return ;
			}
			if (node->_parent == node->_parent->_parent->_left)
				node = _rebalance_left_parent(node);
			else
				node = _rebalance_right_parent(node);
			_M_insert_rebalance(node);
		}

		node_pointer	_rebalance_left_child( node_pointer node ) {
			node_pointer	sibling = node->_parent->_right;
			if (!sibling)
				return 0;

			if (sibling->_color == ft::_S_red) {
				sibling->_color = ft::_S_black;
				node->_parent->_color = ft::_S_red;
				_left_rotate(node->_parent);
				sibling = node->_parent->_right;
			}

			bool	cond = false;
			if (sibling->_left && sibling->_right)
				cond = sibling->_left->_color == ft::_S_black
						&& sibling->_right->_color == ft::_S_black;

			if (cond) {
				sibling->_color = ft::_S_red;
				node = node->_parent;
			} else {
				if (sibling->_right) {
					if (sibling->_right->_color == ft::_S_black) {
						sibling->_left->_color = ft::_S_black;
						sibling->_color = ft::_S_red;
						_right_rotate(sibling);
						sibling = node->_parent->_right;
					}
				}
				sibling->_color = node->_parent->_color;
				node->_parent->_color = ft::_S_black;
				if (sibling->_right)
					sibling->_right->_color = ft::_S_black;
				_left_rotate(node->_parent);
				node = _M_root();
			}
			return node;
		}

		node_pointer	_rebalance_right_child( node_pointer node ) {
			node_pointer	sibling = node->_parent->_left;
			if (!sibling)
				return 0;
	
			if (sibling->_color == ft::_S_red) {
				sibling->_color = ft::_S_black;
				node->_parent->_color = ft::_S_red;
				_right_rotate(node->_parent);
				sibling = node->_parent->_left;
			}
			
			bool	cond = false;
			if (sibling->_left && sibling->_right)
				cond = sibling->_left->_color == ft::_S_black
						&& sibling->_right->_color == ft::_S_black;

			if (cond) {
				sibling->_color = ft::_S_red;
				node = node->_parent;
			} else {
				if (sibling->_left) {
					if (sibling->_left->_color == ft::_S_black) {
						sibling->_right->_color = ft::_S_black;
						sibling->_color = ft::_S_red;
						_left_rotate(sibling);
						sibling = node->_parent->_left;
					}
				}
				sibling->_color = node->_parent->_color;
				node->_parent->_color = ft::_S_black;
				if (sibling->_left)
					sibling->_left->_color = ft::_S_black;
				_right_rotate(node->_parent);
				node = _M_root();
			}
			return node;
		}

		void	_M_erase_rebalance( node_pointer node ) {
			if (!node)
				return ;
			if (node == _M_root() || node->_color == ft::_S_red) {
				node->_color = ft::_S_black;
				return ;
			}
			if (node == node->_parent->_left)
				node = _rebalance_left_child(node);
			else
				node = _rebalance_right_child(node);
			_M_erase_rebalance(node);
		}

		/*node_pointer	_M_copy(const_node_pointer x, node_pointer __p) {
			node_pointer	__top = _M_clone_node(x);
			__top->_parent = __p;
			try {
				if (x->_right)
					__top->_right = _M_copy(x->_right, __top);
				__p = __top;
				x = x->_left;
				while (x) {
					node_pointer	y = _M_clone_node(x);
					__p->_left = y;
					y->_parent = __p;
					if (x->_right)
						y->_right = _M_copy(x->_right, y);
					__p = y;
					x = x->_left;
				}
			} catch (...) {
				_M_erase(__top);
			}
			return __top;
		}*/

		node_pointer	_M_copy(const _Rb_tree& x) {
			_M_root() = _M_copy(x._M_begin(), _M_end());
			_M_leftmost() = ft::_Rb_tree_node<T>::_S_minimum(_M_root());
			_M_rightmost() = ft::_Rb_tree_node<T>::_S_maximum(_M_root());
			_node_count = x._node_count;
			return _M_root();
		}

		node_pointer	_M_copy(const_node_pointer x, node_pointer __p) {
			if (!x)
				return 0;
			node_pointer	__top = _M_clone_node(x);
			__top->_parent = __p;
			__top->_left = _M_copy(x->_left, __top);
			__top->_right = _M_copy(x->_right, __top);
			return __top;
		}

		void    _M_erase(node_pointer x) {
			if (!x)
				return ;
			_M_erase(x->_left);
			_M_erase(x->_right);
			_M_delete_node(x);
		}

		iterator		_M_lower_bound( node_pointer first,
				node_pointer last, const key_type& key ) {
			if (!first)
				return iterator(last);
			if (!_key_comp(_S_key(first), key))
				return _M_lower_bound(first->_left, first, key);
			return _M_lower_bound(first->_right, last, key);
		}

		const_iterator	_M_lower_bound( const_node_pointer first,
				const_node_pointer last, const key_type& key ) const {
			if (!first)
				return const_iterator(last);
			if (!_key_comp(_S_key(first), key))
				return _M_lower_bound(first->_left, first, key);
			return _M_lower_bound(first->_right, last, key);
			//return const_iterator(_M_lower_bound(first, last, key));
		}

		iterator		_M_upper_bound( node_pointer first,
				node_pointer last, const key_type& key ) {
			if (!first)
				return iterator(last);
			if (_key_comp(key, _S_key(first)))
				return _M_upper_bound(first->_left, first, key);
			return _M_upper_bound(first->_right, last, key);
		}

		const_iterator	_M_upper_bound( const_node_pointer first,
				const_node_pointer last, const key_type& key ) const {
			if (!first)
				return const_iterator(last);
			if (_key_comp(key, _S_key(first)))
				return _M_upper_bound(first->_left, first, key);
			return _M_upper_bound(first->_right, last, key);
		}

		node_pointer&		_M_root() { return _header._parent; }
		const_node_pointer	_M_root() const { return _header._parent; }

		node_pointer&		_M_leftmost() { return _header._left; }
		const_node_pointer	_M_leftmost() const { return _header._left; }
		node_pointer&		_M_rightmost() { return _header._right; }
		const_node_pointer	_M_rightmost() const { return _header._right; }

		node_pointer		_M_begin() { return _header._parent; }
		const_node_pointer	_M_begin() const { return _header._parent; }
		node_pointer		_M_end() { return &_header; }
		const_node_pointer	_M_end() const { return &_header; }
	};

	template< class Key, class T, class KeyOfValue, class Compare, class Alloc >
	bool operator==(const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& lhs,
			const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& rhs) {
		return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template< class Key, class T, class KeyOfValue, class Compare, class Alloc >
	bool operator!=(const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& lhs,
			const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template< class Key, class T, class KeyOfValue, class Compare, class Alloc >
	bool operator<(const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& lhs,
			const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template< class Key, class T, class KeyOfValue, class Compare, class Alloc >
	bool operator<=(const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& lhs,
			const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template< class Key, class T, class KeyOfValue, class Compare, class Alloc >
	bool operator>(const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& lhs,
			const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& rhs) {
		return rhs < lhs;
	}

	template< class Key, class T, class KeyOfValue, class Compare, class Alloc >
	bool operator>=(const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& lhs,
			const _Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template< class Key, class T, class KeyOfValue, class Compare, class Alloc >
	void	swap(_Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& lhs,
			_Rb_tree<Key, T, KeyOfValue, Compare, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}

#endif
