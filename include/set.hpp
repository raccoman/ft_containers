#ifndef FT_CONTAINERS_SET_HPP
#define FT_CONTAINERS_SET_HPP

#include <ft_algorithm.hpp>
#include <ft_iterator.hpp>
#include <rbtree.hpp>
#include <vector.hpp>

namespace ft {



	template<
			class Key,
			class Compare = ft::less<Key>,
			class Allocator = std::allocator<Key>
	> class set {
	public:
		typedef Key														key_type;
		typedef Key														value_type;
		typedef std::size_t												size_type;
		typedef std::ptrdiff_t											difference_type;
		typedef Compare													key_compare;
		typedef Compare													value_compare;
		typedef Allocator												allocator_type;
		typedef value_type&												reference;
		typedef const value_type&										const_reference;
		typedef typename allocator_type::pointer						pointer;
		typedef typename allocator_type::const_pointer					const_pointer;
		typedef rbtree_iterator<const value_type, value_compare>		iterator;
		typedef rbtree_iterator<const value_type, value_compare>		const_iterator;
		typedef ft::reverse_iterator<iterator>							reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;

		typedef RBTree<value_type, value_compare>						tree_type;
		typedef RBNode<value_type>										node_type;
		typedef node_type*												node_pointer;

	protected:
		tree_type*					_tree;
		std::allocator<tree_type>	_tree_allocator;
		key_compare					_comparator;
		allocator_type				_allocator;
		size_type					_size;

	public:

		tree_type* tree() { return _tree; }

		set() : _allocator(Allocator()), _size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
		}

