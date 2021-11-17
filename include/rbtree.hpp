#ifndef FT_CONTAINERS_RBTREE_HPP
#define FT_CONTAINERS_RBTREE_HPP

#include <iostream>

namespace ft {

	template<class T>
	class RBNode {

	public:
		typedef T			value_type;
		typedef RBNode<T>*	node;

	public:
		node		parent;
		node		left;
		node		right;
		int8_t		color;
		value_type	data;

	public:
		RBNode(const value_type& data = value_type()) : parent(nullptr), left(nullptr), right(nullptr), color(1), data(data) {};
		~RBNode() {};

		bool isRoot() { return !parent; };
		bool isLeft() { return parent && parent->left == this; };
		bool isRight() { return parent && parent->right == this; };
		bool isRed() { return color; };

		node getSibling() {
			if (isRoot()) return nullptr;
			if (isLeft()) return parent->right;
			else return parent->left;
		};

		node getUncle() {
			if (isRoot()) return nullptr;
			return parent->getSibling();
		};

		node getGrandParent() {
			if (isRoot()) return nullptr;
			return parent->parent;
		};

	};

	template<class T, class U>
	bool operator<(const ft::RBNode<T>& lhs, const ft::RBNode<U>& rhs) {
		return lhs.data < rhs.data;
	}

	template<class T, class Compare = std::less< ft::RBNode<T> >, class Allocator = std::allocator< RBNode<T> > >
	class RBTree {

		typedef RBNode<T>									node;
		typedef node*										node_pointer;
		typedef node&										node_reference;
		typedef Allocator									allocator_type;
		typedef Compare										key_compare;

		/*typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef typename allocator_type::value_type			value_type;
		typedef typename allocator_type::difference_type	difference_type;
		typedef typename allocator_type::size_type			size_type;*/

	protected:
		allocator_type	_allocator;
		key_compare		_comparator;
		node_pointer	_root;

	private:
		node_pointer createNode(T data) {
			return new RBNode<T>(data);
		}

		void binaryInsert(node_pointer x) {
			node_pointer* current = &_root;
			node_pointer parent = nullptr;
			while (*current) {

				parent = *current;

				if (!_comparator(x->data, parent->data) && !_comparator(parent->data, x->data)) {
					parent->data = x->data; //TODO: To see / REPLACE
					break;
				}
				else if (_comparator(x->data, parent->data))
					current = &parent->left;
				else
					current = &parent->right;

			}
			*current = x;
			x->parent = parent;
		}

		void fixTree(node_pointer x) {

			// CASE 1
			if (!x->parent)
				x->color = 0;

			// CASE 2
			else if (x->parent->isRed()) {

				if (x->getUncle() && x->getUncle()->isRed()) {

					x->parent->color = 0;
					x->getUncle()->color = 0;
					x->getGrandParent()->color = 1;
					fixTree(x->getGrandParent());

				}

				// CASE 4
				else {

					if (x->isRight() && x->parent->isLeft()) {
						rotateLeft(x->parent);
						x = x->left;
					}

					else if (x->isLeft() && x->parent->isRight()) {
						rotateRight(x->parent);
						x = x->right;
					}

					// CASE 5
					x->parent->color = 0;
					x->getGrandParent()->color = 1;

					if (x->isLeft() && x->parent->isLeft())
						rotateRight(x->getGrandParent());
					else
						rotateLeft(x->getGrandParent());
				}
			}
		}

		void rotateLeft(node_pointer x) {
			node_pointer y = x->right;
			x->right = y->left;
			if (y->left != NULL) {
				y->left->parent = x;
			}
			y->parent = x->parent;
			if (x->parent == NULL) {
				_root = y;
			}
			else if (x == x->parent->left) {
				x->parent->left = y;
			}
			else {
				x->parent->right = y;
			}
			y->left = x;
			x->parent = y;
		}

		void rotateRight(node_pointer x) {
			node_pointer y = x->left;
			x->left = y->right;
			if (y->right != NULL) {
				y->right->parent = x;
			}
			y->parent = x->parent;
			if (x->parent == NULL) {
				_root = y;
			}
			else if (x == x->parent->right) {
				x->parent->right = y;
			}
			else {
				x->parent->left = y;
			}
			y->right = x;
			x->parent = y;
		}

	public:

		RBTree() : _allocator(allocator_type()), _root(nullptr) {};
		~RBTree() {};

		void insert(T data) {
			node_pointer entry = createNode(data);
			binaryInsert(entry);
			fixTree(entry);
		}

		node_pointer find(T data) {
			node_pointer current = _root;
			while (current != nullptr) {

				if (!_comp(data, current->data) && !_comp(current->data, data))
					return current;
				else if (_comp(data, current->data))
					current = current->left;
				else
					current = current->right;

			}
			return current;
		}



# define NORMAL "\033[0;37m"
# define BOLD_RED "\033[1;31m"

		void print_tree(std::string s = "") {
			(void)s;
			//return;
			std::cout << NORMAL << "<< " << s << " ####################" << std::endl;
			_print_tree(_root);
			std::cout << NORMAL << s << " >> ####################" << std::endl;
		}

		void _print_tree(node* n, size_t l = 0) {

			if (!n) {
				std::cout << std::endl;
				return;
			}
			_print_tree(n->right, l + 1);
			std::string coll = n->isRed() ? BOLD_RED : NORMAL;
			std::cout << coll << std::string(l * 4,' ');
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
