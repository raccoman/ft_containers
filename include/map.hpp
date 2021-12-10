#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP

#include <ft_algorithm.hpp>
#include <rbtree.hpp>
#include <vector.hpp>

namespace ft {

	template<
			class Key,
			class T,
			class Compare = ft::less<Key>,
			class Allocator = std::allocator<ft::pair<const Key, T> >
	>
	class map {

	public:
		typedef Key key_type;
		typedef T mapped_type;
		typedef ft::pair<const Key, T> value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef Compare key_compare;
		typedef Allocator allocator_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;

		class value_compare : public std::binary_function<value_type, value_type, bool> {
		protected:
			key_compare comp;
		public:
			value_compare() : comp(key_compare()) {}

			value_compare(key_compare c) : comp(c) {}

			bool operator()(const value_type &lhs, const value_type &rhs) const {
				return comp(lhs.first, rhs.first);
			}

		};

		typedef rbtree_iterator<value_type, value_compare> iterator;
		typedef rbtree_iterator<const value_type, value_compare, true> const_iterator;
		/*typedef ft::reverse_iterator<iterator>						reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;*/

		typedef RBTree<value_type, value_compare> tree_type;
		typedef RBNode<value_type> node_type;
		typedef node_type *node_pointer;

	protected:
		tree_type *_tree;
		std::allocator<tree_type> _tree_allocator;
		key_compare _comparator;
		allocator_type _allocator;
		size_type _size;

	public:

		tree_type *tree() { return _tree; }

		map() : _allocator(Allocator()), _size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
		}

		explicit map(const Compare &comp, const Allocator &alloc = Allocator()) : _comparator(comp), _allocator(alloc),
																				  _size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
		}

		template<class U>
		map(U first, U last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) : _comparator(comp),
																									  _allocator(alloc),
																									  _size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
			while (first != last)
				insert(*first++);
		}

		map(const map &other) : _comparator(other.key_comp()), _allocator(other.get_allocator()), _size(0) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
			const_iterator it = other.begin();
			while (it != other.end())
				insert(*it++);
		}

		~map() {
			clear();
		}

		map &operator=(const map &other) {
			_tree = _tree_allocator.allocate(1);
			_tree_allocator.construct(_tree);
			_comparator = other._comparator;
			_allocator = other._allocator;
			_size = 0;
			const_iterator it = other.begin();
			while (it != other.end())
				insert(*it++);
			return *this;
		}

		allocator_type get_allocator() const {
			return _allocator;
		}

		T &at(const Key &key) {
			node_pointer res = _tree->find(key);
			if (res == nullptr) return res;
			return res->data.second;
		}

		const T &at(const Key &key) const {
			node_pointer res = _tree->find(key);
			if (res == nullptr) return res;
			return res->data.second;
		}

		T &operator[](const Key &key) {
			value_type pair = ft::make_pair(key, T());
			node_pointer res = _tree->find(pair);
			if (res == end().base()) {
				return insert(pair).first->second;
			}
			return res->data.second;
		}

		iterator begin() { return iterator(_tree->begin()); }

		iterator end() { return iterator(_tree->end()); }

		const_iterator begin() const { return const_iterator(_tree->begin()); }

		const_iterator end() const { return const_iterator(_tree->end()); }

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
				if ((*n).data.first == key) {
					res = n;
					break;
				} else if (!_comparator((*n).data.first, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data.first, key)) {
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
				if ((*n).data.first == key) {
					res = n;
					break;
				} else if (!_comparator((*n).data.first, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data.first, key)) {
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
				if ((*n).data.first == key) {
					if (!n->right)
						break;
					n = n->right;
				} else if (!_comparator((*n).data.first, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data.first, key)) {
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
				if ((*n).data.first == key) {
					if (!n->right)
						break;
					n = n->right;
				} else if (!_comparator((*n).data.first, key)) {
					res = n;
					if (!n->left)
						break;
					n = n->left;
				} else if (_comparator((*n).data.first, key)) {
					if (!n->right)
						break;
					n = n->right;
				}
			}
			return const_iterator(res);
		}

		iterator find(const Key &key) {
			return iterator(_tree->find(ft::make_pair(key, T())));
		}

		const_iterator find(const Key &key) const {
			return const_iterator(_tree->find(ft::make_pair(key, T())));
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
				keys.push_back(it->first);
			for (typename ft::vector<Key>::iterator it = keys.begin(); it != keys.end(); it++)
				erase(*it);
		}

		size_type count(const Key &key) const {
			size_type s = 0;
			for (const_iterator it = begin(); it != end(); it++)
				if ((*it).first == key)
					s++;
			return s;
		}

		void swap(map& other) {
			map temp = *this;
			*this = other;
			other = temp;
		}

		key_compare key_comp() const { return Compare(); }

		value_compare value_comp() const { return value_compare(); }
	};

	template<class Key, class T, class Compare, class Alloc>
	bool operator==(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs) {
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator!=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs) {
		return (!(lhs == rhs));
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator<(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator<=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs) {
		return (!(rhs < lhs));
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator>(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs) {
		return (rhs < lhs);
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator>=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs) {
		return (!(lhs < rhs));
	}


}

#endif