		explicit set(const Compare &comp, const Allocator &alloc = Allocator()) : _comparator(comp), _allocator(alloc),
																				  _size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
		}

		template<class U>
		set(U first, U last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) : _comparator(comp),
				_allocator(alloc),
				_size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
			while (first != last)
				insert(*first++);
		}

		set(const set &other) : _comparator(other.key_comp()), _allocator(other.get_allocator()), _size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
			const_iterator it = other.begin();
			while (it != other.end())
				insert(*it++);
		}

		~set() {
			clear();
		}

		set& operator=(const set& other) {
			_tree_allocator.destroy(_tree);
			_tree_allocator.deallocate(_tree, 1);

			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
			_size = 0;
			insert(other.begin(), other.end());
			return *this;
		}

		allocator_type get_allocator() const {
			return _allocator;
		}

		iterator begin() { return iterator(_tree->begin()); }
		iterator end() { return iterator(_tree->end()); }

		const_iterator begin() const { return const_iterator(_tree->begin()); }
		const_iterator end() const { return const_iterator(_tree->end()); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }

		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		void clear() {
			if (_size) {
				_size = 0;
				_tree->clear();
			}
		}

		bool empty() const { return !_size; }

		size_type size() const { return _size; }

		size_type max_size() const { return _tree->max_size(); }

		ft::pair<iterator, bool> insert(const value_type &value) {
			node_pointer node = _tree->find(value);
			if (node != end().base())
				return ft::make_pair(iterator(node), false);
			node = _tree->insert(value);
			_size++;
			return ft::make_pair(iterator(node), true);
		}

		iterator insert(iterator hint, const value_type &value) {
			(void) hint;
			return insert(value).first;
		}

		template<class U>
		void insert(U first, U last, typename enable_if<!ft::is_integral<U>::value, U>::type * = 0) {
			while (first != last)
				insert(*first++);
		}

		ft::pair<iterator, iterator> equal_range(const Key &key) {
			return ft::make_pair(iterator(lower_bound(key)), iterator(upper_bound(key)));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const Key &key) const {
			return ft::make_pair(const_iterator(lower_bound(key)), const_iterator(upper_bound(key)));
		}

		iterator lower_bound(const Key &key) {
			node_type *n;
			node_type *res;
			res = end().base();
			if (!_size)
				return iterator(res);
			n = _tree->root();
			while (1) {
				if ((*n).data == key) {
					res = n;
					break;
				} else if (!_comparator((*n).data, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data, key)) {
					if (!n->right)
						break;
					n = n->right;
				}
			}
			return iterator(res);
		}

		const_iterator lower_bound(const Key &key) const {
			node_type *n;
			const node_type *res;
			res = end().base();
			if (!_size)
				return const_iterator(res);
			n = _tree->root();
			while (1) {
				if ((*n).data == key) {
					res = n;
					break;
				} else if (!_comparator((*n).data, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data, key)) {
					if (!n->right)
						break;
					n = n->right;
				}
			}
			return const_iterator(res);
		}

		iterator upper_bound(const Key &key) {
			node_type *n;
			node_type *res;
			res = end().base();
			if (!_size)
				return iterator(res);
			n = _tree->root();
			while (1) {
				if ((*n).data == key) {
					if (!n->right)
						break;
					n = n->right;
				} else if (!_comparator((*n).data, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data, key)) {
					if (!n->right)
						break;
					n = n->right;
				}
			}
			return iterator(res);
		}

		const_iterator upper_bound(const Key &key) const {
			node_type *n;
			const node_type *res;
			res = end().base();
			if (!_size)
				return const_iterator(res);
			n = _tree->root();
			while (1) {
				if ((*n).data == key) {
					if (!n->right)
						break;
					n = n->right;
				} else if (!_comparator((*n).data, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data, key)) {
					if (!n->right)
						break;
					n = n->right;
				}
			}
			return const_iterator(res);
		}

		iterator find(const Key &key) {
			return iterator(_tree->find(key));
		}

		const_iterator find(const Key &key) const {
			return const_iterator(_tree->find(key));
		}

		void erase(iterator pos) {
			_tree->remove(pos.base());
			_size--;
		}

		size_type erase(const key_type &k) {
			iterator found = find(k);
			if (found != end()) {
				_tree->remove(found.base());
				_size--;
				return 1;
			}
			return 0;
		}

		void erase(iterator first, iterator last) {
			ft::vector<Key> keys;
			for (iterator it = first; it != last; it++)
				keys.push_back(*it);
			for (typename ft::vector<Key>::iterator it = keys.begin(); it != keys.end(); it++)
				erase(*it);
		}

		size_type count(const Key &key) const {
			size_type s = 0;
			for (const_iterator it = begin(); it != end(); it++)
				if (*it == key)
					s++;
			return s;
		}

		void swap(set& other) {
			set temp = *this;
			*this = other;
			other = temp;
		}

		key_compare key_comp() const { return Compare(); }
		value_compare value_comp() const { return value_compare(); }
	};

	template<class Key, class Compare, class Alloc>
	bool operator==(const ft::set<Key, Compare, Alloc> &lhs, const ft::set<Key, Compare, Alloc> &rhs) {
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template<class Key, class Compare, class Alloc>
	bool operator!=(const ft::set<Key, Compare, Alloc> &lhs, const ft::set<Key, Compare, Alloc> &rhs) {
		return (!(lhs == rhs));
	}

	template<class Key, class Compare, class Alloc>
	bool operator<(const ft::set<Key, Compare, Alloc> &lhs, const ft::set<Key, Compare, Alloc> &rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class Key, class Compare, class Alloc>
	bool operator<=(const ft::set<Key, Compare, Alloc> &lhs, const ft::set<Key, Compare, Alloc> &rhs) {
		return (!(rhs < lhs));
	}

	template<class Key, class Compare, class Alloc>
	bool operator>(const ft::set<Key, Compare, Alloc> &lhs, const ft::set<Key, Compare, Alloc> &rhs) {
		return (rhs < lhs);
	}

	template<class Key, class Compare, class Alloc>
	bool operator>=(const ft::set<Key, Compare, Alloc> &lhs, const ft::set<Key, Compare, Alloc> &rhs) {
		return (!(lhs < rhs));
	}
	
}

#endif
