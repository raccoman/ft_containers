#ifndef FT_CONTAINERS_RBTREE_HPP
#define FT_CONTAINERS_RBTREE_HPP

#include <iostream>
#include <ft_iterator.hpp>
#include <type_traits>

namespace ft {

	enum Color {
		RED, BLACK, DOUBLE_BLACK
	};

	template<class T>
	class RBNode {

	public:
		typedef T value_type;
		typedef RBNode<T> *node_pointer;

	public:
		value_type data;
		Color color;
		node_pointer parent, left, right;

	public:

		RBNode(const value_type &data = value_type(), Color color = RED)
				: data(data), color(color), parent(nullptr), left(nullptr), right(nullptr) {};

		RBNode(const RBNode &other, const value_type &data)
				: data(data), color(other.color), parent(other.parent), left(other.left), right(other.right) {}

		~RBNode() {};

		bool isRoot() { return !parent; };

		bool isLeft() { return parent && parent->left == this; };

		bool isRight() { return parent && parent->right == this; };

		bool isRed() { return color == RED; };

		bool isBlack() { return color == BLACK; };

		bool isDoubleBlack() { return color == DOUBLE_BLACK; };

		node_pointer sibling() {
			if (isRoot()) return nullptr;
			if (isLeft()) return parent->right;
			else return parent->left;
		};

		node_pointer uncle() {
			if (isRoot()) return nullptr;
			return parent->getSibling();
		};

		node_pointer grandparent() {
			if (isRoot()) return nullptr;
			return parent->parent;
		};

	};

	template<class T, class U>
	bool operator<(const ft::RBNode<T> &lhs, const ft::RBNode<U> &rhs) {
		return lhs.data < rhs.data;
	}


	template<class T, class Compare = ft::less<T>, bool isConst = false>
	class rbtree_iterator {
	public:
		typedef T															iterator_type;
		typedef typename iterator_traits<iterator_type>::value_type			value_type;
		typedef typename iterator_traits<iterator_type>::difference_type	difference_type;
		typedef typename iterator_traits<iterator_type>::pointer			pointer;
		typedef typename iterator_traits<iterator_type>::reference			reference;
		typedef typename iterator_traits<iterator_type>::iterator_category	iterator_category;
		typedef RBNode<value_type>*											node_pointer;

	protected:
		node_pointer ptr;
		Compare compare;

	public:
		rbtree_iterator() : ptr(nullptr) {}

		rbtree_iterator(node_pointer ptr) : ptr(ptr) {}

		rbtree_iterator(const RBNode<value_type> *ptr) : ptr(const_cast<node_pointer>(ptr)) {}

		template<class U>
		rbtree_iterator(const rbtree_iterator<U, Compare> &other) : ptr(const_cast<node_pointer>(other.base())) {}

		~rbtree_iterator() {}

		reference operator*() { return ptr->data; }

		pointer operator->() { return &ptr->data; }

		reference operator*() const { return ptr->data; }

		pointer operator->() const { return &ptr->data; }

		rbtree_iterator &operator++() {
			if (ptr->right) {
				ptr = ptr->right;
				while (ptr->left)
					ptr = ptr->left;
			} else {
				while (ptr->parent) {
					if (!ptr->isDoubleBlack() && !compare(ptr->parent->data, ptr->data) && ptr->parent->data != ptr->data) {
						ptr = ptr->parent;
						return (*this);
					}
					ptr = ptr->parent;
				}
			}
			return *this;

		}

		rbtree_iterator operator++(int) {
			rbtree_iterator tmp = *this;
			++*this;
			return tmp;
		}

		rbtree_iterator &operator--() {

			if (ptr->left) {
				ptr = ptr->left;
				while (ptr->right && !ptr->right->isDoubleBlack())
					ptr = ptr->right;
			} else {

				while (ptr->parent) {

					if (ptr->isDoubleBlack() || compare(ptr->parent->data, ptr->data)) {
						ptr = ptr->parent;
						return *this;
					}

					ptr = ptr->parent;
				}
			}
			return *this;
		}

