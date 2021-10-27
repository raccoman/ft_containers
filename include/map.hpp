#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP

#include <algorithm.hpp>
#include <rbtree.hpp>

namespace ft {

	template<
	        class Key,
			class T,
			class Compare = std::less<Key>,
			class Allocator = std::allocator<std::pair<const Key, T> >
	>
	class map {

	public:
		typedef Key										key_type;
		typedef T										mapped_type;
		typedef ft::pair<const Key, T>					value_type;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef Compare									key_compare;
		typedef Allocator								allocator_type;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef map_iterator<value_type>				iterator;
		typedef map_iterator<const value_type>			const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef RBNode<value_type>*						node_pointer;

		class value_compare: public ft::binary_function<value_type, value_type, bool> {
		protected:
			key_compare comp;
		public:

			value_compare(): comp() {}
			value_compare(key_compare c): comp(c) {}

			bool operator()(value_type& lhs, value_type& rhs) {
				return comp(lhs.first, rhs.first);
			}

			bool operator()(const value_type& lhs, const value_type& rhs) const {
				return comp(lhs.first, rhs.first);
			}

		};

	protected:
		RBTree<value_type, value_compare>*	_tree;
		key_compare							_comparator;
		allocator_type						_allocator;
		size_type							_size;

	public:

		map() : _allocator(Allocator()), _size(0) {
			_tree = new RBTree<value_type, value_compare>;
		}

		explicit map(const Compare& comp, const Allocator& alloc = Allocator()) : _comparator(comp), _allocator(alloc), _size(0) {
			_tree = new RBTree<value_type, value_compare>;
		}

		template<class InputIt>
		map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : _comparator(comp), _allocator(alloc) {
			_tree = new RBTree<value_type, value_compare>;
			_size = last - first;
			while (first != last)
				_tree->insert(*first++);
		}

		~map() {
			clear();
			delete _tree;
		}

		map& operator=(const map& other) {
			clear();
			//TODO: Iterate elements, copy and insert them into the tree
			return *this;
		}

		allocator_type get_allocator() const {
			return _allocator;
		}

		T& at(const Key& key) {
			node_pointer res = _tree->find(key);
			if (res == nullptr) return res;
			return res->data.second;
		}

		const T& at(const Key& key) const {
			node_pointer res = _tree->find(key);
			if (res == nullptr) return res;
			return res->data.second;
		}

		T& operator[](const Key& key) {
			node_pointer res = _tree->find(key);
			if (res != nullptr)
				return res->data.second;
			return insert(ft::make_pair(key, T())).first->second;
		}

		iterator begin() {

		}

		void clear() {
			_size = 0;
			_tree->clear();
		}



	};

}

#endif
