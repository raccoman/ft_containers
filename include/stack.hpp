#ifndef FT_CONTAINERS_STACK_HPP
#define FT_CONTAINERS_STACK_HPP

#include <vector.hpp>

namespace ft {

	template<class T, class Container = ft::vector<T> >
	class stack {

	public:
		typedef Container									container_type;
		typedef typename container_type::value_type			value_type;
		typedef typename container_type::size_type			size_type;
		typedef typename container_type::reference			reference;
		typedef typename container_type::const_reference	const_reference;

	public:
		container_type	container;

	public:
		explicit stack(const Container& cont = Container()) : container(cont) {};

		~stack() {};

		stack& operator=(const stack& other) {
			container = other.container;
			return *this;
		}

		reference top() {
			return container.back();
		}

		const_reference top() const {
			return container.back();
		}

		bool empty() const {
			return container.empty();
		}

		size_type size() const {
			return container.size();
		}

		void push(const value_type& value) {
			container.push_back(value);
		}

		void pop() {
			container.pop_back();
		}
	};

	template< class T, class Container >
	bool operator==(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.container == rhs.container;
	}

	template< class T, class Container >
	bool operator!=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.container != rhs.container;
	}

	template< class T, class Container >
	bool operator<(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.container < rhs.container;
	}

	template< class T, class Container >
	bool operator<=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.container <= rhs.container;
	}

	template< class T, class Container >
	bool operator>(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.container > rhs.container;
	}

	template< class T, class Container >
	bool operator>=(const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
		return lhs.container >= rhs.container;
	}

}

#endif