		rbtree_iterator operator--(int) {
			rbtree_iterator tmp = *this;
			--*this;
			return tmp;
		}

		template<class U>
		bool operator==(const U &other) { return ptr == other.base(); }

		template<class U>
		bool operator!=(const U &other) { return ptr != other.base(); }

		node_pointer base() { return ptr; }

		node_pointer base() const { return ptr; }

		Compare comparator() { return compare; }
	};

	template<class T, class Compare = ft::less<RBNode<T> >, class Alloc = std::allocator<RBNode<T> > >
	class RBTree {

	public:
		typedef T value_type;
		typedef Alloc allocator_type;
		typedef typename allocator_type::size_type size_type;
		typedef RBNode<T> node_type;
		typedef node_type *node_pointer;

	private:
		node_pointer _root;
		allocator_type _allocator;
		Compare _comparator;
		node_pointer _end;
		node_pointer _start;

	protected:
		Color getColor(node_pointer node) {
			if (node == nullptr) return BLACK;
			return node->color;
		}

		void setColor(node_pointer node, Color color) {
			if (node == nullptr) return;
			node->color = color;
		}

		void rotateLeft(node_pointer ptr) {
			node_pointer right_child = ptr->right;
			ptr->right = right_child->left;

			if (ptr->right != nullptr)
				ptr->right->parent = ptr;

			right_child->parent = ptr->parent;

			if (ptr->parent == nullptr)
				_root = right_child;
			else if (ptr == ptr->parent->left)
				ptr->parent->left = right_child;
			else
				ptr->parent->right = right_child;

			right_child->left = ptr;
			ptr->parent = right_child;
		}

		void rotateRight(node_pointer ptr) {
			node_pointer left_child = ptr->left;
			ptr->left = left_child->right;

			if (ptr->left != nullptr)
				ptr->left->parent = ptr;

			left_child->parent = ptr->parent;

			if (ptr->parent == nullptr)
				_root = left_child;
			else if (ptr == ptr->parent->left)
				ptr->parent->left = left_child;
			else
				ptr->parent->right = left_child;

			left_child->right = ptr;
			ptr->parent = left_child;
		}

		node_pointer minValueNode(node_pointer node) {
			node_pointer ptr = node;
			while (ptr->left != nullptr && !ptr->isDoubleBlack())
				ptr = ptr->left;
			return ptr;
		}

		node_pointer maxValueNode(node_pointer node) {
			node_pointer ptr = node;
			while (ptr->right != nullptr && !ptr->isDoubleBlack())
				ptr = ptr->right;
			return ptr;
		}

		size_type getBlackHeight(node_pointer node) {
			size_type height = 0;
			while (node != nullptr) {
				if (getColor(node) == BLACK)
					height++;
				node = node->left;
			}
			return height;
		}

		void fixDependencies(node_pointer node) {
			if (node->left)
				node->left->parent = node;
			if (node->right)
				node->right->parent = node;
		}
		node_pointer* selfParentPtr(node_pointer node) {
			if (!node->parent)
				return &_root;
			else if (node->isLeft())
				return &node->parent->left;
			else if (node->isRight())
				return &node->parent->right;
			throw std::exception();
		}
		void swapNode(node_pointer lhs, node_pointer rhs) {
			*selfParentPtr(lhs) = rhs;
			*selfParentPtr(rhs) = lhs;
			std::swap(lhs->parent, rhs->parent);
			std::swap(lhs->left, rhs->left);
			std::swap(lhs->right, rhs->right);
			std::swap(lhs->color, rhs->color);
			fixDependencies(lhs);
			fixDependencies(rhs);
		}
		node_pointer replaceNodeWithLoneChild(node_pointer a, node_pointer b) {
			if (b->parent != a) throw std::exception();
			if (a->left && a->right) throw std::exception();
			if (!a->left && !a->right) throw std::exception();
			b->parent = a->parent;
			*selfParentPtr(a) = b;
			return b;
		}

