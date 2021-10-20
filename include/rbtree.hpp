#ifndef FT_CONTAINERS_RBTREE_HPP
#define FT_CONTAINERS_RBTREE_HPP

#include <iostream>

namespace ft {

	template<class T>
	class RBNode {

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

		void setParent(node parent) { this->parent = parent; }
		void setLeft(node left) { this->left = left; }
		void setRight(node right) { this->right = right; }
		void setColor(int8_t color) { this->color = color; }
		void setData(value_type data) { this->data = data; }

		value_type getData() { return data; }
		node getParent() { return parent; }
		node getLeft() { return left; }
		node getRight() { return right; }

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

	template<class T, class Allocator = std::allocator<RBNode<T> > >
	class RBTree {

		typedef RBNode<T>									node;
		typedef node*										node_pointer;
		typedef node&										node_reference;
		typedef Allocator									allocator_type;

		/*typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef typename allocator_type::value_type			value_type;
		typedef typename allocator_type::difference_type	difference_type;
		typedef typename allocator_type::size_type			size_type;*/

	protected:
		allocator_type	_allocator;
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
				if (x->getData() == parent->getData()) {
					parent->setData(x->getData()); //REPLACE
					break;
				}

				else if (x->getData() < parent->getData())
					current = &parent->left;
				else
					current = &parent->right;

			}
			*current = x;
			x->setParent(parent);
		}

		void fixTree(node_pointer x) {

			// CASE 1
			if (!x->getParent()) {
				x->setColor(0);
			}

			// CASE 2
			else if (x->getParent()->isRed()) {

				if (x->getUncle() && x->getUncle()->isRed()) {
					x->getParent()->setColor(0);
					x->getUncle()->setColor(0);
					x->getGrandParent()->setColor(1);
					fixTree(x->getGrandParent());
				}

				// CASE 4
				else {

					if (x->isRight() && x->getParent()->isLeft()) {
						rotateLeft(x->getParent());
						x = x->getLeft();
					}

					else if (x->isLeft() && x->getParent()->isRight()) {
						rotateRight(x->getParent());
						x = x->getRight();
					}

					// CASE 5
					x->getParent()->setColor(0);
					x->getGrandParent()->setColor(1);

					if (x->isLeft() && x->getParent()->isLeft())
						rotateRight(x->getGrandParent());
					else
						rotateLeft(x->getGrandParent());
				}
			}
		}

		void rotateLeft(node_pointer x) {
			node_pointer y = x->getRight();
			x->setRight(y->getLeft());
			if (y->getLeft() != NULL) {
				y->getLeft()->setParent(x);
			}
			y->setParent(x->getParent());
			if (x->getParent() == NULL) {
				_root = y;
			}
			else if (x == x->getParent()->getLeft()) {
				x->getParent()->setLeft(y);
			}
			else {
				x->getParent()->setRight(y);
			}
			y->setLeft(x);
			x->setParent(y);
		}

		void rotateRight(node_pointer x) {
			node_pointer y = x->getLeft();
			x->setLeft(y->getRight());
			if (y->getRight() != NULL) {
				y->getRight()->setParent(x);
			}
			y->setParent(x->getParent());
			if (x->getParent() == NULL) {
				_root = y;
			}
			else if (x == x->getParent()->getRight()) {
				x->getParent()->setRight(y);
			}
			else {
				x->getParent()->setLeft(y);
			}
			y->setRight(x);
			x->setParent(y);
		}

	public:

		RBTree() : _allocator(allocator_type()), _root(nullptr) {};
		~RBTree() {};

		void insert(T data) {
			node_pointer entry = createNode(data);
			binaryInsert(entry);
			fixTree(entry);
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
			_print_tree(n->getRight(), l + 1);
			std::string coll = n->isRed() ? BOLD_RED : NORMAL;
			std::cout << coll << std::string(l * 4,' ');
			if (n->getParent()) {
				std::cout << "(" << n->getParent()->getData() << ")";
				if (n->isLeft())
					std::cout << "\\";
				else
					std::cout << "/";
			}
			std::cout << n->getData() << NORMAL;
			_print_tree(n->getLeft(), l + 1);
		}
	};


}

#endif