		void fixInsertRBTree(node_pointer ptr) {
			node_pointer parent = nullptr;
			node_pointer grandparent = nullptr;
			while (ptr != _root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
				parent = ptr->parent;
				grandparent = parent->parent;
				if (parent == grandparent->left) {
					node_pointer uncle = grandparent->right;
					if (getColor(uncle) == RED) {
						setColor(uncle, BLACK);
						setColor(parent, BLACK);
						setColor(grandparent, RED);
						ptr = grandparent;
					} else {
						if (ptr == parent->right) {
							rotateLeft(parent);
							ptr = parent;
							parent = ptr->parent;
						}
						rotateRight(grandparent);
						std::swap(parent->color, grandparent->color);
						ptr = parent;
					}
				} else {
					node_pointer uncle = grandparent->left;
					if (getColor(uncle) == RED) {
						setColor(uncle, BLACK);
						setColor(parent, BLACK);
						setColor(grandparent, RED);
						ptr = grandparent;
					} else {
						if (ptr == parent->left) {
							rotateRight(parent);
							ptr = parent;
							parent = ptr->parent;
						}
						rotateLeft(grandparent);
						std::swap(parent->color, grandparent->color);
						ptr = parent;
					}
				}
			}
			setColor(_root, BLACK);
		}

		node_pointer insertBST(node_pointer root, node_pointer ptr) {
			if (root == nullptr || root->isDoubleBlack()) return ptr;
			if (_comparator(ptr->data, root->data)) {

				root->left = insertBST(root->left, ptr);
				root->left->parent = root;

		} else if (!_comparator(ptr->data, root->data)) {

				root->right = insertBST(root->right, ptr);
				root->right->parent = root;

			}
			return root;
		}

		void fixDeleteRBTree(node_pointer node) {
			if (node->parent) {							// case 1
				if (node->sibling() && node->sibling()->color == RED) 		// case 2
				{
					node->parent->color = RED;
					node->sibling()->color = BLACK;
					if (node->isLeft())
						rotateLeft(node->parent);
					else
						rotateRight(node->parent);
				}
				if (node->parent->color == BLACK && node->sibling() &&		// case 3
					node->sibling()->color == BLACK &&
					(!node->sibling()->left || node->sibling()->left->color == BLACK) &&
					((!node->sibling()->right || node->sibling()->right->isDoubleBlack()) || node->sibling()->right->color == BLACK))
				{
					node->sibling()->color = RED;
					fixDeleteRBTree(node->parent);
				}
				else if (node->parent->color == RED && node->sibling() &&		// case 4
						 node->sibling()->color == BLACK &&
						 (!node->sibling()->left || node->sibling()->left->color == BLACK) &&
						 ((!node->sibling()->right || node->sibling()->right->isDoubleBlack()) || node->sibling()->right->color == BLACK))
				{
					node->sibling()->color = RED;
					node->parent->color = BLACK;
				}
				else if (node->isLeft() && node->sibling() &&				// case 5
						 node->sibling()->color == BLACK &&
						 node->sibling()->left && node->sibling()->left->color == RED &&
						 ((!node->sibling()->right || node->sibling()->right->isDoubleBlack()) || node->sibling()->right->color == BLACK))
				{
					node->sibling()->color = RED;
					node->sibling()->left->color = BLACK;
					rotateRight(node->sibling());
				}
				else if (node->isRight() && node->sibling() &&			// still case 5
						 node->sibling()->color == BLACK &&
						 node->sibling()->right && !node->sibling()->right->isDoubleBlack() && node->sibling()->right->color == RED &&
						 (!node->sibling()->left || node->sibling()->left->color == BLACK))
				{
					node->sibling()->color = RED;
					node->sibling()->right->color = BLACK;
					rotateLeft(node->sibling());
				}
				if (node->sibling())
					node->sibling()->color = node->parent->color; // case 6
				node->parent->color = BLACK;

				if (node->sibling()) {
					if (node->isLeft()) {
						if (node->sibling()->right && !node->sibling()->right->isDoubleBlack())
							node->sibling()->right->color = BLACK;
						rotateLeft(node->parent);
					} else {
						if (node->sibling()->right && !node->sibling()->right->isDoubleBlack())
							node->sibling()->left->color = BLACK;
						rotateRight(node->parent);
					}
				}
			}
		}

		void deleteBST(node_pointer node) {

			if (!node->parent && !node->left && (!node->right || node->right->isDoubleBlack()))
				_root = NULL;

			else if (!node->left && (!node->right || node->right->isDoubleBlack())) {

				if (node->isLeft())
					node->parent->left = NULL;
				else
					node->parent->right = NULL;

			} else if (node->left && node->right && !node->right->isDoubleBlack()) {

				node_pointer min = minValueNode(node->right);
				swapNode(node, min);
				deleteBST(node);

			} else if (!node->left) {

				node = replaceNodeWithLoneChild(node, node->right);
				fixDeleteRBTree(node);

			} else if (!node->right || node->right->isDoubleBlack()) {
				node = replaceNodeWithLoneChild(node, node->left);
				fixDeleteRBTree(node);
			}

		}

		void covid19(node_pointer node) {
			if (!node || node->isDoubleBlack()) return;

			covid19(node->left);
			covid19(node->right);

			_allocator.destroy(node);
			_allocator.deallocate(node, 1);
		}

		void fixSEPoints() {

			if (!_root) {
				_start = _root;
				_end = _root;
				return;
			}

			node_pointer current = _root;
			while (current->left != nullptr)
				current = current->left;
			_start = current;

			if (!_end)
				_allocator.construct(_end, node_type(value_type(), DOUBLE_BLACK));

			current = _root;
			while (current->right != nullptr && !current->right->isDoubleBlack())
				current = current->right;
			current->right = _end;
			_end->parent = current;
		}

	public:
		RBTree(const allocator_type &alloc = allocator_type())
				: _root(nullptr), _allocator(alloc), _end(nullptr), _start(nullptr) {
			_end = _allocator.allocate(1);
			_allocator.construct(_end, node_type(value_type(), DOUBLE_BLACK));
		}

		~RBTree() {
			_allocator.destroy(_end);
			_allocator.deallocate(_end, 1);
		}

		node_pointer insert(const value_type &data) {
			node_pointer node = _allocator.allocate(1);
			_allocator.construct(node, RBNode<value_type>(data));
			_root = insertBST(_root, node);
			fixInsertRBTree(node);
			fixSEPoints();

			return node;
		}

		void remove(node_pointer node) {
			if (!node) return;
			deleteBST(node);
			fixSEPoints();
		}

		node_pointer find(const value_type &data) {
			node_pointer current = _root;
			while (current != nullptr && !current->isDoubleBlack()) {
				if (!_comparator(data, current->data) && !_comparator(current->data, data))
					return current;
				else if (_comparator(data, current->data))
					current = current->left;
				else
					current = current->right;
			}
			return _end;
		}

		void clear() {
			covid19(_root);
			_start = nullptr;
			_end = nullptr;
		}

		size_type max_size() { return _allocator.max_size(); }

		node_pointer end() { return _end; }

		node_pointer begin() { return _start; }

		node_pointer root() { return _root; }

# define NORMAL "\033[0;37m"
# define BOLD_RED "\033[1;31m"

		void print_tree(std::string s = "") {
			(void) s;
			//return;
			std::cout << NORMAL << "===============================" << std::endl;
			_print_tree(_root);
			std::cout << NORMAL << "END NODE AFTER: " << _end->parent->data << std::endl;
			std::cout << NORMAL << "===============================" << std::endl;
		}

		void _print_tree(node_pointer n, size_t l = 0) {

			if (!n || n->isDoubleBlack()) {
				std::cout << std::endl;
				return;
			}

			_print_tree(n->right, l + 1);
			std::string coll = n->isRed() ? BOLD_RED : NORMAL;
			std::cout << coll << std::string(l * 4, ' ');
			if (n->parent) {
				std::cout << "(" << n->parent->data << ")";
				if (n->isLeft())
					std::cout << "\\";
				else
					std::cout << "/";
			}
			std::cout << n->data << NORMAL;
			_print_tree(n->left, l + 1);
		}

	};

}

#endif
